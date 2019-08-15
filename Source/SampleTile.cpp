#include "SampleTile.h"

#include "SamplifyLookAndFeel.h"
#include "TagTile.h"
#include "SamplifyMainComponent.h"

#include <iomanip>
#include <sstream>

using namespace samplify;

SampleTile::SampleTile(Sample::Reference sample) : mTagContainer(false)
{
	setRepaintsOnMouseActivity(true);
	setSize(SAMPLE_TILE_MIN_WIDTH, SAMPLE_TILE_MIN_WIDTH * SAMPLE_TILE_ASPECT_RATIO);
	setSample(sample);
	addAndMakeVisible(mTagContainer);
}

SampleTile::~SampleTile()
{
}
void SampleTile::paint (Graphics& g)
{
	SamplifyProperties::getInstance()->getSampleLibrary()->checkThreadFinished();
	if (!mSample.isNull())
	{
		const Rectangle<float> titleRect = getTitleRect();
		const Rectangle<float> timeRect = getTimeRect();
		const Rectangle<float> typeRect = getTypeRect();
		const Rectangle<float> thumbnailRect = getThumbnailRect();
		const Rectangle<float> parentDirRect = getParentDirRect();
		float tileCornerRadius = 2.0f;
		//setup colors to use
		Colour backgroundColor;
		Colour foregroundColor;
		if (isMouseOver(true))
		{
			backgroundColor = getLookAndFeel().findColour(SAMPLE_TILE_BG_HOVER_COLOR_ID);
			foregroundColor = getLookAndFeel().findColour(SAMPLE_TILE_FG_HOVER_COLOR_ID);
		}
		else
		{
			backgroundColor = getLookAndFeel().findColour(SAMPLE_TILE_BG_DEFAULT_COLOR_ID);
			foregroundColor = getLookAndFeel().findColour(SAMPLE_TILE_FG_DEFAULT_COLOR_ID);
		}
		g.setColour(backgroundColor);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), tileCornerRadius);
		g.setFont(SAMPLE_TILE_TITLE_FONT);
		g.setColour(Colours::black);
		// draw an outline around the component
		g.drawRoundedRectangle(getLocalBounds().toFloat(), tileCornerRadius, 2.0f);
		g.drawLine(thumbnailRect.getTopLeft().x, 
			thumbnailRect.getTopLeft().y,
			0, 
			getWidth(), 
			2.0f);
		g.drawLine(titleRect.getBottomLeft().x,
			titleRect.getBottomLeft().y,
			titleRect.getBottomRight().x,
			titleRect.getBottomRight().y,
			2.0f);
		g.drawLine(timeRect.getTopRight().x, 
			timeRect.getTopRight().y,
			timeRect.getBottomRight().x,
			timeRect.getBottomRight().y,
			2.0f);
		g.drawLine(typeRect.getTopRight().x,
			typeRect.getTopRight().y,
			typeRect.getBottomRight().x,
			typeRect.getBottomRight().y,
			2.0f);
		g.drawLine(parentDirRect.getTopLeft().x,
			parentDirRect.getTopLeft().y,
			parentDirRect.getTopRight().x,
			parentDirRect.getTopRight().y);
		g.setColour(Colours::darkslategrey);
		g.drawText(mSample.getFilename(), titleRect, Justification::centredLeft);
		g.drawText(mSample.getRelativeParentFolders()[0], parentDirRect, Justification::centred);
		g.setColour(foregroundColor);

		switch (mSample.getSampleType())
		{
		case Sample::SampleType::ONESHOT:
			FontAwesome::drawCenterd(g, FontAwesome_Share, 30.0f, foregroundColor, typeRect.toNearestInt());
			break;
		case Sample::SampleType::LOOP:
			FontAwesome::drawCenterd(g, FontAwesome_Refresh, 30.0f, foregroundColor, typeRect.toNearestInt());
			break;
		default:
			break;
		}

		g.setFont(16.0f);
		std::stringstream str;
		str << std::fixed << std::setprecision(2) << mSample.getLength();
		g.drawText(String(str.str()), timeRect.withLeft(timeRect.getTopLeft().x + 2.0f), Justification::centred);

		SampleAudioThumbnail::Reference thumbnail = mSample.getThumbnail();
		if (!thumbnail.isNull())
		{
			if (thumbnail.getNumChannels() != 0)
			{
				thumbnail.drawChannel(g, thumbnailRect.toNearestInt(), 0.0, thumbnail.getTotalLength(), 0, 1.0f);
			}
		}
		std::shared_ptr<AudioPlayer> auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
		if (auxPlayer->getFile() == mSample.getFile())
		{
			float startT = auxPlayer->getStartCueRelative();
			float currentT = auxPlayer->getRelativeTime();
			float startX = thumbnailRect.getTopLeft().x + ((thumbnailRect.getTopRight().x - thumbnailRect.getTopLeft().x) * startT);
			float currentX = thumbnailRect.getTopLeft().x + ((thumbnailRect.getTopRight().x - thumbnailRect.getTopLeft().x) * currentT);
			float y1 = thumbnailRect.getTopLeft().y;
			float y2 = thumbnailRect.getBottomLeft().y;
			g.setColour(Colours::black);
			g.drawLine(startX, y1, startX, y2, 1.0f);
			if (auxPlayer->getState() == AudioPlayer::TransportState::Playing)
			{
				g.setColour(Colours::red);
				g.drawLine(currentX, y1, currentX, y2, 1.0f);
				repaint();
			}

		}
		if (SamplifyProperties::getInstance()->getSampleLibrary()->isUpdating())
		{
			g.setColour(Colours::black);
			g.drawText("UPDATING", getLocalBounds(), Justification::centred);
		}
		mTagContainer.setTags(mSample.getTags());
	}
	else
	{
		mTagContainer.setTags(StringArray());
	}
}

void SampleTile::resized()
{
	mTagContainer.setBounds(getTagRect().toNearestInt());
}

bool SampleTile::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
	if (dragSourceDetails.description == "Tags")
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SampleTile::mouseDown(const MouseEvent& e)
{
	Rectangle audiowaveRect = getThumbnailRect();
	if (!audiowaveRect.contains(e.getMouseDownPosition().toFloat()) && e.mods.isRightButtonDown())
	{
		PopupMenu menu;
		menu.addItem(1, "Open Q-Editor", false, false);
		menu.addSeparator();
		menu.addItem(2, "Rename Sample", false, false);
		menu.addItem(3, "Delete Sample", false, false);
		int selection = menu.show();
	}
}

void SampleTile::mouseUp(const MouseEvent& e)
{
	if (!mSample.isNull())
	{
		int widthSegment = getWidth() / 4;
		int heightSegment = getHeight() / 3;
		Rectangle thumbnailRect = getThumbnailRect();
		Rectangle parentDirRect = getParentDirRect();
		if (thumbnailRect.contains(e.getMouseDownPosition().toFloat()))
		{
			if (e.mods.isLeftButtonDown())
			{
				playSample();
			}
			else if (e.mods.isRightButtonDown())
			{
				float rectWidth = thumbnailRect.getWidth();
				float mouseDownX = e.getMouseDownX();
				playSample(mouseDownX / rectWidth);
			}
			SamplifyProperties::getInstance()->getAudioPlayer()->addChangeListener(this);
		}
		if (parentDirRect.contains(e.getMouseDownPosition().toFloat()))
		{
			PopupMenu menu;
			StringArray parentDirs = mSample.getRelativeParentFolders();
			for (int i = 0; i < parentDirs.size(); i++)
			{
				menu.addItem(i + 1, parentDirs[i]);
			}
			int choice = menu.show();
			File newDir = mSample.getFile();
			for (int i = 0; i < choice; i++)
			{
				newDir = newDir.getParentDirectory();
			}
			SamplifyProperties::getInstance()->getSampleLibrary()->updateCurrentSamples(newDir);
		}
	}
}

void samplify::SampleTile::mouseDrag(const MouseEvent& e)
{
	if (!mSample.isNull())
	{
		StringArray files = StringArray();
		files.add(mSample.getFullPathName());
		DragAndDropContainer::performExternalDragDropOfFiles(files, false);
	}
}

void SampleTile::mouseMove(const MouseEvent & e)
{
	repaint();
}

void SampleTile::playSample()
{
	playSample(0.0f);
}

void SampleTile::playSample(float t)
{
	if (!mSample.isNull())
	{
		std::shared_ptr<AudioPlayer> auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
		if (auxPlayer->getFile() != mSample.getFile())
		{
			auxPlayer->loadFile(mSample);
		}
		else
		{
			auxPlayer->stop();
			auxPlayer->reset();
		}
		auxPlayer->setRelativeTime(t);
		auxPlayer->play();
	}

}

void SampleTile::itemDropped(const SourceDetails & dragSourceDetails)
{
	if (!mSample.isNull())
	{
		if (TagTile * tagComp = dynamic_cast<TagTile*>(dragSourceDetails.sourceComponent.get()))
		{
			mSample.addTag(tagComp->getTag());
			mTagContainer.setTags(mSample.getTags());
			SamplifyMainComponent::getInstance()->getFilterExplorer().getTagExplorer().getTagContainer().resetTags();
		}
	}
}

void SampleTile::changeListenerCallback(ChangeBroadcaster* source)
{
	if (!mSample.isNull())
	{
		std::shared_ptr<AudioPlayer> aux = SamplifyProperties::getInstance()->getAudioPlayer();
		if (aux->getFile() == mSample.getFile())
		{
			if (!(aux->getState() == AudioPlayer::TransportState::Starting ||
				aux->getState() == AudioPlayer::TransportState::Playing))
			{
				aux->removeChangeListener(this);
			}

			repaint();
		}
	}
}

void SampleTile::setSample(Sample::Reference sample)
{
	if (!sample.isNull())
	{
		bool yes = true;
		if (!mSample.isNull())
		{
			if (mSample == sample)
			{
				yes = false;
			}
		}
		if (yes)
		{
			if (sample.getThumbnail().isNull())
			{
				sample.generateThumbnailAndCache();
			}
		}
	}
	mSample = sample;
	repaint();
}

Sample::Reference SampleTile::getSample()
{
	return mSample;
}


Rectangle<float> SampleTile::getTitleRect()
{
	return Rectangle<float>(0,0,getWidth() * 0.66, SAMPLE_TILE_TITLE_FONT.getHeight() + 2.0f);
}

Rectangle<float> SampleTile::getTypeRect()
{
	return Rectangle<float>(0, getTitleRect().getHeight(), getWidth() / 5, getWidth() / 5);
}

Rectangle<float> SampleTile::getTimeRect()
{
	return Rectangle<float>(getWidth() / 5, getTitleRect().getHeight(), getWidth()/5, getWidth()/5);
}

Rectangle<float> SampleTile::getThumbnailRect()
{
	float startY = getTitleRect().getHeight() + getTypeRect().getHeight();
	return Rectangle<float>(0, startY, getWidth(), getHeight() - startY);
}

Rectangle<float> SampleTile::getTagRect()
{
	float startX = getTypeRect().getWidth() + getTimeRect().getWidth();
	return Rectangle<float>(startX, getTitleRect().getHeight(), getWidth() - startX, getTypeRect().getHeight());
}

Rectangle<float> samplify::SampleTile::getParentDirRect()
{
	return Rectangle<float>(getWidth() * 0.66f, 0, getWidth() * 0.33, SAMPLE_TILE_TITLE_FONT.getHeight() + 2.0f);
}

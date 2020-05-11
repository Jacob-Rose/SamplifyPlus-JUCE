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
	mTagContainer.addMouseListener(this, false);
	addAndMakeVisible(mTagContainer);
}

SampleTile::~SampleTile()
{
}
void SampleTile::paint (Graphics& g)
{
	if (!mSample.isNull())
	{
		//setup colors to use
		Colour backgroundColor;
		Colour foregroundColor;
		Colour outlineColor;
		Colour titleColor;
		if (isMouseOver(true))
		{
			backgroundColor = getLookAndFeel().findColour(SAMPLE_TILE_BG_HOVER_COLOR_ID);
			foregroundColor = getLookAndFeel().findColour(SAMPLE_TILE_FG_HOVER_COLOR_ID);
			outlineColor = getLookAndFeel().findColour(SAMPLE_TILE_OUTLINE_HOVER_COLOR_ID);
		}
		else
		{
			backgroundColor = getLookAndFeel().findColour(SAMPLE_TILE_BG_DEFAULT_COLOR_ID);
			foregroundColor = getLookAndFeel().findColour(SAMPLE_TILE_FG_DEFAULT_COLOR_ID);
			outlineColor = getLookAndFeel().findColour(SAMPLE_TILE_OUTLINE_DEFAULT_COLOR_ID);
		}

		if (backgroundColor.getPerceivedBrightness() > 0.5f)
		{
			titleColor = Colours::slategrey;
		}
		else
		{
			titleColor = Colours::whitesmoke;
		}

		//Draw BG
		g.setColour(backgroundColor);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), SAMPLE_TILE_CORNER_RADIUS);
		if (mSample.getInfoText() != "")
		{
			//Draw info icon
			g.setColour(foregroundColor);
			g.fillEllipse(m_InfoIconRect.reduced(INFO_ICON_PADDING));
			g.setColour(outlineColor);
			g.drawEllipse(m_InfoIconRect.reduced(INFO_ICON_PADDING), SAMPLE_TILE_OUTLINE_THICKNESS);

			//Draw Title
			g.setFont(SAMPLE_TILE_TITLE_FONT);
			g.setColour(titleColor);
			g.drawText(mSample.getFilename(), m_TitleRect.withTrimmedLeft(m_InfoIconRect.getWidth()), Justification::centredLeft);
		}
		else
		{
			//Draw Title
			g.setFont(SAMPLE_TILE_TITLE_FONT);
			g.setColour(titleColor);
			g.drawText(mSample.getFilename(), m_TitleRect.withTrimmedLeft(2.0f), Justification::centredLeft);
		}

		
		g.setColour(titleColor);
		//Draw Time
		g.setFont(16.0f);
		std::stringstream str;
		str << std::fixed << std::setprecision(2) << mSample.getLength();
		g.drawText(String(str.str()), m_TimeRect.withLeft(m_TimeRect.getTopLeft().x + 2.0f), Justification::centred);
		

		//Draw Thumbnail
		g.setColour(foregroundColor);
		std::shared_ptr<SampleAudioThumbnail> thumbnail = mSample.getThumbnail();
		if (thumbnail->isFullyLoaded())
		{
			if (thumbnail->getNumChannels() != 0)
			{
				thumbnail->drawChannel(g, m_ThumbnailRect.toNearestInt(), 0.0, thumbnail->getTotalLength(), 0, 1.0f);
			}
		}

		//Draw Audio Line if playing
		std::shared_ptr<AudioPlayer> auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
		if (auxPlayer->getSampleReference() == mSample)
		{
			float startT = auxPlayer->getStartCueRelative();
			float currentT = auxPlayer->getRelativeTime();
			float startX = m_ThumbnailRect.getTopLeft().x + ((m_ThumbnailRect.getTopRight().x - m_ThumbnailRect.getTopLeft().x) * startT);
			float currentX = m_ThumbnailRect.getTopLeft().x + ((m_ThumbnailRect.getTopRight().x - m_ThumbnailRect.getTopLeft().x) * currentT);
			float y1 = m_ThumbnailRect.getTopLeft().y;
			float y2 = m_ThumbnailRect.getBottomLeft().y;
			g.setColour(Colours::black);
			g.drawLine(startX, y1, startX, y2, 1.0f);
			if (auxPlayer->getState() == AudioPlayer::TransportState::Playing)
			{
				g.setColour(Colours::red);
				g.drawLine(currentX, y1, currentX, y2, 1.0f);
				repaint();
			}
		}
		//set tags
		mTagContainer.setTags(mSample.getTags());

		g.setColour(outlineColor);
		g.drawRoundedRectangle(getLocalBounds().reduced(1).toFloat(), SAMPLE_TILE_CORNER_RADIUS, SAMPLE_TILE_OUTLINE_THICKNESS);
	}
	else
	{
		//reset all info if no sample currently active
		mTagContainer.setTags(StringArray());
	}
}

void SampleTile::resized()
{
	updateRects();
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


void SampleTile::mouseUp(const MouseEvent& e)
{
	if (!mSample.isNull())
	{
		int widthSegment = getWidth() / 4;
		int heightSegment = getHeight() / 3;
		if (m_ThumbnailRect.contains(e.getMouseDownPosition().toFloat()))
		{
			if (e.mods.isLeftButtonDown())
			{
				playSample();
			}
			else if (e.mods.isRightButtonDown())
			{
				float rectWidth = m_ThumbnailRect.getWidth();
				float mouseDownX = e.getMouseDownX();
				playSample(mouseDownX / rectWidth);
			}
			SamplifyProperties::getInstance()->getAudioPlayer()->addChangeListener(this);
		}
		/*
		else if (m_TitleRect.contains(e.getMouseDownPosition().toFloat()) && e.mods.isLeftButtonDown())
		{
			PopupMenu menu;
			StringArray parentDirs = mSample.getRelativeParentFolders();
			for (int i = 0; i < parentDirs.size(); i++)
			{
				menu.addItem(i + 1, parentDirs[i]);
			}
			int choice = menu.show();
		}
		*/

		else if (m_InfoIconRect.contains(e.getPosition().toFloat()))
		{
			//todo make a tooltip not this
			PopupMenu menu;
			menu.addItem(1, mSample.getInfoText(), true, false);
			menu.show();
		}
		else
		{
			PopupMenu menu;
			menu.addItem(1, "Open Q-Editor", false, false); //QEDITOR IS THE PLACE TO BREAK A SAMPLE
			menu.addSeparator();
			menu.addItem(2, "Rename Sample", false, false);
			menu.addItem(3, "Delete Sample", false, false);
			int selection = menu.show();
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

void samplify::SampleTile::mouseExit(const MouseEvent& e)
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
		if (auxPlayer->getSampleReference() != mSample)
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
		if (aux->getSampleReference() == mSample)
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
		bool alreadyThis = false;
		if (!mSample.isNull())
		{
			if (mSample == sample)
			{
				alreadyThis = true;
			}
		}
		if (!alreadyThis)
		{
			sample.generateThumbnailAndCache();
			
		}
	}
	mSample = sample;
	repaint();
}

Sample::Reference SampleTile::getSample()
{
	return mSample;
}


void SampleTile::updateRects()
{
	//Core Rects
	m_TitleRect = Rectangle<float>(0, 0, getWidth(), SAMPLE_TILE_TITLE_FONT.getHeight() + 4.0f);
	m_TypeRect = Rectangle<float>(0, getHeight() - (getWidth() / 5), getWidth() / 5, getWidth() / 5);
	m_TimeRect = Rectangle<float>(getWidth() / 5, getHeight() - (getWidth() / 5), getWidth() / 5, getWidth() / 5);

	//Derivative Rects
	float startY = m_TitleRect.getHeight();
	m_ThumbnailRect = Rectangle<float>(0, startY, getWidth(), getHeight() - (startY + (getWidth() / 5)));

	float offset = (m_TitleRect.getHeight() + m_ThumbnailRect.getHeight());
	m_TagRect = Rectangle<float>(getWidth() / 2, offset, getWidth() / 2, getHeight() - offset);
	mTagContainer.setBounds(m_TagRect.toNearestInt());

	m_InfoIconRect = Rectangle<float>(0, 0, m_TitleRect.getHeight(), m_TitleRect.getHeight()); //square in top right

}

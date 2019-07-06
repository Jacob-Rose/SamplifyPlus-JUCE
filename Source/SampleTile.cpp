#include "SampleTile.h"

#include "SamplifyLookAndFeel.h"
#include "TagTile.h"

#include <iomanip>
#include <sstream>

using namespace samplify;

SampleTile::SampleTile(SampleReference* sample)
{
	setRepaintsOnMouseActivity(true);
	setSize(SAMPLE_TILE_MIN_WIDTH, SAMPLE_TILE_MIN_WIDTH * SAMPLE_TILE_ASPECT_RATIO);
	setSampleReference(sample);
	addAndMakeVisible(mTagContainer);
}

SampleTile::~SampleTile()
{
}
void SampleTile::paint (Graphics& g)
{
	g.setColour(Colours::beige);
	//g.fillRect(getLocalBounds().toFloat());
	if (mSampleReference != nullptr)
	{
		//setup colors to use
		Colour backgroundColor;
		Colour foregroundColor;
		if (isMouseOverOrDragging())
		{
			backgroundColor = getLookAndFeel().findColour(SAMPLETILE_COLOR_ID_BG_HOVER);
			foregroundColor = getLookAndFeel().findColour(SAMPLETILE_COLOR_ID_FG_HOVER);
		}
		else
		{
			backgroundColor = getLookAndFeel().findColour(SAMPLETILE_COLOR_ID_BG_DEFAULT);
			foregroundColor = getLookAndFeel().findColour(SAMPLETILE_COLOR_ID_FG_DEFAULT);
		}
		g.fillAll(backgroundColor);
		g.setColour(foregroundColor);
		// draw an outline around the component
		g.drawRoundedRectangle(getLocalBounds().toFloat(), 12.0f, 2.0f);   
		
		int widthSegment = getWidth() / 4;
		int heightSegment = getHeight() / 3;

		Rectangle<float> sampleTypeBox(0, 0, widthSegment, heightSegment);
		g.setColour(foregroundColor);
		//draw box around sampleType box
		g.drawLine(sampleTypeBox.getBottomLeft().x, 
			sampleTypeBox.getBottomLeft().y, 
			sampleTypeBox.getBottomRight().x, 
			sampleTypeBox.getBottomRight().y, 1.0f);
		g.drawLine(sampleTypeBox.getTopRight().x,
			sampleTypeBox.getTopRight().y,
			sampleTypeBox.getBottomRight().x,
			sampleTypeBox.getBottomRight().y, 1.0f);

		///DRAW SAMPLE TYPE
		switch (mSampleReference->getSampleType())
		{
		case SampleReference::SampleType::ONESHOT:
			FontAwesome::drawCenterd(g, FontAwesome_Share, 30.0f, foregroundColor, sampleTypeBox.toNearestInt());
			break;
		case SampleReference::SampleType::LOOP:
			FontAwesome::drawCenterd(g, FontAwesome_Refresh, 30.0f, foregroundColor, sampleTypeBox.toNearestInt());
			break;
		default:
			break;
		}

		Rectangle<float> sampleKeyBox(0, heightSegment, widthSegment, heightSegment);
		g.setColour(foregroundColor);
		g.drawLine(sampleKeyBox.getTopRight().x,
			sampleKeyBox.getTopRight().y,
			sampleKeyBox.getBottomRight().x,
			sampleKeyBox.getBottomRight().y, 1.0f);
		g.drawLine(sampleKeyBox.getBottomLeft().x,
			sampleKeyBox.getBottomLeft().y,
			sampleKeyBox.getBottomRight().x,
			sampleKeyBox.getBottomRight().y, 1.0f);

		Rectangle<float> sampleTimeBox(0, heightSegment * 2, widthSegment, heightSegment);
		g.setColour(foregroundColor);

		g.drawLine(sampleTimeBox.getTopRight().x,
			sampleTimeBox.getTopRight().y,
			sampleTimeBox.getBottomRight().x,
			sampleTimeBox.getBottomRight().y, 1.0f);
		g.setColour(foregroundColor);
		g.setFont(16.0f);
		std::stringstream str;
		str << std::fixed << std::setprecision(2) << mSampleReference->getLength();
		g.drawText(String(str.str()), sampleTimeBox.withLeft(sampleTimeBox.getTopLeft().x + 2.0f), Justification::centred);


		Rectangle<float> thumbnailBounds(widthSegment, getHeight() / 2, widthSegment * 3, getHeight() / 2);
		SampleAudioThumbnail* mThumbnail = mSampleReference->getAudioThumbnail();
		if (mThumbnail != nullptr && mThumbnail->isFullyLoaded())
		{
			if (mThumbnail->getNumChannels() != 0)
			{
				g.setColour(foregroundColor);
				mThumbnail->drawChannel(g, thumbnailBounds.toNearestInt(), 0.0, mThumbnail->getTotalLength(), 0, 1.0f);
			}
		}

		Rectangle<float> sampleFilenameBox(widthSegment, 0, widthSegment * 3, getHeight() / 2);

		g.setColour(foregroundColor);
		g.setFont((getHeight() / 10) * 1.2f);
		g.drawText(mSampleReference->getFilename(), sampleFilenameBox, Justification::topLeft, true);
		g.setFont((getHeight() / 10) * 0.8f);
		g.drawText(mSampleReference->getFullPathName(), sampleFilenameBox, Justification::bottomLeft, true);
		AudioPlayer* auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
		if (auxPlayer->getFile() == mSampleReference->getFile())
		{
			float startT = auxPlayer->getStartCueRelative();
			float currentT = auxPlayer->getRelativeTime();
			float startX = thumbnailBounds.getTopLeft().x + ((thumbnailBounds.getTopRight().x - thumbnailBounds.getTopLeft().x) * startT);
			float currentX = thumbnailBounds.getTopLeft().x + ((thumbnailBounds.getTopRight().x - thumbnailBounds.getTopLeft().x) * currentT);
			float y1 = thumbnailBounds.getTopLeft().y;
			float y2 = thumbnailBounds.getBottomLeft().y;
			g.setColour(Colours::black);
			g.drawLine(startX, y1, startX, y2, 1.0f);
			if (auxPlayer->getState() == AudioPlayer::TransportState::Playing)
			{
				g.setColour(Colours::red);
				g.drawLine(currentX, y1, currentX, y2, 1.0f);
				repaint();
			}

		}
	}
	else
	{

	}
}

void SampleTile::resized()
{
	int widthSegment = getWidth() / 4;
	int heightSegment = getHeight() / 3;
	mTagContainer.setBounds(widthSegment, (getHeight() / 10) * 1.2f, widthSegment * 3, heightSegment);
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

void SampleTile::mouseDown(const MouseEvent& mouseEvent)
{
	if (mSampleReference != nullptr)
	{
			int widthSegment = getWidth() / 4;
			int heightSegment = getHeight() / 3;
			Rectangle audiowaveRect = Rectangle<float>(widthSegment, getHeight() / 2, widthSegment * 3, getHeight() / 2);
			if (audiowaveRect.contains(mouseEvent.getMouseDownPosition().toFloat()))
			{
				if (mouseEvent.mods.isLeftButtonDown())
				{
					playSample();
				}
				else if (mouseEvent.mods.isRightButtonDown())
				{
					float rectWidth = audiowaveRect.getWidth();
					float mouseDownX = mouseEvent.getMouseDownX() - widthSegment;
					playSample(mouseDownX / rectWidth );
				}
				SamplifyProperties::getInstance()->getAudioPlayer()->addChangeListener(this);

			}
			else
			{
				if (mouseEvent.mods.isCtrlDown())
				{
					StringArray files = StringArray();
					files.add(mSampleReference->getFile().getFullPathName());
					DragAndDropContainer::performExternalDragDropOfFiles(files, false);
				}
				
			}
	}
}

void SampleTile::mouseUp(const MouseEvent& e)
{
}

void SampleTile::mouseMove(const MouseEvent & e)
{

}

void SampleTile::playSample()
{
	playSample(0.0f);
}

void SampleTile::playSample(float t)
{
	AudioPlayer* auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
	if (auxPlayer->getFile() != mSampleReference->getFile())
	{
		auxPlayer->loadFile(mSampleReference);
	}
	else
	{
		auxPlayer->stop();
		auxPlayer->reset();
	}
	auxPlayer->setRelativeTime(t);
	auxPlayer->play();
}

void SampleTile::itemDropped(const SourceDetails & dragSourceDetails)
{
	TagTile* tagComp = (TagTile*)(dragSourceDetails.sourceComponent.get());
	mSampleReference->addTag(tagComp->getTag());
	mTagContainer.setTags(mSampleReference->getTags());
}

void SampleTile::changeListenerCallback(ChangeBroadcaster* source)
{
  	AudioPlayer* aux = SamplifyProperties::getInstance()->getAudioPlayer();
	if (aux->getFile() == mSampleReference->getFile())
	{
		if (!(aux->getState() == AudioPlayer::TransportState::Starting ||
			aux->getState() == AudioPlayer::TransportState::Playing))
		{
			aux->removeChangeListener(this);
		}

		repaint();
	}
}

void SampleTile::setSampleReference(SampleReference * sample)
{
	if (sample != nullptr)
	{
		if (sample->getAudioThumbnail() == nullptr)
		{
			sample->generateThumbnailAndCache();
		}
	}
	mSampleReference = sample;
	repaint();
}

SampleReference* SampleTile::getSampleReference()
{
	return mSampleReference;
}

bool SampleTile::operator==(SampleReference * ref)
{
	return ref == mSampleReference;
}

#include "SampleTile.h"

#include "SamplifyLookAndFeel.h"
#include "TagTile.h"

#include <iomanip>
#include <sstream>

using namespace samplify;

SampleTile::SampleTile(Sample::SampleReference sample)  : mSampleReference(sample)
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
	Rectangle<float> titleRect = getTitleRect();
	Rectangle<float> timeRect = getTimeRect();
	Rectangle<float> typeRect = getTypeRect();
	Rectangle<float> thumbnailBounds = getThumbnailRect();
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
	g.fillAll(backgroundColor);
	//g.fillRoundedRectangle(getBounds().toFloat(), 4.0f);
	g.setFont(SAMPLE_TILE_TITLE_FONT);
	g.setColour(Colours::black);
	// draw an outline around the component
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 4.0f, 2.0f);   
	g.drawLine(thumbnailBounds.getTopLeft().x, 
		thumbnailBounds.getTopLeft().y, 
		thumbnailBounds.getTopRight().x, 
		thumbnailBounds.getTopRight().y, 
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
	g.setColour(Colours::darkslategrey);
	g.drawText(mSampleReference.getFilename(), titleRect, Justification::centredLeft);
	g.setColour(foregroundColor);
	//DRAW SAMPLE TYPE
	switch (mSampleReference.getSampleType())
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
	str << std::fixed << std::setprecision(2) << mSampleReference.getLength();
	g.drawText(String(str.str()), timeRect.withLeft(timeRect.getTopLeft().x + 2.0f), Justification::centred);

	SampleAudioThumbnail* mThumbnail = mSampleReference.getAudioThumbnail();
	if (mThumbnail != nullptr && mThumbnail->isFullyLoaded())
	{
		if (mThumbnail->getNumChannels() != 0)
		{
			g.setColour(foregroundColor);
			mThumbnail->drawChannel(g, thumbnailBounds.toNearestInt(), 0.0, mThumbnail->getTotalLength(), 0, 1.0f);
		}
	}
	AudioPlayer* auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
	if (auxPlayer->getFile() == mSampleReference.getFile())
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

}

void SampleTile::mouseUp(const MouseEvent& mouseEvent)
{
	int widthSegment = getWidth() / 4;
	int heightSegment = getHeight() / 3;
	Rectangle audiowaveRect = getThumbnailRect();
	if (audiowaveRect.contains(mouseEvent.getMouseDownPosition().toFloat()))
	{
		if (mouseEvent.mods.isLeftButtonDown())
		{
			playSample();
		}
		else if (mouseEvent.mods.isRightButtonDown())
		{
			float rectWidth = audiowaveRect.getWidth();
			float mouseDownX = mouseEvent.getMouseDownX();
			playSample(mouseDownX / rectWidth);
		}
		SamplifyProperties::getInstance()->getAudioPlayer()->addChangeListener(this);

	}
}

void samplify::SampleTile::mouseDrag(const MouseEvent& e)
{
	StringArray files = StringArray();
	files.add(mSampleReference.getFile().getFullPathName());
	DragAndDropContainer::performExternalDragDropOfFiles(files, false);
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
	AudioPlayer* auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
	if (auxPlayer->getFile() != mSampleReference.getFile())
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
	mSampleReference.addTag(tagComp->getTag());
	mTagContainer.setTags(mSampleReference.getTags());
}

void SampleTile::changeListenerCallback(ChangeBroadcaster* source)
{
  	AudioPlayer* aux = SamplifyProperties::getInstance()->getAudioPlayer();
	if (aux->getFile() == mSampleReference.getFile())
	{
		if (!(aux->getState() == AudioPlayer::TransportState::Starting ||
			aux->getState() == AudioPlayer::TransportState::Playing))
		{
			aux->removeChangeListener(this);
		}

		repaint();
	}
}

void SampleTile::setSampleReference(Sample::SampleReference sample)
{
	if (sample.getAudioThumbnail() == nullptr)
	{
		sample.generateThumbnailAndCache();
	}
	mSampleReference = sample;
	repaint();
}

Rectangle<float> SampleTile::getTitleRect()
{
	return Rectangle<float>(0,0,getWidth(), SAMPLE_TILE_TITLE_FONT.getHeight() + 2.0f);
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

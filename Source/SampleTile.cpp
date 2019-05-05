/*
  ==============================================================================

    SampleTile.cpp
    Created: 31 May 2018 1:20:01pm
    Author:  jacob

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleTile.h"
#include <iomanip>
#include <sstream>

//==============================================================================
SampleTile::SampleTile(SampleReference* sample)
{
	setRepaintsOnMouseActivity(true);
	setSize(SAMPVIEW_WIDTH, SAMPVIEW_HEIGHT);
	setSampleReference(sample);
}

SampleTile::~SampleTile()
{
}

void SampleTile::paint (Graphics& g)
{
	if (mSampleReference != nullptr)
	{
		Colour backgroundColor;
		Colour foregroundColor;
		if (isMouseOverOrDragging() && !isMouseOverButton)
		{
			backgroundColor = C_BACKGROUND_HOVER;
			foregroundColor = C_FOREGROUND_HOVER;
		}
		else
		{
			backgroundColor = C_BACKGROUND_DEFAULT;
			foregroundColor = C_FOREGROUND_DEFAULT;
		}
		g.fillAll(backgroundColor);
		g.setColour(foregroundColor);
		// draw an outline around the component
		g.drawRect(getLocalBounds(), 1);   
		
		int widthSegment = getWidth() / 4;
		int heightSegment = getHeight() / 3;

		Rectangle<int> sampleTypeBox(0, 0, widthSegment, heightSegment);
		g.setColour(foregroundColor);
		//draw box around sampleType box
		g.drawRect(sampleTypeBox, 1);

		///DRAW SAMPLE TYPE
		switch (mSampleReference->getSampleType())
		{
		case SampleType::ONESHOT:
			FontAwesome::drawCenterd(g, FontAwesome_Share, 30.0f, foregroundColor, sampleTypeBox);
			break;
		case SampleType::LOOP:
			FontAwesome::drawCenterd(g, FontAwesome_Refresh, 30.0f, foregroundColor, sampleTypeBox);
			break;
		default:
			break;
		}

		Rectangle<int> sampleKeyBox(0, heightSegment, widthSegment, heightSegment);
		g.setColour(foregroundColor);
		g.drawRect(sampleKeyBox, 1);

		Rectangle<int> sampleTimeBox(0, heightSegment * 2, widthSegment, heightSegment);
		g.setColour(foregroundColor);
		g.drawRect(sampleTimeBox, 1);
		g.setColour(foregroundColor);
		g.setFont(16.0f);
		std::stringstream str;
		str << std::fixed << std::setprecision(2) << mSampleReference->getSampleLength();
		g.drawText(String(str.str()), sampleTimeBox, Justification::centred);


		Rectangle<int> thumbnailBounds(widthSegment, getHeight() / 2, widthSegment * 3, getHeight() / 2);
		SampleAudioThumbnail* mThumbnail = mSampleReference->getAudioThumbnail();
		if (mThumbnail != nullptr && mThumbnail != NULL && mThumbnail->isFullyLoaded())
		{
			if (mThumbnail->getNumChannels() != 0)
			{
				g.setColour(foregroundColor);
				mThumbnail->drawChannel(g, thumbnailBounds, 0.0, mThumbnail->getTotalLength(), 0, 1.0f);
			}
		}

		if (isMouseOverButton) { g.setColour(C_BACKGROUND_HOVER); }
		else { g.setColour(C_BACKGROUND_DEFAULT); }

		g.fillRect(buttonBounds);
		g.setColour(foregroundColor);
		g.drawRect(buttonBounds);
		FontAwesome::drawCenterd(g, FontAwesome_Play, 12.0f, foregroundColor, buttonBounds);

		Rectangle<int> sampleFilenameBox(widthSegment, 0, widthSegment * 3, getHeight() / 2);

		g.setColour(foregroundColor);
		g.setFont((getHeight() / 10) * 1.2f);
		g.drawText(mSampleReference->getFilename(), sampleFilenameBox, Justification::topLeft, true);
		g.setFont((getHeight() / 10) * 0.8f);
		g.drawText(mSampleReference->getFullPathName(), sampleFilenameBox, Justification::bottomLeft, true);


		float totalShifted = 0.0f;
		StringArray* tags = mSampleReference->getSampleTags();
		for (int i = 0; i < tags->size(); i++)
		{
			Rectangle<float> area = Rectangle<float>(0 + totalShifted, heightSegment, totalShifted, heightSegment / 2);
			g.drawRoundedRectangle(area, 0.5f, 1.0f);
			g.drawText(tags->getReference(i), area, juce::Justification::centred);
		}
		
	}
	else
	{
		//g.fillAll(Colours::aqua);
	}
}

void SampleTile::resized()
{
	buttonBounds = Rectangle<int>(getWidth() * 0.875, getHeight() * 0.68, getWidth() * 0.1, getWidth() * 0.1);
}

void SampleTile::mouseDown(const MouseEvent & e)
{
	if (mSampleReference != nullptr)
	{
		if (isMouseOverButton)
		{
			playSample();
		}
		else
		{
			StringArray files = StringArray();
			files.add(mSampleReference->getFile().getFullPathName());
			DragAndDropContainer::performExternalDragDropOfFiles(files, false);
		}
		
	}
}

void SampleTile::mouseMove(const MouseEvent & e)
{

	if (isMouseWithinPlayButton(e))
	{
		if (!isMouseOverButton)
		{
			isMouseOverButton = true;
			repaint();
		}
	}
	else
	{
		if (isMouseOverButton)
		{
			isMouseOverButton = false;
			repaint();
		}
	}
}

void SampleTile::playSample()
{
	
}

bool SampleTile::isMouseWithinPlayButton(const MouseEvent & e)
{
	if (buttonBounds.contains(e.getPosition()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SampleTile::isInterestedInDragSource(const SourceDetails & dragSourceDetails)
{
	return false;
}

void SampleTile::itemDropped(const SourceDetails & dragSourceDetails)
{
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

SampleReference * SampleTile::getSampleReference()
{
	return mSampleReference;
}

bool SampleTile::operator==(SampleReference * ref)
{
	return ref == mSampleReference;
}

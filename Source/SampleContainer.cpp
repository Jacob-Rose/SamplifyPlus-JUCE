/*
  ==============================================================================

    SampleContainer.cpp
    Created: 31 May 2018 1:20:21pm
    Author:  Jacob Rose

  ==============================================================================
*/

#include "SampleContainer.h"
#include "SampleLibrary.h"
#include "SamplifyProperties.h"
#include <algorithm>
#include <cmath>

SampleContainer::SampleContainer()
{
	mFlexBox.alignContent = FlexBox::AlignContent::flexStart;
	mFlexBox.flexDirection = FlexBox::Direction::row;
	mFlexBox.justifyContent = FlexBox::JustifyContent::flexStart;
	mFlexBox.alignItems = FlexBox::AlignItems::flexStart;
	mFlexBox.flexWrap = FlexBox::Wrap::wrap;
	SamplifyProperties::getInstance()->getSampleLibrary()->addChangeListener(this);
}

SampleContainer::~SampleContainer()
{
	//delete sampviews
	//delete flex items?
}

void SampleContainer::paint (Graphics& g)
{
	
}

void SampleContainer::resized()
{
	refreshItemsBounds();
}

Rectangle<int> SampleContainer::getViewportBounds()
{
	return mViewport.getBounds();
}


void SampleContainer::changeListenerCallback(ChangeBroadcaster * source)
{
	refreshItems();
}

void SampleContainer::refreshItems()
{
	clearItems();
	for (int i = 0; i < mCurrentSampleReferences.size() && i < MAX_LOADED_SAMPLES; i++)
	{
		if (mFreeSampleItems.size() < 1)
		{
			createFreeSampleTile();
		}
		
		mFlexBox.items.add(createFlexItem());
		((SampleTile*)mFlexBox.items.getLast().associatedComponent)->setSampleReference(mCurrentSampleReferences[i]);
		addAndMakeVisible(mFlexBox.items.getLast().associatedComponent);
	}
	refreshItemsBounds();
}

void SampleContainer::refreshItemsBounds()
{
	Rectangle<int> bounds = getBounds();
	setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), calculateAllRowsHeight());
	float boxWidth = 1000000;
	int c = 1;
	while (boxWidth > SAMPVIEW_MAX_WIDTH)
	{
		boxWidth = getBounds().getWidth() / c;
		c++;
	}
	float boxHeight = boxWidth * SAMPVIEW_ASPECTRATIO;
	for (int i = 0; i < mFlexBox.items.size(); i++)
	{
		mFlexBox.items.getReference(i).associatedComponent->setSize(boxWidth, boxHeight);
	}
	mFlexBox.performLayout(getLocalBounds());
}

void SampleContainer::setSampleItems(std::vector<SampleReference*> currentSamples)
{
	mCurrentSampleReferences = currentSamples;
	refreshItems();
}

int SampleContainer::calculateAllRowsHeight()
{
	int columns = roundFloatToInt(getLocalBounds().getWidth() / mFlexBox.items[0].width);
	float height = mFlexBox.items[0].height;
	return height * calculateRowCount();
}

int SampleContainer::calculateRowCount()
{
	int columns = calculateColumnCount();
	if (columns > 0)
	{
		return SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples()->size() / columns;
	}
	return 0;
}

int SampleContainer::calculateColumnCount()
{
	return roundFloatToInt(getLocalBounds().getWidth() / mFlexBox.items[0].width);
}

float SampleContainer::calculateBoxWidth()
{
	return getLocalBounds().getWidth() / calculateColumnCount();
}

float SampleContainer::calculateBoxHeight()
{
	return calculateBoxWidth() * SAMPVIEW_ASPECTRATIO;
}

void SampleContainer::clearItems()
{
	for (int i = 0; i < mFlexBox.items.size(); i++)
	{
		((SampleTile*)mFlexBox.items[i].associatedComponent)->setSampleReference(nullptr);
		mFreeSampleItems.push_back((SampleTile*)mFlexBox.items[i].associatedComponent);
	}
	mFlexBox.items.clear();
}

void SampleContainer::createFreeSampleTile()
{
	SampleTile* tile = new SampleTile(nullptr);
	mFreeSampleItems.push_back(tile);
}

FlexItem SampleContainer::createFlexItem()
{
	FlexItem item = FlexItem().withMinWidth(SAMPVIEW_MIN_WIDTH)
		.withMaxWidth(SAMPVIEW_MAX_WIDTH)
		.withMaxHeight(SAMPVIEW_MAX_WIDTH * SAMPVIEW_ASPECTRATIO)
		.withMinHeight(SAMPVIEW_MIN_WIDTH * SAMPVIEW_ASPECTRATIO);
	item.associatedComponent = mFreeSampleItems.at(0);
	mFreeSampleItems.erase(mFreeSampleItems.begin());
	return item;
}

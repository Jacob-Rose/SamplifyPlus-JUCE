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
	initializeItems();
	sampleListUpdated();
	SamplifyProperties::getInstance()->getSampleLibrary()->addChangeListener(this);
}

SampleContainer::~SampleContainer()
{
	deleteSampleViews();
}

void SampleContainer::paint (Graphics& g)
{
	
}

void SampleContainer::resized()
{
	refreshBounds();
}

Rectangle<int> SampleContainer::getViewportBounds()
{
	return mViewport.getBounds();
}

void SampleContainer::sampleListUpdated()
{
	refreshItems();
	//refreshBounds();
}


void SampleContainer::initializeItems()
{
	for (int i = 0; i < MAX_LOADED_SAMPLES; i++)
	{
		FlexItem item = FlexItem();
		item.minHeight = SAMPVIEW_MIN_WIDTH;
		item.maxHeight = SAMPVIEW_MAX_WIDTH;
		mAllFlexItems.push_back(item);
		SampleTile* tile = new SampleTile(nullptr);
		item.associatedComponent = tile;
		//FlexItem* flexItem = &mFlexBox.items.getReference(mFlexBox.items.size() - 1);
	}
	refreshItems();
	refreshBounds();
}

void SampleContainer::changeListenerCallback(ChangeBroadcaster * source)
{
	refreshItems();
}

void SampleContainer::refreshItems()
{
	for (int i = 0; i < mFlexBox.items.size(); i++)
	{
		mFlexBox.items.getReference(i).associatedComponent = nullptr;
		removeChildComponent(mFlexBox.items[i].associatedComponent);
	}

	std::vector<SampleReference*>* sampleList = SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples();
	//check if already been loaded
	for (int i = 0; i < sampleList->size() && i < MAX_LOADED_SAMPLES; i++)
	{
		SampleTile* toLoad = nullptr;
		int j = 0;
		mFlexBox.items.getReference(i).associatedComponent = toLoad;
		addAndMakeVisible(toLoad);
	}
	refreshBounds();
}

void SampleContainer::refreshBounds()
{
	Rectangle<int> bounds = getBounds();
	setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), calculateHeight());
	mFlexBox.performLayout(getLocalBounds());
}

int SampleContainer::calculateHeight()
{
	return mFlexBox.items);
}

int SampleContainer::calculateRows()
{
	int elementsPerRow = (getWidth() / (SAMPVIEW_WIDTH + (2 * SAMPVIEW_MARGIN)));
	if (elementsPerRow > 0)
	{
		return SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples()->size() / elementsPerRow;
	}
	return 0;
}

void SampleContainer::deleteSampleViews()
{
	for (int i = 0; i < mLoadedSampleTiles.size(); i++)
	{
		delete mLoadedSampleTiles[i];
	}
}

/*
  ==============================================================================

    SampleContainer.cpp
    Created: 31 May 2018 1:20:21pm
    Author:  Jacob Rose

  ==============================================================================
*/

#include "SampleContainer.h"
#include "SampleLibrary.h"
#include "AppProperties.h"
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
	AppProperties::getInstance()->getSampleLibrary()->addChangeListener(this);
}

SampleContainer::~SampleContainer()
{
	
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
	refreshBounds();
}
//! this is important
//? what dod 


void SampleContainer::initializeItems()
{
	for (int i = 0; i < MAX_LOADED_SAMPLES; i++)
	{
		mFlexBox.items.add(FlexItem().withMinWidth(SAMPVIEW_WIDTH).withMinHeight(SAMPVIEW_HEIGHT).
			withMaxWidth(SAMPVIEW_WIDTH + 200).withMaxHeight(SAMPVIEW_HEIGHT + 200).
			withMargin(SAMPVIEW_MARGIN));
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
	std::vector<SampleReference*>* sampleList = AppProperties::getInstance()->getSampleLibrary()->getCurrentSamples();
	SampleTile* toLoad = nullptr;
	for (int i = 0; i < sampleList->size() && i < MAX_LOADED_SAMPLES; i++)
	{
		int j = 0;
		while (toLoad == nullptr && j < mLoadedSampleTiles.size())
		{
			if (mLoadedSampleTiles.at(j)->getSampleReference() == sampleList->at(i))
			{
				toLoad = mLoadedSampleTiles.at(j);
			}
			j++;
		}
		if (toLoad == nullptr)
		{
			mLoadedSampleTiles.push_back(new SampleTile(sampleList->at(i)));
		}
	}

	for (int i = 0; i < sampleList->size() && i < MAX_LOADED_SAMPLES; i++)
	{
		SampleTile* toLoad = nullptr;
		int j = 0;
		while (toLoad == nullptr && j < mLoadedSampleTiles.size())
		{
			if (mLoadedSampleTiles.at(j)->getSampleReference() == sampleList->at(i))
			{
				toLoad = mLoadedSampleTiles.at(j);
			}
			j++;
		}
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
	return (int)((SAMPVIEW_HEIGHT + (SAMPVIEW_MARGIN * 2)) * calculateRows());
}

int SampleContainer::calculateRows()
{
	int elementsPerRow = (getWidth() / (SAMPVIEW_WIDTH + (2 * SAMPVIEW_MARGIN)));
	if (elementsPerRow > 0)
	{
		return AppProperties::getInstance()->getSampleLibrary()->getCurrentSamples()->size() / elementsPerRow;
	}
	return 0;
}

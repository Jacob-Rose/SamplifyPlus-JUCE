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


SampleContainer::SampleContainer()
{
	mFlexBox.alignContent = FlexBox::AlignContent::flexStart;
	mFlexBox.flexDirection = FlexBox::Direction::row;
	mFlexBox.justifyContent = FlexBox::JustifyContent::flexStart;
	mFlexBox.alignItems = FlexBox::AlignItems::flexStart;
	mFlexBox.flexWrap = FlexBox::Wrap::wrap;
	initializeItems();
	sampleListUpdated();
	AppProperties::getSampleLibrary()->addChangeListener(this);
}

SampleContainer::~SampleContainer()
{
	for (int i = 0; i < mActiveSampleTiles.size(); i++)
	{
		delete mActiveSampleTiles[i];
	}
	mActiveSampleTiles.clear();
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

void SampleContainer::addFlexItem(SampleReference * sample)
{
	mFlexBox.items.add(FlexItem().withMinWidth(SAMPVIEW_WIDTH).withMinHeight(SAMPVIEW_HEIGHT).
		withMaxWidth(SAMPVIEW_WIDTH+200).withMaxHeight(SAMPVIEW_HEIGHT+200).
		withMargin(SAMPVIEW_MARGIN));
	FlexItem* flexItem = &mFlexBox.items.getReference(mFlexBox.items.size() - 1);

	SampleTile* sampleTile = new SampleTile(sample);
	mActiveSampleTiles.push_back(sampleTile);
	flexItem->associatedComponent = sampleTile;
	addAndMakeVisible(sampleTile);
}

void SampleContainer::initializeItems()
{
	for (int i = 0; i < MAX_LOADED_SAMPLES; i++)
	{
		addFlexItem(nullptr);
	}
}

void SampleContainer::changeListenerCallback(ChangeBroadcaster * source)
{
	refreshItems();
}

void SampleContainer::refreshItems()
{
	for (int i = 0; i < mFlexBox.items.size(); i++)
	{
		FlexItem* flexItem = &mFlexBox.items.getReference(i);
		SampleTile* flexItemComponent = (SampleTile*)(flexItem->associatedComponent);
		std::vector<SampleReference*>* sampleList = AppProperties::getSampleLibrary()->getCurrentSamples();
		if (i < sampleList->size())
		{
			flexItemComponent->setSampleReference((*sampleList)[i]);
			//flexItemComponent->setSampleReference(nullptr);
		}
		else
		{
			flexItemComponent->setSampleReference(nullptr);
		}
	}
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
		return mActiveSampleTiles.size() / elementsPerRow;
	}
	return 0;
}
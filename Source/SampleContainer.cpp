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
	/*
	using Track = Grid::TrackInfo;
	mGrid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	mGrid.templateRows = { Track(1_fr), Track(1_fr), Track(1_fr) };
	mGrid.autoColumns = Track(1_fr);
	mGrid.autoRows = Track(1_fr);
	mGrid.autoFlow = Grid::AutoFlow::row;
	*/
}

SampleContainer::~SampleContainer()
{
	//delete sampviews
	for (int i = 0; i < mUsedSampleTiles.size(); i++)
	{
		delete mUsedSampleTiles[i];
		mUsedSampleTiles[i] = nullptr;
	}
	mUsedSampleTiles.clear();
}

void SampleContainer::paint (Graphics& g)
{
	
}

void SampleContainer::resized()
{
	updateItems();
}

void SampleContainer::updateItems()
{
	int columns = calculateColumnCount();
	if (columns > 0)
	{
		for (unsigned int i = 0;i < mCurrentSampleReferences.size() && i < 12; i++)
		{
			if (i < mUsedSampleTiles.size())
			{
				mUsedSampleTiles[i]->setSampleReference(mCurrentSampleReferences[i]);
			}
			else
			{
				SampleTile* tile = new SampleTile(mCurrentSampleReferences[i]);
				mUsedSampleTiles.push_back(tile);

				int column = i % columns;
				int row = i / columns; //will cut off, not round (i feel like a real coder)

				tile->setSampleReference(mCurrentSampleReferences[i]);

				addAndMakeVisible(tile);
				tile->setBounds(column * SAMPVIEW_WIDTH, row * SAMPVIEW_HEIGHT, SAMPVIEW_WIDTH, SAMPVIEW_HEIGHT);
			}
			
		}
	}
	setBounds(Rectangle<int>(0, 0, calculateColumnCount() * SAMPVIEW_WIDTH, calculateRowCount() * SAMPVIEW_HEIGHT));
	//repaint();
}

void SampleContainer::clearItems()
{
	for (unsigned int i = 0; i < mUsedSampleTiles.size(); i++)
	{
		delete mUsedSampleTiles.at(i);
		mUsedSampleTiles.at(i) = nullptr;
	}
	mUsedSampleTiles.clear();
}

void SampleContainer::setSampleItems(std::vector<SampleReference*> currentSamples)
{
	mCurrentSampleReferences = currentSamples;
	updateItems();
}

int SampleContainer::calculateAllRowsHeight()
{
	int columns = calculateColumnCount();
	float height;
	if (mUsedSampleTiles.size() > 0)
	{
		height = mUsedSampleTiles[0]->getHeight();
	}
	else
	{
		height = 0;
	}
	return height * calculateRowCount();
}

int SampleContainer::calculateRowCount()
{
	int columns = calculateColumnCount();
	if (columns > 0)
	{
		return mUsedSampleTiles.size() / columns;
	}
	return 0;
}

int SampleContainer::calculateColumnCount()
{
	return getWidth() / SAMPVIEW_WIDTH;
}

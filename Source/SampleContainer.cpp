#include "SampleContainer.h"
#include "SampleLibrary.h"
#include "SamplifyProperties.h"
#include "SamplifyLookAndFeel.h"

#include <cmath>

using namespace samplify;

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
		int width = getWidth() / columns;
		int height = SAMPLE_TILE_ASPECT_RATIO * width;
		if (columns > 0)
		{
			for (unsigned int i = 0; i < mCurrentSampleReferences.size() && i < mMaxItems; i++)
			{
				int column = i % columns;
				int row = i / columns; //will cut off, not round (i feel like a real coder)

				SampleTile* tile;
				if (i < mUsedSampleTiles.size())
				{
					tile = mUsedSampleTiles[i];
				}
				else
				{
					tile = new SampleTile(mCurrentSampleReferences[i]);
					mUsedSampleTiles.push_back(tile);
					addAndMakeVisible(tile);
				}
				tile->setBounds((column * width) + SAMPLE_TILE_CONTAINER_ITEM_PADDING, 
								(row * height) + SAMPLE_TILE_CONTAINER_ITEM_PADDING,
								width - (SAMPLE_TILE_CONTAINER_ITEM_PADDING * 2),
								height - (SAMPLE_TILE_CONTAINER_ITEM_PADDING * 2));
				tile->setSampleReference(mCurrentSampleReferences[i]);

			}
		}
		setBounds(Rectangle<int>(0, 0, calculateColumnCount() * width, calculateRowCount() * height));
	}


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

void SampleContainer::setSampleItems(std::vector<Sample::SampleReference> currentSamples)
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
	return getWidth() / SAMPLE_TILE_MIN_WIDTH;
}

void SampleContainer::extendItems()
{
	//todo put on new thread

	if (mCurrentSampleReferences.size() > mMaxItems)
	{
		mMaxItems += calculateColumnCount() * 3;
		updateItems();
	}
	
}

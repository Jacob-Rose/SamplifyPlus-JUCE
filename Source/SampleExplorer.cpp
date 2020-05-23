#include "SampleExplorer.h"
#include "SampleLibrary.h"
#include "SamplifyProperties.h"
#include "SamplifyMainComponent.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

SampleExplorer::SampleExplorer() : mViewport(&mSampleContainer)
{
    addAndMakeVisible(mViewport);
	addAndMakeVisible(mFilter);
	mFilter.addItem("A-Z", 1);
	mFilter.addItem("Z-A", 2);
	mFilter.addItem("Newest", 3);
	mFilter.addItem("Oldest", 4);
	mFilter.addItem("Random", 5);
	mViewport.addAndMakeVisible(mSampleContainer);
	addAndMakeVisible(mSearchBar);
	mViewport.setViewedComponent(&mSampleContainer);
	mViewport.setScrollBarsShown(true, false, true, false);
	mSearchBar.addListener(this);
}

SampleExplorer::~SampleExplorer()
{
}

void SampleExplorer::paint (Graphics& g)
{
	if (mIsUpdating)
	{
		float size = getWidth() / 5;
		getLookAndFeel().drawSpinningWaitAnimation(g, getLookAndFeel().findColour(SAMPLE_TILE_FG_DEFAULT_COLOR_ID), (getWidth() / 2) - (size / 2), size, size, size);
		repaint();
	}
}

void SampleExplorer::resized()
{
    mSearchBar.setBounds(0, 0, getWidth() - 120, 30);
	mFilter.setBounds(getWidth() - 120, 0, 120, 30);
	mViewport.setBounds(0,30,getWidth(), getHeight()-30);
	mSampleContainer.setBounds(mViewport.getBounds().withRight(mViewport.getWidth() - mViewport.getScrollBarThickness()));
}

void SampleExplorer::textEditorTextChanged(TextEditor& e)
{
	SamplifyProperties::getInstance()->getSampleLibrary()->updateCurrentSamples(e.getText());
}

void SampleExplorer::changeListenerCallback(ChangeBroadcaster* source)
{
	if (SampleLibrary* sl = dynamic_cast<SampleLibrary*>(source))
	{
		if (sl->isAsyncValid())
		{
			mSampleContainer.setSampleItems(Sample::List()); //set to empty
			mIsUpdating = true;
			repaint();
		}
		else
		{
			mIsUpdating = false;
			mSampleContainer.setSampleItems(sl->getCurrentSamples());
		}
	}
}

SampleExplorer::SampleViewport::SampleViewport(SampleContainer* container)
{
	mSampleContainer = container;
}

void SampleExplorer::SampleViewport::visibleAreaChanged(const Rectangle<int>& newVisibleArea)
{
	int height = mSampleContainer->getHeight();
	int bottomBounds = newVisibleArea.getBottomLeft().y;
	int topBounds = newVisibleArea.getTopLeft().y;
	if (height == bottomBounds)
	{
		mSampleContainer->extendItems();
		
		setViewPosition(0, topBounds);
	}
}

SampleExplorer::SampleSearchbar::SampleSearchbar()
{
	addAndMakeVisible(mEraseSearchButton);
	mEraseSearchButton.setButtonText("Clear");
	mEraseSearchButton.addListener(this);
}

void SampleExplorer::SampleSearchbar::resized()
{
	mEraseSearchButton.setBoundsRelative(0.8f, 0.2f, 0.1f, 0.6f);
	TextEditor::resized();
}

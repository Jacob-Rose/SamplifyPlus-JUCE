#include "SampleExplorer.h"
#include "SampleLibrary.h"
#include "SamplifyProperties.h"

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
	mFilter.addListener(this);
	mSampleContainer.setSampleItems(SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples());
	SamplifyProperties::getInstance()->getSampleLibrary()->addChangeListener(this);
}

SampleExplorer::~SampleExplorer()
{
}

void SampleExplorer::paint (Graphics& g)
{
    
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

void SampleExplorer::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged->getSelectedId() != 5)
	{
		SortingMethod method;
		switch (comboBoxThatHasChanged->getSelectedId())
		{
		case 1:
			method = SortingMethod::FirstToLast;
			break;
		case 2:
			method = SortingMethod::LastToFirst;
			break;
		case 3:
			method = SortingMethod::Newest;
			break;
		case 4:
			method = SortingMethod::Oldest;
			break;
		}
		//SamplifyProperties::getInstance()->getSampleLibrary()->sortCurrentSamples(method);
		
	}
	else
	{

	}
}

void SampleExplorer::changeListenerCallback(ChangeBroadcaster* source)
{
	mSampleContainer.setSampleItems(SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples());
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

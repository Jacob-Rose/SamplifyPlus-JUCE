#include "SampleExplorer.h"
#include "SampleLibrary.h"
#include "SamplifyProperties.h"

using namespace samplify;

SampleExplorer::SampleExplorer()
{
    addAndMakeVisible(mViewport);
	addAndMakeVisible(mSearchBar);
	mViewport.setViewedComponent(&mSampleContainer);
	mViewport.setScrollBarsShown(true, false, true, false);
	mSearchBar.addListener(this);
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
    mSearchBar.setBounds(0, 0, getWidth(), 30);
	mViewport.setBounds(0,30,getWidth(), getHeight()-30);
	mSampleContainer.setBounds(mViewport.getBounds());
}

void SampleExplorer::textEditorTextChanged(TextEditor& e)
{
	SamplifyProperties::getInstance()->getSampleLibrary()->updateCurrentSamples(e.getText());
}

void SampleExplorer::changeListenerCallback(ChangeBroadcaster* source)
{
	mSampleContainer.setSampleItems(SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples());
}

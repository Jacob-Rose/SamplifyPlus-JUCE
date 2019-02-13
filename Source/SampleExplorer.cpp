/*
  ==============================================================================

    SampleExplorer.cpp
    Created: 31 May 2018 12:43:08pm
    Author:  jacob

  ==============================================================================
*/

#include "SampleExplorer.h"
#include "SampleLibrary.h"
#include "AppProperties.h"
//==============================================================================
SampleExplorer::SampleExplorer()
{
    addAndMakeVisible(mViewport);
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
    
}

void SampleExplorer::resized()
{
    mSearchBar.setBounds(0, 0, getWidth(), 30);
	mViewport.setBounds(0,30,getWidth(), getHeight()-30);
	mSampleContainer.setBounds(mViewport.getBounds());
	mSampleContainer.refreshBounds();
}

void SampleExplorer::textEditorTextChanged(TextEditor& e)
{
	AppProperties::getSampleLibrary()->updateCurrentSamples(e.getText());
}

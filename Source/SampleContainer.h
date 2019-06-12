/*
  ==============================================================================

    SampleContainer.h
    Created: 31 May 2018 1:20:21pm
    Author:  jacob

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleReference.h"
#include "SampleTile.h"

class SampleContainer    : public Component, public ChangeListener
{
public:
    SampleContainer();
    ~SampleContainer();

    void paint (Graphics&) override;
    void resized() override;

	Rectangle<int> getViewportBounds();

	void sampleListUpdated();
	
	void initializeItems();

	void changeListenerCallback(ChangeBroadcaster* source);

	void refreshItems();

	void refreshBounds();

	int calculateHeight();

	int calculateRows();

	void deleteSampleViews();

private:
	std::vector<SampleTile*> mLoadedSampleTiles;
	FlexBox mFlexBox;
	Viewport mViewport;
	static const int MAX_LOADED_SAMPLES = 250;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleContainer)
};

/*
  ==============================================================================

    SampleContainer.h
    Created: 31 May 2018 1:20:21pm
    Author:  Jacob Rose
	Contains SampleTiles and properly adjust depending on input search terms

  ==============================================================================
*/
#ifndef SAMPLECONTAINER_H
#define SAMPLECONTAINER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleReference.h"
#include "SampleTile.h"

class SampleContainer    : public Component, public ChangeListener
{
public:
	//========================================================
    SampleContainer();
    ~SampleContainer();

    void paint (Graphics&) override;
    void resized() override;

	Rectangle<int> getViewportBounds();

	void changeListenerCallback(ChangeBroadcaster* source);

	void refreshItems();
	void clearItems();

	void refreshItemsBounds();

	void setSampleItems(std::vector<SampleReference*> mSampleReferences);
	//======================================================
	int calculateAllRowsHeight();
	int calculateRowCount();
	int calculateColumnCount();

	float calculateBoxWidth();
	float calculateBoxHeight();

private:
	void createFreeSampleTile();
	FlexItem createFlexItem();
	//=============================================================================
	std::vector<SampleTile*> mFreeSampleItems;
	std::vector<SampleReference*> mCurrentSampleReferences;
	FlexBox mFlexBox;
	Viewport mViewport;
	static const int MAX_LOADED_SAMPLES = 100;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleContainer)
};
#endif
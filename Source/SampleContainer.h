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

class SampleContainer    : public Component
{
public:
	//========================================================
    SampleContainer();
    ~SampleContainer();

    void paint (Graphics&) override;
    void resized() override;

	void updateItems();
	void clearItems();

	void setSampleItems(std::vector<SampleReference*> mSampleReferences);
	//======================================================
	int calculateAllRowsHeight();
	int calculateRowCount();
	int calculateColumnCount();

private:
	void createFreeSampleTile();
	//=============================================================================
	std::vector<SampleTile*> mUsedSampleTiles; //store all of them? mabye this will fix it
	std::vector<SampleTile*> mFreeSampleTiles;
	std::vector<SampleReference*> mCurrentSampleReferences;
	static const int MAX_LOADED_SAMPLES = 100;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleContainer)
};
#endif
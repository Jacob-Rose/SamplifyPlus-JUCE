/*
  ==============================================================================

    FilterExplorer.h
    Created: 31 May 2018 1:21:30pm
    Author:  jacob

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppProperties.h"

//==============================================================================
/*
*/
class FilterExplorer    : public Component, public ChangeListener
{
public:
    FilterExplorer();
    ~FilterExplorer();

    void paint (Graphics&) override;
    void resized() override;
	void updateTags();
	void filterTags();

	void changeListenerCallback(ChangeBroadcaster* source);

private:
	StringArray mLatestTags;
	TextEditor mSearchBox;
	FlexBox mTagFlexBox;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterExplorer)
};

/*
  ==============================================================================

    FilterExplorer.h
    Created: 31 May 2018 1:21:30pm
    Author:  jacob

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SamplifyProperties.h"

//==============================================================================
/*
*/
class FilterExplorer    : public Component, public TextEditor::Listener
{
public:
    FilterExplorer();
    ~FilterExplorer();

    void paint (Graphics&) override;
    void resized() override;
	void updateTags();
	void filterTags();

	void textEditorTextChanged(TextEditor&) override;

private:
	TextButton mNewButtonTag;
	StringArray mContainedTags; //all tags for current files in directory with text from search bar contained in it
	StringArray mNotContainedTags; //all tags for lower half, only ones with the text in search bar contained in it
	TextEditor mSearchBox;
	FlexBox mTagFlexBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterExplorer)
};
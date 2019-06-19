/*
  ==============================================================================

    FilterExplorer.h
    Created: 31 May 2018 1:21:30pm
    Author:  Jacob Rose

	Component that displays all the tags

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SamplifyProperties.h"


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
	TextEditor mSearchBox;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterExplorer)
};
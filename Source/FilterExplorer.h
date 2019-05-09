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
	StringArray mLatestTags;
	TextEditor mSearchBox;
	FlexBox mTagFlexBox;
	//NewTagButton mNewTagButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterExplorer)
};


class NewTagButton : public Button
{
public:
	NewTagButton();
	~NewTagButton();

	void paintButton(Graphics& g,
		bool isMouseOverButton,
		bool isButtonDown) override;
private:

};
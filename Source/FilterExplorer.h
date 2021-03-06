/*
  ==============================================================================

    FilterExplorer.h
    Created: 31 May 2018 1:21:30pm
    Author:  Jake Rose

	Component that displays all the tags

  ==============================================================================
*/

#ifndef FILTEREXPLORER_H
#define FILTEREXPLORER_H

#include "JuceHeader.h"

#include "SamplifyProperties.h"
#include "TagExplorer_V2.h"

namespace samplify
{
	class FilterExplorer : public Component, public TextEditor::Listener
	{
	public:
		//===========================================================
		FilterExplorer();
		~FilterExplorer();

		void paint(Graphics&) override;
		void resized() override;

		void textEditorTextChanged(TextEditor&) override;

		TextEditor& getSearchBar() { return mSearchBar; }
		TagExplorer_V2& getTagExplorer() { return mTagExplorer; }
	private:
		TagExplorer_V2 mTagExplorer;
		TextEditor mSearchBar;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterExplorer)
	};
}
#endif
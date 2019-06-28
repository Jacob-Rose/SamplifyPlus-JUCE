/*
  ==============================================================================

	SampleTile.h
	Created: 27 June 2019
	Author:  Jake Rose

  ==============================================================================
*/
#ifndef TAGEXPLORER_H
#define TAGEXPLORER_H

#include "JuceHeader.h"

#include "TagContainer.h"

namespace samplify
{
	class TagExplorer : public Component
	{
	public:
		TagExplorer();
		~TagExplorer();
		void updateTags(juce::String newSearch);

		void resized() override;
		void paint(Graphics&) override;
		void addNewTag();
	private:
		TextButton mNewButtonTag;
		TagContainer mNewTags;
		TagContainer mContainedTags; //all tags for current files in directory with text from search bar contained in it
		TagContainer mNotContainedTags; //all tags for lower half, only ones with the text in search bar contained in it
	};
}
#endif
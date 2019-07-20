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
		class Container : public Component
		{
		public:
			Container() : Component()
			{
				addAndMakeVisible(mNewTags);
				addAndMakeVisible(mContainedTags);
				addAndMakeVisible(mNotContainedTags);
				updateTags("");
			}
			void paint(Graphics& g) override;
			//this references samplifyproperties
			void addNewTag(juce::String tag)
			{
				mNewTags.addTag(tag);
				updateTagContainerBounds();
			}
			void updateTags(juce::String newSearch);
			void updateTagContainerBounds();
			void resetTags();

			void resized() override
			{
				updateTagContainerBounds();
			}
		private:
			const Font infoFont = Font(16.0f);
			TagContainer mNewTags;
			TagContainer mContainedTags; //all tags for current files in directory with text from search bar contained in it
			TagContainer mNotContainedTags; //all tags for lower half, only ones with the text in search bar contained in it
		};
		Container* getTagContainer() {
			return &mTagsContainer;
		}
		void resized() override;
		void paint(Graphics&) override;
		void addNewTag();
	private:

		TextButton mNewButtonTag;
		Viewport mTagViewport;
		Container mTagsContainer;
	};
}
#endif
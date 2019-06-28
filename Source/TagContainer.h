/*
  ==============================================================================

    TagContainer.h
    Created: 26 Jun 2019 6:31:38pm
    Author:  Jake Rose

	*Transferable* -- Does not reference outside classes except for objects

  ==============================================================================
*/

#ifndef TAGCONTAINER_H
#define TAGCONTAINER_H

#include "JuceHeader.h"

#include "TagTile.h"

namespace samplify
{
	class TagContainer : public Component
	{
	public:
		//========================================================
		TagContainer();
		~TagContainer();

		void paint(Graphics&) override;
		void resized() override;

		void updateItems(Graphics& g);
		void clearItems();

		StringArray getTags();

		void setItems(StringArray newTags);
		void removeTag(juce::String tag);
		void addTag(juce::String tag);
		//======================================================
		int calculateAllRowsHeight();
		int getRowCount();

		void extendItems();
	private:
		//=============================================================================
		std::vector<TagTile*> mUsedSampleTags;
		StringArray mCurrentTags; //tags to store in the container
		int mLineCount = 0;
		int mMaxItems = 20;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TagContainer)
	};
}
#endif
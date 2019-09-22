/*
  ==============================================================================

    TagLibrary.h
    Created: 21 Sep 2019 5:39:32pm
    Author:  student

  ==============================================================================
*/

#ifndef TAGLIBRARY_H
#define TAGLIBRARY_H

#include <JuceHeader.h>

namespace samplify
{
	class TagLibrary : public ChangeBroadcaster
	{
	public:
		void addTag(juce::String tag, Colour color);
		void addTag(juce::String tag);
		void renameTag(juce::String currentTagName, juce::String desiredName);
		void deleteTag(juce::String tag);
		//StringArray getAllTags();

		void saveTags(PropertiesFile* file);

		Colour getTagColor(juce::String text);
	private:
		std::map<juce::String, Colour> mSampleTagColors;
		JUCE_LEAK_DETECTOR(TagLibrary);
	};
}
#endif

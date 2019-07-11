/*
  ==============================================================================

    TagManager.h
    Created: 10 Jul 2019 1:31:21pm
    Author:  Jake Rose
	*not implemented
  ==============================================================================
*/

#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include "JuceHeader.h"

namespace samplify
{
	class TagManager
	{
	public:
		void addTag(juce::String tag, Colour color);
		void addTag(juce::String tag);
		void deleteTag(juce::String tag);

		Colour getTagColor(juce::String text);
	private:
		std::map<juce::String, Colour> mTagColors;
	};
}
#endif

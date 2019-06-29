/*
  ==============================================================================

    TagTile.h
    Created: 31 May 2018 1:21:39pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef TAGTILE_H
#define TAGTILE_H

#include "JuceHeader.h"

namespace samplify
{
	class TagTile : public Component, public DragAndDropContainer
	{
	public:
		TagTile(juce::String tag);
		~TagTile();

		void setTag(juce::String tag);
		juce::String getTag() { return mTag; }

		void paint(Graphics&) override;
		void resized() override;



		void mouseUp(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;

	private:
		juce::String mTag;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TagTile)
	};
}
#endif
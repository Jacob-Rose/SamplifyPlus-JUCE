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

//==============================================================================
class TagTile    : public Component, public DragAndDropContainer
{
public:
    TagTile(juce::String tag);
    ~TagTile();

    void paint (Graphics&) override;
    void resized() override;

private:
	juce::String mTag;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TagTile)
};
#endif
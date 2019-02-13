/*
  ==============================================================================

    TagTile.h
    Created: 31 May 2018 1:21:39pm
    Author:  jacob

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class TagTile    : public Component, public DragAndDropContainer
{
public:
    TagTile();
    ~TagTile();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TagTile)
};

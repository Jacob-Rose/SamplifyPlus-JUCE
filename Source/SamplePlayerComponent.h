/*
  ==============================================================================

    SamplePlayerComponent.h
    Created: 4 Apr 2020 7:43:10pm
    Author:  jacob

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class SamplePlayerComponent    : public Component
{
public:
    SamplePlayerComponent();
    ~SamplePlayerComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplePlayerComponent)
};

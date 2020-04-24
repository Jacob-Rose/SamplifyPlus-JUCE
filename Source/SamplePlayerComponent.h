/*
  ==============================================================================

    SamplePlayerComponent.h
    Created: 4 Apr 2020 7:43:10pm
    Author:  jacob

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sample.h"

using namespace samplify;

class SamplePlayerComponent    : public Component, public ChangeListener, public TextEditor::Listener
{
public:
    SamplePlayerComponent();
    ~SamplePlayerComponent();

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void textEditorTextChanged(TextEditor&) override;

    void paint (Graphics&) override;
    void resized() override;

    Sample::Reference getCurrentSample();

private:
    Sample::Reference mSample = nullptr;
    TextEditor mSampleInfoEditor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplePlayerComponent)
};

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

class SamplePlayerComponent    : public Component, public ChangeListener, public TextEditor::Listener, public MouseListener
{
public:

    enum ColourIds
    {
        waveformColour = 0x1245
    };
    SamplePlayerComponent();
    ~SamplePlayerComponent();

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void textEditorTextChanged(TextEditor&) override;

    void paint (Graphics&) override;
    void resized() override;

    void mouseDown(const MouseEvent& event) override;

    Sample::Reference getCurrentSample();

private:
    TextEditor mSampleInfoEditor;

    Rectangle<int> m_ThumbnailRect;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplePlayerComponent)
};

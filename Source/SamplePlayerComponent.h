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
#include "TagContainer.h"

namespace samplify
{
    class SamplePlayerComponent : public Component, public ChangeListener, public TextEditor::Listener, public Button::Listener
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

        void buttonClicked(Button*) override;

        void paint(Graphics&) override;
        void resized() override;

        void mouseDown(const MouseEvent& event) override;

        Sample::Reference getCurrentSample();

    private:
        TextEditor mSampleInfoEditor;
        TextButton mSampleColorSelectorButton;
        TextButton mSampleDirectoryChainButton;
        TagContainer mSampleTagContainer;

        Rectangle<int> m_ThumbnailRect;
        Rectangle<int> m_TitleRect;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplePlayerComponent)
    };
}

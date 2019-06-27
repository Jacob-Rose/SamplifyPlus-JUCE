/*
  ==============================================================================

    SampleExplorer.h
    Created: 31 May 2018 12:43:08pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SAMPLEEXPLORER_H
#define SAMPLEEXPLORER_H

#include "JuceHeader.h"

#include "SampleContainer.h"
//==============================================================================
class SampleExplorer    : public Component, public TextEditor::Listener, public ChangeListener
{
public:
    SampleExplorer();
    ~SampleExplorer();

    void paint (Graphics&) override;
    void resized() override;
    void textEditorTextChanged(TextEditor &) override;
	void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    Viewport mViewport;
	TextEditor mSearchBar;
	SampleContainer mSampleContainer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleExplorer)
};
#endif

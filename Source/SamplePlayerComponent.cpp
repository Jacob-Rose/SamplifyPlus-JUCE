/*
  ==============================================================================

    SamplePlayerComponent.cpp
    Created: 4 Apr 2020 7:43:10pm
    Author:  jacob

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SamplePlayerComponent.h"
#include "SamplifyProperties.h"

//==============================================================================
SamplePlayerComponent::SamplePlayerComponent()
{
    addAndMakeVisible(mSampleInfoEditor);
    mSampleInfoEditor.addListener(this);
    mSampleInfoEditor.setMultiLine(true, true);
}

SamplePlayerComponent::~SamplePlayerComponent()
{

}

void SamplePlayerComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}

void SamplePlayerComponent::textEditorTextChanged(TextEditor& e)
{
    getCurrentSample().setInfoText(e.getText());
}

void SamplePlayerComponent::paint (Graphics& g)
{
    if (!getCurrentSample().isNull())
    {
        mSampleInfoEditor.setText(getCurrentSample().getInfoText());
        g.drawText(getCurrentSample().getFilename(), Rectangle<float>(0, 0, getWidth(), 50), Justification::left, true);
    }
}

void SamplePlayerComponent::resized()
{
    mSampleInfoEditor.setBounds(0, 50, getWidth(), getHeight() / 3);
}



Sample::Reference SamplePlayerComponent::getCurrentSample()
{
    return SamplifyProperties::getInstance()->getAudioPlayer()->getSampleReference();
}

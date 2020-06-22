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
#include "SamplifyLookAndFeel.h"

using namespace samplify;

//==============================================================================
SamplePlayerComponent::SamplePlayerComponent() : mSampleTagContainer(false)
{
    addAndMakeVisible(mSampleInfoEditor);
    addAndMakeVisible(mSampleColorSelectorButton);
    addAndMakeVisible(mSampleDirectoryChainButton); //shows the parent folders of sample
    mSampleColorSelectorButton.setName("SampleColor");
    mSampleColorSelectorButton.setButtonText("Sample Color");
    mSampleDirectoryChainButton.setName("ParentFolders");
    mSampleDirectoryChainButton.setButtonText("Parent Folders");
    mSampleInfoEditor.addListener(this);
    mSampleInfoEditor.setTextToShowWhenEmpty("Add Comment about Sample", getLookAndFeel().findColour(TextEditor::textColourId).darker(0.4f));
    mSampleInfoEditor.setMultiLine(true, true);
    //addMouseListener(this, false);
}

SamplePlayerComponent::~SamplePlayerComponent()
{

}

void SamplePlayerComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    resized();
    repaint();
}

void SamplePlayerComponent::textEditorTextChanged(TextEditor& e)
{
    getCurrentSample().setInfoText(e.getText());
}

void SamplePlayerComponent::buttonClicked(Button*)
{
}

void SamplePlayerComponent::paint (Graphics& g)
{
    Sample::Reference samp = getCurrentSample();
    if (!samp.isNull())
    {
        mSampleInfoEditor.setText(samp.getInfoText());
        g.drawText(samp.getFile().getFileName(), m_TitleRect, Justification::left, true);

        //draw thumbnail
        if (samp.getThumbnail() != nullptr)
        {
            g.setColour(getLookAndFeel().findColour(waveformColour));
            if (SampleAudioThumbnail* thumbnail = dynamic_cast<SampleAudioThumbnail*>(samp.getThumbnail().get()))
            {
                samp.getThumbnail()->drawChannels(g, m_ThumbnailRect, 0, samp.getLength(), 1.0f, 160);
            }
            else
            {
                samp.getThumbnail()->drawChannels(g, m_ThumbnailRect, 0, samp.getLength(), 1.0f);
            }
        }
        //Draw Audio Line if playing
        std::shared_ptr<AudioPlayer> auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
        if (auxPlayer->getSampleReference() == samp)
        {
            float startT = auxPlayer->getStartCueRelative();
            float currentT = auxPlayer->getRelativeTime();
            float startX = m_ThumbnailRect.getTopLeft().x + ((m_ThumbnailRect.getTopRight().x - m_ThumbnailRect.getTopLeft().x) * startT);
            float currentX = m_ThumbnailRect.getTopLeft().x + ((m_ThumbnailRect.getTopRight().x - m_ThumbnailRect.getTopLeft().x) * currentT);
            float y1 = m_ThumbnailRect.getTopLeft().y;
            float y2 = m_ThumbnailRect.getBottomLeft().y;
            g.setColour(Colours::black);
            g.drawLine(startX, y1, startX, y2, 1.0f);
            if (auxPlayer->getState() == AudioPlayer::TransportState::Playing)
            {
                g.setColour(Colours::red);
                g.drawLine(currentX, y1, currentX, y2, 1.0f);
                repaint();
            }
        }
    }
}
const int padding = 10;
void SamplePlayerComponent::resized()
{
    if (!getCurrentSample().isNull())
    {
        m_ThumbnailRect = Rectangle<int>(padding, 0, getWidth() - (padding * 2), getHeight() / 2);
        float cHeight = m_ThumbnailRect.getHeight();
        m_TitleRect = Rectangle<int>(0, m_ThumbnailRect.getHeight(), (getWidth()/3)*2, SAMPLE_TILE_TITLE_FONT.getHeight() + 2);
        mSampleDirectoryChainButton.setBounds(m_TitleRect.getWidth(), cHeight, (getWidth() / 3), m_TitleRect.getHeight());
        cHeight += m_TitleRect.getHeight();
        mSampleInfoEditor.setBounds(0, cHeight, getWidth() / 4, getHeight()- cHeight);
        float cWidth = mSampleInfoEditor.getWidth();
        mSampleColorSelectorButton.setBounds(cWidth, cHeight, 100, getHeight() - cHeight);
        cWidth += 100;
    }
    else
    {
        m_ThumbnailRect = Rectangle<int>(0, 0, 0, 0);
        mSampleInfoEditor.setBounds(0, 0, 0, 0);
        mSampleColorSelectorButton.setBounds(0, 0, 0, 0);
        mSampleDirectoryChainButton.setBounds(0, 0, 0, 0); 
    }
    
}

void SamplePlayerComponent::mouseDown(const MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        if (m_ThumbnailRect.contains(e.getMouseDownPosition()))
        {
            float rectWidth = m_ThumbnailRect.getWidth();
            float mouseDownX = e.getMouseDownX();
            SamplifyProperties::getInstance()->getAudioPlayer()->playSample(mouseDownX / rectWidth);
        }
    }
}



Sample::Reference SamplePlayerComponent::getCurrentSample()
{
    return SamplifyProperties::getInstance()->getAudioPlayer()->getSampleReference();
}

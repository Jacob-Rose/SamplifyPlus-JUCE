#include "SamplifyMainComponent.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

SamplifyMainComponent* SamplifyMainComponent::mInstance = nullptr;

SamplifyMainComponent::SamplifyMainComponent() 
{
	//mCopyProtection.reset(new SecurityThread());
	//mCopyProtection.get()->run();
	mInstance = this;
	//usingCustomDeviceManager = true;
	addKeyListener(this);

	SamplifyProperties::getInstance()->setAudioPlayer(&mAudioPlayer);

	addAndMakeVisible(mDirectoryExplorer);
	addAndMakeVisible(mSampleExplorer);
	addAndMakeVisible(mFilterExplorer);
    
	AudioDeviceManager::AudioDeviceSetup adsetup;
	deviceManager.getAudioDeviceSetup(adsetup);
	adsetup.bufferSize = 512;
	adsetup.sampleRate = 48000;
	deviceManager.setAudioDeviceSetup(adsetup, true);
	//deviceManager.initialise(2,2,0,true,juce::String(), &adsetup);
	setAudioChannels(0, 2);
	setupLookAndFeel();
    setSize (600, 400);

}

SamplifyMainComponent::~SamplifyMainComponent()
{
	//deviceManager.closeAudioDevice();
	shutdownAudio();
	/*
	if (mInstance == this)
	{
		mInstance = nullptr;
	}
	*/
}

bool SamplifyMainComponent::keyPressed(const KeyPress& key, Component* originatingComponent)
{
	if (key.getTextCharacter() == 'g')
	{
		SamplifyProperties::getInstance()->getAudioPlayer()->play();
		return true;
	}
	else if (key.getTextCharacter() == 'h')
	{
		SamplifyProperties::getInstance()->getAudioPlayer()->stop();
		return true;
	}
	return false;
}

void SamplifyMainComponent::setupOutputChannels(int numOutputChannels)
{
}

void SamplifyMainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.

	// You can use this function to initialise any resources you might need,
	// but be careful - it will be called on the audio thread, not the GUI thread.
	mAudioPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
	// For more details, see the help for AudioProcessor::prepareToPlay()
}

void SamplifyMainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	mAudioPlayer.getNextAudioBlock(bufferToFill);
	/*
	if (mCopyProtection.get() == nullptr)
	{
		juce::JUCEApplicationBase::quit();
	}
	*/
}

void SamplifyMainComponent::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.
	mAudioPlayer.releaseResources();
	// For more details, see the help for AudioProcessor::releaseResources()
}

void samplify::SamplifyMainComponent::setupLookAndFeel()
{
	getLookAndFeel().setColour(MAIN_BASE_COLOR_ID, Colours::white);
	getLookAndFeel().setColour(MAIN_ACCENT_COLOR_ID, Colours::darkblue);
	getLookAndFeel().setColour(SAMPLE_TILE_BG_DEFAULT_COLOR_ID, getLookAndFeel().findColour(MAIN_BASE_COLOR_ID));
	getLookAndFeel().setColour(SAMPLE_TILE_BG_HOVER_COLOR_ID, getLookAndFeel().findColour(MAIN_BASE_COLOR_ID).darker());
	getLookAndFeel().setColour(SAMPLE_TILE_FG_DEFAULT_COLOR_ID, getLookAndFeel().findColour(MAIN_ACCENT_COLOR_ID));
	getLookAndFeel().setColour(SAMPLE_TILE_FG_HOVER_COLOR_ID, getLookAndFeel().findColour(MAIN_ACCENT_COLOR_ID));
}

//==============================================================================
void SamplifyMainComponent::paint (Graphics& g)
{
    // Our component is opaque, so we must completely fill the background with a solid colour
    g.fillAll (getLookAndFeel().findColour(MAIN_BASE_COLOR_ID));
}

void SamplifyMainComponent::resized()
{
	int widthSegment = getWidth() / 5;
	mDirectoryExplorer.setBounds(0, 0, widthSegment, getHeight());
	mSampleExplorer.setBounds(widthSegment, 0, widthSegment * 3, getHeight());
	mFilterExplorer.setBounds(widthSegment * 4, 0, widthSegment, getHeight());
}

SamplifyMainComponent* SamplifyMainComponent::getInstance()
{
	return mInstance;
}

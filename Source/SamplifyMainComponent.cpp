#include "SamplifyMainComponent.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

SamplifyMainComponent* SamplifyMainComponent::mInstance = nullptr;
SamplifyMainComponent::SamplifyMainComponent()
{
	mInstance = this;
    mDirectoryExplorer.reset(new DirectoryExplorer());
    mSampleExplorer.reset(new SampleExplorer());
    mFilterExplorer.reset(new FilterExplorer());
	mMenuBar.reset(new MenuBarComponent());
	mAudioPlayer.reset(new AudioPlayer());
	SamplifyProperties::getInstance()->setAudioPlayer(mAudioPlayer.get());

	addAndMakeVisible(*mDirectoryExplorer);
	addAndMakeVisible(*mSampleExplorer);
	addAndMakeVisible(*mFilterExplorer);
	addAndMakeVisible(*mMenuBar);
    
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

SamplifyMainComponent::SamplifyMainComponent(AudioDeviceManager& deviceManager) :
	AudioAppComponent(deviceManager)
{
}

SamplifyMainComponent::~SamplifyMainComponent()
{
	//deviceManager.closeAudioDevice();
}

void SamplifyMainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.

	// You can use this function to initialise any resources you might need,
	// but be careful - it will be called on the audio thread, not the GUI thread.
	if (mAudioPlayer != nullptr)
	{
		mAudioPlayer->prepareToPlay(samplesPerBlockExpected, sampleRate);
	}
	// For more details, see the help for AudioProcessor::prepareToPlay()
}

void SamplifyMainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	if (mAudioPlayer != nullptr)
	{
		mAudioPlayer->getNextAudioBlock(bufferToFill);
	}
}

void SamplifyMainComponent::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.
	if (mAudioPlayer != nullptr)
	{
		mAudioPlayer->releaseResources();
	}


	// For more details, see the help for AudioProcessor::releaseResources()
}

void samplify::SamplifyMainComponent::setupLookAndFeel()
{
	getLookAndFeel().setColour(MAIN_BASE_COLOR, Colours::white);
	getLookAndFeel().setColour(MAIN_ACCENT_COLOR, Colours::orangered);
	getLookAndFeel().setColour(SAMPLETILE_COLOR_BG_DEFAULT, getLookAndFeel().findColour(MAIN_BASE_COLOR));
	getLookAndFeel().setColour(SAMPLETILE_COLOR_BG_HOVER, getLookAndFeel().findColour(MAIN_BASE_COLOR).darker());
	getLookAndFeel().setColour(SAMPLETILE_COLOR_FG_DEFAULT, getLookAndFeel().findColour(MAIN_ACCENT_COLOR));
	getLookAndFeel().setColour(SAMPLETILE_COLOR_FG_HOVER, getLookAndFeel().findColour(MAIN_ACCENT_COLOR));
}

//==============================================================================
void SamplifyMainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour(MAIN_BASE_COLOR));
}

void SamplifyMainComponent::resized()
{
	int widthSegment = getWidth() / 5;
	mDirectoryExplorer->setBounds(0, 16, widthSegment, getHeight());
	mMenuBar->setBounds(0, 0, getWidth(), 16);
	mSampleExplorer->setBounds(widthSegment, 16, widthSegment * 3, getHeight()-16);
	mFilterExplorer->setBounds(widthSegment * 4, 16, widthSegment, getHeight()-16);
}

SamplifyMainComponent* SamplifyMainComponent::getInstance()
{
	return mInstance;
}

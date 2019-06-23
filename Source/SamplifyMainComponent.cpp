#include "SamplifyMainComponent.h"

SamplifyMainComponent* SamplifyMainComponent::mInstance = nullptr;
SamplifyMainComponent::SamplifyMainComponent()
{
	mInstance = this;
    mDirectoryExplorer.reset(new DirectoryExplorer());
    mSampleExplorer.reset(new SampleExplorer());
    mFilterExplorer.reset(new FilterExplorer());
	mMenuBar.reset(new MenuBarComponent());

	addAndMakeVisible(*mDirectoryExplorer);
	addAndMakeVisible(*mSampleExplorer);
	addAndMakeVisible(*mFilterExplorer);
	addAndMakeVisible(*mMenuBar);
    
	AudioDeviceManager::AudioDeviceSetup ads;
	//setAudioChannels(0,2)

    setSize (600, 400);
}

SamplifyMainComponent::SamplifyMainComponent(AudioDeviceManager& deviceManager) :
	AudioAppComponent(deviceManager)
{
}

SamplifyMainComponent::~SamplifyMainComponent()
{
}

void SamplifyMainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.

	// You can use this function to initialise any resources you might need,
	// but be careful - it will be called on the audio thread, not the GUI thread.
	SamplifyProperties::getInstance()->getAudioPlayer()->prepareToPlay(samplesPerBlockExpected, sampleRate);
	// For more details, see the help for AudioProcessor::prepareToPlay()
}

void SamplifyMainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	// Your audio-processing code goes here!

	// For more details, see the help for AudioProcessor::getNextAudioBlock()
	SamplifyProperties::getInstance()->getAudioPlayer()->getNextAudioBlock(bufferToFill);
}

void SamplifyMainComponent::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.
	SamplifyProperties::getInstance()->getAudioPlayer()->releaseResources();
	// For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void SamplifyMainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (SamplifyProperties::getInstance()->MAIN_BASE_COLOR);
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

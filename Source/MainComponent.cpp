#include "MainComponent.h"


MainComponent::MainComponent()
{
    mDirectoryExplorer.reset(new DirectoryExplorer());
    mSampleExplorer.reset(new SampleExplorer());
    mFilterExplorer.reset(new FilterExplorer());

	addAndMakeVisible(*mDirectoryExplorer);
	addAndMakeVisible(*mSampleExplorer);
	addAndMakeVisible(*mFilterExplorer);
    
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.

	// You can use this function to initialise any resources you might need,
	// but be careful - it will be called on the audio thread, not the GUI thread.

	// For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	// Your audio-processing code goes here!

	// For more details, see the help for AudioProcessor::getNextAudioBlock()

	// Right now we are not producing any data, in which case we need to clear the buffer
	// (to prevent the output of random noise)
	bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.

	// For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::white);
}

void MainComponent::resized()
{
	float widthSegment = getWidth() / 5;
	mDirectoryExplorer->setBounds(0, 0, widthSegment, getHeight());
	mSampleExplorer->setBounds(widthSegment, 0, widthSegment * 3, getHeight());
	mFilterExplorer->setBounds(widthSegment * 4, 0, widthSegment, getHeight());
}

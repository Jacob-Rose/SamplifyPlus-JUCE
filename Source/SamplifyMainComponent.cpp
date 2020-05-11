#include "SamplifyMainComponent.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

SamplifyMainComponent* SamplifyMainComponent::mInstance = nullptr;

SamplifyMainComponent::SamplifyMainComponent()
{
	setupLookAndFeel();
	mInstance = this;
	addKeyListener(this);
	mAudioPlayer = std::make_shared<AudioPlayer>();
	SamplifyProperties::getInstance()->setAudioPlayer(mAudioPlayer);


	addAndMakeVisible(mDirectoryExplorer);
	addAndMakeVisible(mSampleExplorer);
	addAndMakeVisible(mFilterExplorer);
	addAndMakeVisible(mSamplePlayer);
	//addAndMakeVisible(unlockForm);
    
	//Setup Audio
	AudioDeviceManager::AudioDeviceSetup adsetup;
	deviceManager.getAudioDeviceSetup(adsetup);
	adsetup.bufferSize = 512;
	adsetup.sampleRate = 48000;
	deviceManager.setAudioDeviceSetup(adsetup, true);
	//deviceManager.initialise(2,2,0,true,juce::String(), &adsetup);
	setAudioChannels(0, 2);

	SamplifyProperties::getInstance()->getSampleLibrary()->addChangeListener(&mSampleExplorer);
	SamplifyProperties::getInstance()->getAudioPlayer()->addChangeListener(&mSamplePlayer);
    setSize (600, 400);
	//startTimer(100);

	//initial load
	SamplifyProperties::getInstance()->getSampleLibrary()->updateCurrentSamples("");
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
	//pause/play controls
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

void SamplifyMainComponent::changeListenerCallback(ChangeBroadcaster* source)
{
	SamplifyProperties::getInstance()->savePropertiesFile();
}

void SamplifyMainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	if (mAudioPlayer != nullptr)
	{
		mAudioPlayer->prepareToPlay(samplesPerBlockExpected, sampleRate);
	}
}

void SamplifyMainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	if (mAudioPlayer != nullptr)
	{
		mAudioPlayer->getNextAudioBlock(bufferToFill);
	}
	
	/*
	if (mCopyProtection.get() == nullptr)
	{
		juce::JUCEApplicationBase::quit();
	}
	*/
}

void SamplifyMainComponent::releaseResources()
{
	if (mAudioPlayer != nullptr)
	{
		mAudioPlayer->releaseResources();
	}
}

void samplify::SamplifyMainComponent::setupLookAndFeel()
{
	getLookAndFeel().setColour(MAIN_BASE_COLOR_ID, MAIN_BASE_COLOR);
	getLookAndFeel().setColour(TextEditor::backgroundColourId, MAIN_BASE_COLOR);
	getLookAndFeel().setColour(MAIN_ACCENT_COLOR_ID, MAIN_ACCENT_COLOR);
	getLookAndFeel().setColour(SAMPLE_TILE_BG_DEFAULT_COLOR_ID, SAMPLE_TILE_BG_DEFAULT_COLOR);
	getLookAndFeel().setColour(SAMPLE_TILE_BG_HOVER_COLOR_ID, SAMPLE_TILE_BG_HOVER_COLOR);
	getLookAndFeel().setColour(SAMPLE_TILE_FG_DEFAULT_COLOR_ID, SAMPLE_TILE_FG_DEFAULT_COLOR);
	getLookAndFeel().setColour(SAMPLE_TILE_FG_HOVER_COLOR_ID, SAMPLE_TILE_FG_HOVER_COLOR);
	getLookAndFeel().setColour(SAMPLE_TILE_OUTLINE_DEFAULT_COLOR_ID, SAMPLE_TILE_OUTLINE_DEFAULT_COLOR);
	getLookAndFeel().setColour(SAMPLE_TILE_OUTLINE_HOVER_COLOR_ID, SAMPLE_TILE_OUTLINE_HOVER_COLOR);
}

//==============================================================================
void SamplifyMainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour(MAIN_BASE_COLOR_ID));
}

void SamplifyMainComponent::resized()
{
	int widthSegment = getWidth() / 5;
	mDirectoryExplorer.setBounds(0, 0, widthSegment, getHeight());
	mSampleExplorer.setBounds(widthSegment, 0, widthSegment * 3, getHeight());
	mFilterExplorer.setBounds(widthSegment * 4, getHeight() / 2, widthSegment, getHeight()/2);
	mSamplePlayer.setBounds(widthSegment * 4, 0, widthSegment, getHeight()/2);
}

SamplifyMainComponent* SamplifyMainComponent::getInstance()
{
	return mInstance;
}

void SamplifyMainComponent::timerCallback()
{
	/*
	if (!isUnlocked && authorizationStatus.isUnlocked())
	{
		isUnlocked = true;
		unlockApp();
	}
	*/
}
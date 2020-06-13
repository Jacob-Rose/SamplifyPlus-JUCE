#include "SamplifyMainComponent.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

SamplifyMainComponent* SamplifyMainComponent::mInstance = nullptr;

SamplifyMainComponent::SamplifyMainComponent()
{
	setupLookAndFeel(getLookAndFeel());
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
	//startTimer(100);
	setSize(AppValues::getInstance().WINDOW_WIDTH, AppValues::getInstance().WINDOW_HEIGHT);
	//initial load
	SamplifyProperties::getInstance()->getSampleLibrary()->updateCurrentSamples("");
}

SamplifyMainComponent::~SamplifyMainComponent()
{
	shutdownAudio();
	if (mInstance == this)
	{
		mInstance = nullptr;
	}
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
}

void SamplifyMainComponent::releaseResources()
{
	if (mAudioPlayer != nullptr)
	{
		mAudioPlayer->releaseResources();
	}
}


void samplify::SamplifyMainComponent::setupLookAndFeel(LookAndFeel& laf)
{
	laf.setColour(ResizableWindow::backgroundColourId, AppValues::getInstance().MAIN_BACKGROUND_COLOR);

	laf.setColour(SampleTile::backgroundDefaultColorID, AppValues::getInstance().MAIN_BACKGROUND_COLOR);
	laf.setColour(SampleTile::backgroundHoverColorID, AppValues::getInstance().MAIN_BACKGROUND_COLOR.darker(0.2f));
	laf.setColour(SampleTile::foregroundDefaultColorID, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(SampleTile::foregroundHoverColorID, AppValues::getInstance().MAIN_FOREGROUND_COLOR.darker(0.2f));

	laf.setColour(SampleExplorer::loadingWheelColorId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);

	laf.setColour(DirectoryExplorerTreeViewItem::defaultBackgroundId, Colours::transparentWhite);
	laf.setColour(DirectoryExplorerTreeViewItem::selectedBackgroundId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(DirectoryExplorerTreeViewItem::checkboxActiveBackgroundId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(DirectoryExplorerTreeViewItem::checkboxMixedBackgroundId, AppValues::getInstance().MAIN_FOREGROUND_COLOR.withSaturation(0.2f));
	laf.setColour(DirectoryExplorerTreeViewItem::checkboxDisabledBackgroundId, Colours::grey);
	laf.setColour(DirectoryExplorerTreeViewItem::checkboxNotLoadedBackgroundId, Colours::lightpink);

	laf.setColour(TextEditor::backgroundColourId, AppValues::getInstance().MAIN_BACKGROUND_COLOR);
	laf.setColour(TextEditor::textColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(TextEditor::outlineColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);

	laf.setColour(TextButton::textColourOnId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(TextButton::buttonOnColourId, AppValues::getInstance().MAIN_BACKGROUND_COLOR.darker(0.2f));
	laf.setColour(TextButton::buttonColourId, AppValues::getInstance().MAIN_BACKGROUND_COLOR);
	laf.setColour(TextButton::textColourOffId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);

	laf.setColour(ComboBox::backgroundColourId, AppValues::getInstance().MAIN_BACKGROUND_COLOR);
	laf.setColour(ComboBox::textColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(ComboBox::arrowColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(ComboBox::outlineColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(ComboBox::buttonColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);

	laf.setColour(PopupMenu::backgroundColourId, AppValues::getInstance().MAIN_BACKGROUND_COLOR);
	laf.setColour(PopupMenu::headerTextColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(PopupMenu::highlightedBackgroundColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR.brighter(0.3f));
	laf.setColour(PopupMenu::highlightedTextColourId, AppValues::getInstance().MAIN_BACKGROUND_COLOR);
	laf.setColour(PopupMenu::textColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
}

//==============================================================================
void SamplifyMainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
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
#include "SamplifyMainComponent.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

SamplifyMainComponent* SamplifyMainComponent::mInstance = nullptr;

SamplifyMainComponent::SamplifyMainComponent() : 
	mResizableEdgeDirectoryExplorer(&mDirectoryExplorer, &mResizableEdgeDirectoryExplorerBounds, ResizableEdgeComponent::Edge::rightEdge),
	mResizableEdgeFilterExplorer(&mFilterExplorer, &mResizableEdgeDirectoryExplorerBounds, ResizableEdgeComponent::Edge::leftEdge),
	mResizableEdgeAudioPlayer(&mSamplePlayerComponent, &mResizableEdgeAudioPlayerBounds, ResizableEdgeComponent::Edge::topEdge)
{
	setupLookAndFeel(getLookAndFeel());
	mInstance = this;
	addKeyListener(this);

	mAudioPlayer = std::make_shared<AudioPlayer>();
	SamplifyProperties::getInstance()->setAudioPlayer(mAudioPlayer);

	mResizableEdgeDirectoryExplorerBounds.setMinimumWidth(100);
	mResizableEdgeFilterExplorerBounds.setMinimumWidth(100);
	mResizableEdgeAudioPlayerBounds.setMinimumHeight(100);
	mResizableEdgeAudioPlayerBounds.setMaximumHeight(400);
	mDirectoryExplorer.setSize(200, 1000); //todo make these save
	mFilterExplorer.setSize(200, 1000); //todo make these save
	mSamplePlayerComponent.setSize(200, 200); //todo make these save
	mResizableEdgeDirectoryExplorer.addMouseListener(this, false);
	mResizableEdgeFilterExplorer.addMouseListener(this, false);
	mResizableEdgeAudioPlayer.addMouseListener(this, false);
	addAndMakeVisible(mResizableEdgeFilterExplorer);
	addAndMakeVisible(mResizableEdgeDirectoryExplorer);
	addAndMakeVisible(mResizableEdgeAudioPlayer);

	addAndMakeVisible(mDirectoryExplorer);
	addAndMakeVisible(mSampleExplorer);
	addAndMakeVisible(mFilterExplorer);
	addAndMakeVisible(mSamplePlayerComponent);

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
	SamplifyProperties::getInstance()->getAudioPlayer()->addChangeListener(&mSamplePlayerComponent);
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
	//todo
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

	laf.setColour(DirectoryExplorerTreeViewItem::defaultBackgroundId, AppValues::getInstance().MAIN_BACKGROUND_COLOR.withAlpha(0.0f));
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

	laf.setColour(ScrollBar::thumbColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);

	laf.setColour(ComboBox::backgroundColourId, AppValues::getInstance().MAIN_BACKGROUND_COLOR);
	laf.setColour(ComboBox::textColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(ComboBox::arrowColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(ComboBox::outlineColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(ComboBox::buttonColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);


	laf.setColour(SamplePlayerComponent::waveformColourId, AppValues::getInstance().MAIN_FOREGROUND_COLOR);

	laf.setColour(LookAndFeel_V4::ColourScheme::UIColour::defaultFill, AppValues::getInstance().MAIN_BACKGROUND_COLOR);
	laf.setColour(LookAndFeel_V4::ColourScheme::UIColour::defaultText, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(LookAndFeel_V4::ColourScheme::UIColour::highlightedFill, AppValues::getInstance().MAIN_FOREGROUND_COLOR);
	laf.setColour(LookAndFeel_V4::ColourScheme::UIColour::highlightedText, AppValues::getInstance().MAIN_BACKGROUND_COLOR);

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
const int edgeSize = 8;
void SamplifyMainComponent::resized()
{
	int lWidth = mDirectoryExplorer.getWidth(); //set by dragger
	mDirectoryExplorer.setBounds(0, 0, lWidth, getHeight());
	mResizableEdgeDirectoryExplorer.setBounds(lWidth, 0, edgeSize, getHeight());
	lWidth += mResizableEdgeDirectoryExplorer.getWidth();

	int rWidth = mFilterExplorer.getWidth(); //set by dragger
	mFilterExplorer.setBounds(getWidth() - rWidth, 0, rWidth, getHeight()); 

	mResizableEdgeFilterExplorer.setBounds(getWidth() - rWidth - edgeSize, 0, edgeSize, getHeight());
	rWidth += mResizableEdgeFilterExplorer.getWidth();

	float bHeight = mSamplePlayerComponent.getHeight();
	mSamplePlayerComponent.setBounds(lWidth, getHeight() - bHeight, getWidth() - (lWidth + rWidth), bHeight);
	mResizableEdgeAudioPlayer.setBounds(lWidth, getHeight() - (bHeight + edgeSize), getWidth() - (lWidth + rWidth), edgeSize);
	bHeight += mResizableEdgeAudioPlayer.getHeight();

	mSampleExplorer.setBounds(lWidth, 0, getWidth() - (rWidth + lWidth), getHeight() - bHeight);

	mResizableEdgeDirectoryExplorerBounds.setMaximumWidth(getWidth() - (rWidth));
	mResizableEdgeFilterExplorerBounds.setMaximumWidth(getWidth() - (lWidth));
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

void SamplifyMainComponent::mouseDrag(const MouseEvent& e)
{
	resized();
}

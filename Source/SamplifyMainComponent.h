#pragma once

#include "JuceHeader.h"

#include "AudioPlayer.h"
#include "DirectoryExplorer.h"
#include "FilterExplorer.h"
#include "SampleExplorer.h"

/*
    The Root Component of the Application
*/
class SamplifyMainComponent   : public AudioAppComponent
{
public:

    SamplifyMainComponent();
    ~SamplifyMainComponent();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	AudioPlayer* getAudioPlayer();
	static SamplifyMainComponent* getInstance();

private:
    std::unique_ptr<DirectoryExplorer> mDirectoryExplorer = nullptr;
    std::unique_ptr<SampleExplorer> mSampleExplorer = nullptr;
    std::unique_ptr<FilterExplorer> mFilterExplorer = nullptr;
	std::unique_ptr<MenuBarComponent> mMenuBar = nullptr;

	static SamplifyMainComponent* mInstance;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplifyMainComponent)
};

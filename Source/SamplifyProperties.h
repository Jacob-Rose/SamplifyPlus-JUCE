/*
  ==============================================================================

    AppProperties.h
    Created: 31 May 2018 3:16:14pm
    Author:  Jacob Rose

	extends JUCE::ApplicationProperties and makes it a singleton with added features for my needs
  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleLibrary.h"
#include <string>
#include <vector>
#include "AudioPlayer.h"

#define SAMPVIEW_ASPECTRATIO = 9.0f/16.0f; //opposite because system based on width not height
#define SAMPVIEW_MAX_WIDTH = 250;
#define SAMPVIEW_MIN_WIDTH = 150;


class SamplifyProperties : public ApplicationProperties
{
public:

	static void initInstance();
	static void cleanupInstance();
	static SamplifyProperties* getInstance();
	
	void init();
	void cleanup();

	void loadDirectories();

	File browseForDirectory();

	void browseForDirectoryAndAdd();

	std::vector<File> getDirectories() { return mDirectories; }

	File getSelectedDirectory() { return mSelectedDirectory;	}

	SampleLibrary* getSampleLibrary() {	return mSampleLibrary.get(); }
	AudioPlayer* getAudioPlayer() { return &mAudioPlayer; }

	void setSelectedDirectory(File directory);

	void saveDirectories();

	void getThemeColor(std::string key);

	void clearDirectories();

	void updateDirectories(std::vector<File> directories);

	const Colour MAIN_BASE_COLOR = Colours::slategrey;
	const Colour MAIN_ACCENT_COLOR = Colours::orangered;
	const Colour MAIN_TEXT_COLOR = Colours::white;
private:
	SamplifyProperties();
	~SamplifyProperties();

	bool mIsInit = false;

	ApplicationProperties mApplicationProperties;
	std::unique_ptr<SampleLibrary> mSampleLibrary = nullptr;
	std::vector<File> mDirectories = std::vector<File>();
	File mSelectedDirectory = File("");
	AudioPlayer mAudioPlayer;

	static SamplifyProperties* smAppProperties;
	JUCE_LEAK_DETECTOR(SamplifyProperties)
};
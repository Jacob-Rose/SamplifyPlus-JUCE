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
#include <map>
#include "AudioPlayer.h"
#include "TagDrawer.h"


class SamplifyProperties : public ApplicationProperties
{
public:
	//==================================================================================
	static void initInstance();
	static void cleanupInstance();
	static SamplifyProperties* getInstance();
	//==================================================================================
	void init();
	void cleanup();
	//=Saving============================================================================
	void loadDirectoriesFromPropertiesFile();
	void saveDirectoriesToPropertiesFile();
	//=Directories=========================================================================
	void setDirectories(std::vector<File> directories);
	std::vector<File> getDirectories() { return mDirectories; }
	void clearDirectories();

	File browseForDirectory();
	void browseForDirectoryAndAdd();

	void setSelectedDirectory(File directory);
	File getSelectedDirectory() { return mSelectedDirectory;	}
	//==================================================================================
	SampleLibrary* getSampleLibrary() {	return mSampleLibrary.get(); }
	AudioPlayer* getAudioPlayer() { return &mAudioPlayer; }
	//==================================================================================
	void addTag(std::string text, Colour color);
	void addTag(std::string text)
	{
		Random& r = Random::getSystemRandom();
		addTag(text, Colour(r.nextInt(Range(0, 256)),
			r.nextInt(Range(0, 256)),
			r.nextInt(Range(0, 256))));
	}

	Colour getTagColor(std::string text);
	//==================================================================================
	const Colour MAIN_BASE_COLOR = Colours::slategrey;
	const Colour MAIN_ACCENT_COLOR = Colours::orangered;
	const Colour MAIN_TEXT_COLOR = Colours::white;
private:
	//==================================================================================
	SamplifyProperties();
	~SamplifyProperties();
	//==================================================================================
	ApplicationProperties mApplicationProperties;
	std::unique_ptr<SampleLibrary> mSampleLibrary = nullptr;
	std::map<std::string, Colour> mSampleTagColors;
	std::vector<File> mDirectories = std::vector<File>();
	File mSelectedDirectory = File("");
	AudioPlayer mAudioPlayer;
	//==================================================================================
	static SamplifyProperties* smAppProperties;
	bool mIsInit = false;

	JUCE_LEAK_DETECTOR(SamplifyProperties)
};
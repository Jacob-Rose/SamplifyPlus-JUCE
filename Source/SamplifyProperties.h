/*
  ==============================================================================

    AppProperties.h
    Created: 31 May 2018 3:16:14pm
    Author:  Jake Rose

	extends JUCE::ApplicationProperties and makes it a singleton with added features for my needs
  ==============================================================================
*/

#ifndef SAMPLIFYPROPERTIES_H
#define SAMPLIFYPROPERTIES_H

#include "JuceHeader.h"

#include "SampleLibrary.h"
#include "AudioPlayer.h"
#include "TagDrawer.h"

#include <string>
#include <vector>
#include <map>


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
	void addTag(juce::String text, Colour color);
	void addTag(juce::String text)
	{
		Random& r = Random::getSystemRandom();
		addTag(text, Colour(r.nextInt(Range(0, 256)),
			r.nextInt(Range(0, 256)),
			r.nextInt(Range(0, 256))));
	}

	Colour getTagColor(juce::String text);
	//==================================================================================
	const Colour MAIN_BASE_COLOR = Colours::slategrey;
	const Colour MAIN_ACCENT_COLOR = Colours::orangered;
	const Colour MAIN_TEXT_COLOR = Colours::white;

	const int TAG_TEXT_PADDING = 2;
	const int TAG_TEXT_FONT_SIZE = 14;

	const Colour SAMPLE_TILE_COLOR_BG_DEFAULT = Colours::white;
	const Colour SAMPLE_TILE_COLOR_BG_HOVER = Colours::lightgrey;
	const Colour SAMPLE_TILE_COLOR_FG_DEFAULT = Colours::lavenderblush;
	const Colour SAMPLE_TILE_COLOR_FG_HOVER = Colours::lavenderblush;

	const int SAMPLE_TILE_MIN_WIDTH = 150;
	const float SAMPLE_TILE_ASPECT_RATIO = 9.0f / 16.0f;

	const int SAMPLE_TAG_FONT_SIZE = 12;
	const int SAMPLE_TAG_TEXT_PADDING = 2;
private:
	//==================================================================================
	SamplifyProperties();
	~SamplifyProperties();
	//==================================================================================
	ApplicationProperties mApplicationProperties;
	std::unique_ptr<SampleLibrary> mSampleLibrary = nullptr;
	std::map<juce::String, Colour> mSampleTagColors;
	std::vector<File> mDirectories = std::vector<File>();
	File mSelectedDirectory = File("");
	AudioPlayer mAudioPlayer;
	//==================================================================================
	static SamplifyProperties* smAppProperties;
	bool mIsInit = false;

	JUCE_LEAK_DETECTOR(SamplifyProperties)
};
#endif
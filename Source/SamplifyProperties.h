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

#include "AudioPlayer.h"
#include "SampleLibrary.h"
#include "SampleList.h"
#include "TagDrawer.h"

#include <map>
#include <string>
#include <vector>

namespace samplify
{
	class SamplifyProperties : public ApplicationProperties
	{
	public:
		//=Instance Handling========================================
		static void initInstance();
		static void cleanupInstance();
		static SamplifyProperties* getInstance();
		//=Init/Cleanup============================================
		void init();
		void cleanup();
		//=Saving=================================================
		void loadDirectoriesFromPropertiesFile();
		void saveDirectoriesToPropertiesFile();
		void loadTagColorsFromPropertiesFile();
		//=Directories==============================================
		void setDirectories(std::vector<File> directories);
		std::vector<File> getDirectories() { return mDirectories; }
		void clearDirectories();
		//=======================================================
		File browseForDirectory();
		void browseForDirectoryAndAdd();

		void setSelectedDirectory(File directory);
		File getSelectedDirectory() { return mSelectedDirectory; }
		//=======================================================
		SampleLibrary* getSampleLibrary() { return mSampleLibrary.get(); }
		void setAudioPlayer(AudioPlayer* ap) { mAudioPlayer = ap; }
		AudioPlayer* getAudioPlayer() { return mAudioPlayer; }
		//=======================================================
		void addTag(juce::String text, Colour color);
		void addTag(juce::String text);

		Colour getTagColor(juce::String text);
	private:
		//========================================================
		SamplifyProperties();
		~SamplifyProperties();
		//========================================================
		ApplicationProperties mApplicationProperties;
		std::unique_ptr<SampleLibrary> mSampleLibrary = nullptr;
		std::map<juce::String, Colour> mSampleTagColors;
		std::vector<File> mDirectories = std::vector<File>();
		File mSelectedDirectory = File("");
		AudioPlayer* mAudioPlayer = nullptr;
		//========================================================
		static SamplifyProperties* smAppProperties;
		bool mIsInit = false;

		JUCE_LEAK_DETECTOR(SamplifyProperties)
	};
}

#endif
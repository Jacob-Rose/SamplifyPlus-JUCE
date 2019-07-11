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
#include "SampleList.h"

#include <map>
#include <string>
#include <vector>

namespace samplify
{
	class SamplifyProperties : public ApplicationProperties
	{
	public:
		//=======================================================
		class LoadSamplesThread : public ThreadWithProgressWindow
		{
		public:
			LoadSamplesThread(File file) : ThreadWithProgressWindow("loading samples...", true, false), 
				mDirectory(file) {}
			void run();
		private:
			File mDirectory;
		};

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
		void removeDirectory(File dir);
		void addDirectory(File dir);
		std::vector<File> getDirectories() { return mDirectories; }
		void clearDirectories();

		//=======================================================
		static File browseForDirectory();
		void browseForDirectoryAndAdd();
		void openRemoveDirectoryDialogue();

		//=======================================================
		void setAudioPlayer(AudioPlayer* ap) { mAudioPlayer = ap; }
		AudioPlayer* getAudioPlayer() { return mAudioPlayer; }
		//=======================================================
		void addTag(juce::String tag, Colour color);
		void addTag(juce::String tag);
		void deleteTag(juce::String tag);

		Colour getTagColor(juce::String text);
	private:
		//========================================================
		SamplifyProperties();
		~SamplifyProperties();
		//========================================================
		ApplicationProperties mApplicationProperties;
		std::map<juce::String, Colour> mSampleTagColors;
		std::vector<File> mDirectories;
		File mSelectedDirectory = File("");
		AudioPlayer* mAudioPlayer = nullptr;
		//========================================================
		static SamplifyProperties* smAppProperties;
		bool mIsInit = false;

		JUCE_LEAK_DETECTOR(SamplifyProperties)
	};
}

#endif
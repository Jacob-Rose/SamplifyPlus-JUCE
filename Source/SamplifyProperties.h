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
#include "TagDrawer.h"

#include <map>
#include <string>
#include <vector>

namespace samplify
{
	class SamplifyProperties : public ApplicationProperties, public ChangeBroadcaster
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

		class TagLibrary
		{
		public:
			void addTag(juce::String tag, Colour color);
			void addTag(juce::String tag);
			void renameTag(juce::String currentTagName, juce::String desiredName);
			void deleteTag(juce::String tag);
			//StringArray getAllTags();

			Colour getTagColor(juce::String text);
			friend SamplifyProperties;
		private:
			std::map<juce::String, Colour> mSampleTagColors;
			JUCE_LEAK_DETECTOR(TagLibrary);
		};

		//=Instance Handling========================================
		static void initInstance();
		static void cleanupInstance();
		static SamplifyProperties* getInstance();
		//=Init/Cleanup============================================
		void init();
		void cleanup();
		//=Saving=================================================
		void loadPropertiesFile();
		void savePropertiesFile();
		//=Directories==============================================
		void removeDirectory(File dir);
		void addDirectory(File dir);
		void setDirectories(std::vector<File> directories);
		std::vector<File> getDirectories() { return mDirectories; }
		void clearDirectories();
		//=======================================================
		static File browseForDirectory();
		void browseForDirectoryAndAdd();

		void loadSamplesFromDirectory(File& file);
		void loadSamplesFromDirectories(std::vector<File>&);
		//=======================================================
		std::shared_ptr<SampleLibrary> getSampleLibrary() { return mSampleLibrary; }
		/*Take control of AP so it will be handeled*/
		void setAudioPlayer(std::shared_ptr<AudioPlayer> ap) { mAudioPlayer = ap; }
		std::shared_ptr<AudioPlayer> getAudioPlayer() { return mAudioPlayer; }
		TagLibrary& getTagLibrary() { return mTagLibrary; }
		ApplicationProperties& getApplicationProperties() { return mApplicationProperties; }
		//=======================================================
	private:
		//========================================================
		SamplifyProperties();
		~SamplifyProperties();
		//========================================================
		ApplicationProperties mApplicationProperties;
		std::shared_ptr<SampleLibrary> mSampleLibrary = nullptr; //this should be unique_ptr
		TagLibrary mTagLibrary;
		std::vector<File> mDirectories = std::vector<File>();
		std::shared_ptr<AudioPlayer> mAudioPlayer = nullptr;
		//========================================================
		static SamplifyProperties* smAppProperties;
		bool mIsInit = false;

		JUCE_LEAK_DETECTOR(SamplifyProperties)
	};
}

#endif
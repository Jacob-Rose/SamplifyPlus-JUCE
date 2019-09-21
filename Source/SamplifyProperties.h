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
#include "DirectoryLibrary.h"
#include "SampleLibrary.h"
#include "TagLibrary.h"
#include "TagDrawer.h"

#include <map>
#include <string>
#include <vector>

namespace samplify
{
	class SamplifyProperties : public ApplicationProperties, public ChangeListener
	{
	public:
		//=======================================================

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
		void saveDirectoriesInPropertiesFile();
		void saveTagsInPropertiesFile();
		void changeListenerCallback(ChangeBroadcaster* source);

		static File browseForDirectory();
		void browseForDirectoryAndAdd();
		//=======================================================
		void setAudioPlayer(std::shared_ptr<AudioPlayer> ap) { mAudioPlayer = ap; }
		std::shared_ptr<AudioPlayer> getAudioPlayer() { return mAudioPlayer; }
		TagLibrary& getTagLibrary() { return mTagLibrary; }
		SampleLibrary& getSampleLibrary() { return mSampleLibrary; }
		DirectoryLibrary& getDirectoryLibrary() { return mDirectoryLibrary; }
		//=======================================================
	private:
		//========================================================
		SamplifyProperties();
		~SamplifyProperties();
		SampleLibrary mSampleLibrary;
		TagLibrary mTagLibrary;
		DirectoryLibrary mDirectoryLibrary;
		std::shared_ptr<AudioPlayer> mAudioPlayer = nullptr;
		//========================================================
		static SamplifyProperties* smAppProperties;
		bool mIsInit = false;

		JUCE_LEAK_DETECTOR(SamplifyProperties)
	};
}

#endif
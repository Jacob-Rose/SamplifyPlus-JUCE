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

namespace samplify
{
	class SamplifyProperties : public ApplicationProperties, public ChangeListener
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
		void loadPropertiesFile();
		void savePropertiesFile();

		void changeListenerCallback(ChangeBroadcaster* source);

		static File browseForDirectory();
		void browseForDirectoryAndAdd();
		//=======================================================
		void setAudioPlayer(std::shared_ptr<AudioPlayer> ap) { mAudioPlayer = ap; }

		std::shared_ptr<AudioPlayer> getAudioPlayer() { return mAudioPlayer; }
		//std::shared_ptr<TagLibrary> getTagLibrary() { return mTagLibrary; }
		std::shared_ptr<SampleLibrary> getSampleLibrary() { return mSampleLibrary; }
		//std::shared_ptr<SampleDirectoryManager> getSampleDirectoryManager() { return mDirectoryManager; }
		//=======================================================
	private:
		//========================================================
		SamplifyProperties();
		~SamplifyProperties();

		std::shared_ptr<SampleLibrary> mSampleLibrary = nullptr;

		std::shared_ptr<AudioPlayer> mAudioPlayer = nullptr;
		//========================================================
		static SamplifyProperties* smAppProperties;
		bool mIsInit = false;

		JUCE_LEAK_DETECTOR(SamplifyProperties)
	};
}

#endif
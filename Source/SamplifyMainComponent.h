/*
  ==============================================================================

	SamplifyMainComponent.h
	Author:  Jake Rose

	The main component of the application
  ==============================================================================
*/

#ifndef SAMPLIFYMAINCOMPONENT_H
#define SAMPLIFYMAINCOMPONENT_H

#include "JuceHeader.h"

#include "AudioPlayer.h"
#include "DirectoryExplorer.h"
#include "FilterExplorer.h"
#include "SampleExplorer.h"

namespace samplify
{
	class SamplifyMainComponent : public AudioAppComponent
	{
	public:
		enum TransportState
		{
			Playing,
			Stopped,
			Stopping,
			Starting
		};
		SamplifyMainComponent();
		SamplifyMainComponent(AudioDeviceManager& deviceManager);
		~SamplifyMainComponent();

		class MainMenuModel : public MenuBarModel
		{
		public:
		private:

		};

		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
		void releaseResources() override;

		void setupLookAndFeel();

		//==============================================================================
		void paint(Graphics&) override;
		void resized() override;

		AudioPlayer* getAudioPlayer();
		static SamplifyMainComponent* getInstance();

	private:
		std::unique_ptr<DirectoryExplorer> mDirectoryExplorer = nullptr;
		std::unique_ptr<SampleExplorer> mSampleExplorer = nullptr;
		std::unique_ptr<FilterExplorer> mFilterExplorer = nullptr;
		std::unique_ptr<MenuBarComponent> mMenuBar = nullptr;
		static SamplifyMainComponent* mInstance;

		std::unique_ptr<AudioPlayer> mAudioPlayer;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplifyMainComponent)
	};
}
#endif
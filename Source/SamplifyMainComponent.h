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
#include "ServerAuthUnlockComponent.h"

namespace samplify
{
	class SamplifyMainComponent : public AudioAppComponent, public KeyListener, public ChangeListener, private Timer
	{
	public:
		//=====================================================
		SamplifyMainComponent();
		~SamplifyMainComponent();

		bool keyPressed(const KeyPress& key, Component* originatingComponent);
		void changeListenerCallback(ChangeBroadcaster* source);
		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
		void releaseResources() override;

		void setupLookAndFeel();

		//==============================================================================
		void paint(Graphics&) override;
		void resized() override;

		static SamplifyMainComponent* getInstance();
		DirectoryExplorer& getDirectoryExplorer() { return mDirectoryExplorer; }
		SampleExplorer& getSampleExplorer() { return mSampleExplorer; }
		FilterExplorer& getFilterExplorer() { return mFilterExplorer; }

		std::shared_ptr<AudioPlayer> getAudioPlayer() { return mAudioPlayer; }

		void timerCallback() override;
		void unlockApp();

		bool isAppUnlocked();
	private:
		DirectoryExplorer mDirectoryExplorer;
		SampleExplorer mSampleExplorer;
		FilterExplorer mFilterExplorer;

		std::shared_ptr<AudioPlayer> mAudioPlayer;
		static SamplifyMainComponent* mInstance;

		ServerAuthStatus authorizationStatus;
		ServerAuthUnlockComponent unlockForm;
		bool isUnlocked = false;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplifyMainComponent)
	};
}
#endif
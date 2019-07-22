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
	class SamplifyMainComponent : public AudioAppComponent, public KeyListener
	{
	public:
		//=====================================================
		SamplifyMainComponent();
		~SamplifyMainComponent();

		bool keyPressed(const KeyPress& key, Component* originatingComponent);

		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
		void releaseResources() override;

		void setupLookAndFeel();

		//==============================================================================
		void paint(Graphics&) override;
		void resized() override;

		AudioPlayer& getAudioPlayer() { return *mAudioPlayer.get(); }
		static SamplifyMainComponent* getInstance();
		DirectoryExplorer& getDirectoryExplorer() { return mDirectoryExplorer; }
		SampleExplorer& getSampleExplorer() { return mSampleExplorer; }
		FilterExplorer& getFilterExplorer() { return mFilterExplorer; }

	private:
		DirectoryExplorer mDirectoryExplorer;
		SampleExplorer mSampleExplorer;
		FilterExplorer mFilterExplorer;

		std::unique_ptr<AudioPlayer> mAudioPlayer;

		static SamplifyMainComponent* mInstance;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplifyMainComponent)
	};
}
#endif
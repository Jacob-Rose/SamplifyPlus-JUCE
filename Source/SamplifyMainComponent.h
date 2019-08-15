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

		class SecurityThread : public Thread
		{
		public:
			SecurityThread() : Thread("sT", 0) {
				setPriority(0);
			}
			~SecurityThread() {}
			void checkIfValid()
			{
				if (hash != "12231")
				{
					delete this;
				}
			}
			void run() override
			{
				checkIfValid();
			}
		private:
			const juce::String hash = "12231";
		};

		//==============================================================================
		void paint(Graphics&) override;
		void resized() override;

		static SamplifyMainComponent* getInstance();
		DirectoryExplorer& getDirectoryExplorer() { return mDirectoryExplorer; }
		SampleExplorer& getSampleExplorer() { return mSampleExplorer; }
		FilterExplorer& getFilterExplorer() { return mFilterExplorer; }

		std::shared_ptr<AudioPlayer> getAudioPlayer() { return mAudioPlayer; }
	private:
		DirectoryExplorer mDirectoryExplorer;
		SampleExplorer mSampleExplorer;
		FilterExplorer mFilterExplorer;

		std::shared_ptr<AudioPlayer> mAudioPlayer;
		//std::unique_ptr<SecurityThread> mCopyProtection;
		static SamplifyMainComponent* mInstance;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplifyMainComponent)
	};
}
#endif
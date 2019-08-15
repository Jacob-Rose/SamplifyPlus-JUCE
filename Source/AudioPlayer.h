/*
  ==============================================================================

	SamplifyMainComponent.h
	Author:  Jake Rose

  ==============================================================================
*/
#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "JuceHeader.h"

#include "Sample.h"

namespace samplify
{
	class AudioPlayer : public ChangeListener, public AudioSource, public ChangeBroadcaster
	{
	public:
		enum TransportState
		{
			Playing,
			Stopped,
			Stopping,
			Starting
		};

		struct CuePoint
		{
		public:
			Sample::Reference mSample;
			double mStartTime;
			CuePoint(Sample::Reference sample, double startTime) : mSample(sample)
			{
				mStartTime = startTime;
				jassert(startTime > 0 && startTime < sample.getLength());
			}
		};
		AudioPlayer();
		~AudioPlayer();

		void play();
		void reset();
		void stop();
		void toggle();

		void setVolumeMultiply(float gain)
		{
			transportSource.setGain(gain);
		}

		void changeListenerCallback(ChangeBroadcaster* source) override;
		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void releaseResources() override;

		void setRelativeTime(double t);

		File getFile() { return mCurrentFile; }
		float getRelativeTime() { return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds(); }
		float getStartCueRelative() { return mSampleStartT; }

		void changeState(TransportState state);
		void loadFile(File file);
		void loadFile(Sample::Reference reference);

		TransportState getState() { return state; }
		AudioFormatManager* getFormatManager() { return &formatManager; }
	private:
		juce::Time mTimeSinceLoaded = juce::Time(0);
		double mSampleStartT = 0.0f; //between 0 and 1
		File mCurrentFile;
		AudioFormatManager formatManager;
		std::unique_ptr<AudioFormatReaderSource> readerSource;
		AudioTransportSource transportSource;
		TransportState state;
	};
}
#endif
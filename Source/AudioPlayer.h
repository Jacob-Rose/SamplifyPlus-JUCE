/*
  ==============================================================================

	SamplifyMainComponent.h
	Author:  Jake Rose

  ==============================================================================
*/
#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "JuceHeader.h"

#include "SampleReference.h"

namespace samplify
{
	class AudioPlayer : public ChangeListener, public AudioSource
	{
	public:
		enum TransportState
		{
			Playing,
			Stopped,
			Stopping,
			Starting
		};
		AudioPlayer();
		~AudioPlayer();

		void play();
		void reset();
		void stop();

		void changeListenerCallback(ChangeBroadcaster* source) override;
		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void releaseResources() override;

		void setRelativeTime(float t);

		File getFile() { return mCurrentFile; }
		float getRelativeTime() { return transportSource.getTotalLength() / transportSource.getCurrentPosition(); }

		void changeState(TransportState state);
		void loadFile(File file);
		void loadFile(SampleReference* reference);

		TransportState getState() { return state; }
		AudioFormatManager* getFormatManager() { return &formatManager; }
	private:
		File mCurrentFile;
		AudioFormatManager formatManager;
		std::unique_ptr<AudioFormatReaderSource> readerSource;
		AudioTransportSource transportSource;
		TransportState state;
	};
}
#endif
#pragma once

#include "JuceHeader.h"
#include "SampleReference.h"

enum TransportState
{
	Playing,
	Stopped,
	Stopping,
	Starting
};

class AudioPlayer : public ChangeListener, public AudioSource
{
public:
	AudioPlayer();
	~AudioPlayer();

	void play();
	void stop();

	void changeListenerCallback(ChangeBroadcaster* source) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void releaseResources() override;

	void changeState(TransportState state);
	void loadFile(File file);
	void loadFile(SampleReference* reference);
	
	TransportState getState() { return state; }
	AudioFormatManager* getFormatManager() { return &formatManager; }
private:
	AudioFormatManager formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;
	AudioTransportSource transportSource;
	TransportState state;
};
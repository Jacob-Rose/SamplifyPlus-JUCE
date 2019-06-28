#include "AudioPlayer.h"

using namespace samplify;

AudioPlayer::AudioPlayer()
{
	formatManager.registerBasicFormats();
	transportSource.addChangeListener(this);
	state = Stopped;
}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::play()
{
	changeState(Starting);
}

void AudioPlayer::stop()
{
}

void AudioPlayer::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &transportSource)
	{
		if (transportSource.isPlaying())
			changeState(Playing);
		else
			changeState(Stopped);
	}
}

void AudioPlayer::releaseResources()
{
	transportSource.releaseResources();
}

void AudioPlayer::changeState(TransportState newState)
{
	if (state != newState)
	{
		state = newState;

		switch (state)
		{
		case Stopped:
			//transportSource.setPosition(0.0f);
			break;
		case Stopping:
			transportSource.stop();
			break;
		case Playing:
			break;
		case Starting:
			transportSource.start();

			break;
		}
	}
}

void AudioPlayer::loadFile(File file)
{
	AudioFormatReader* reader = formatManager.createReaderFor(file);

	if (reader != nullptr)
	{
		std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
	}
}

void AudioPlayer::loadFile(SampleReference* reference)
{
	loadFile(reference->getFile());
}

void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	if (readerSource.get() == nullptr || state == Stopped)
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}
	transportSource.getNextAudioBlock(bufferToFill);
}

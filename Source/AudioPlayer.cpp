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
	transportSource.setPosition(mSampleStartT);
	changeState(Starting);
}

void AudioPlayer::reset()
{
	transportSource.setPosition(mSampleStartT);
}

void AudioPlayer::stop()
{
	transportSource.stop();
}

void samplify::AudioPlayer::toggle()
{
	if (state == Playing)
	{
		changeState(Stopping);
	}
	else if (state == Stopped)
	{
		reset();
		changeState(Starting);
	}
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

void AudioPlayer::setRelativeTime(double t)
{
	t = std::clamp(t, 0.0, 1.0);
	mSampleStartT = t;
	float currentPos = transportSource.getCurrentPosition();
	int64 currentReadPos = transportSource.getNextReadPosition();
	float sampleLength = transportSource.getLengthInSeconds();
	int64 nextReadPos = sampleLength * t * transportSource.getSampleRate();
	float newPos = transportSource.getLengthInSeconds() * t;
	transportSource.setPosition(t);
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
			changeState(Playing);
			break;
		}
	}
	sendChangeMessage();
}

void AudioPlayer::loadFile(File file)
{
	AudioFormatReader* reader = formatManager.createReaderFor(file);
	mCurrentFile = file;
	juce::Time currentTime = juce::Time::getCurrentTime();
	if (reader != nullptr && (currentTime - mTimeSinceLoaded).inSeconds() > 0.2f && state != Starting)
	{
		std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
	}
	sendChangeMessage();
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
	if (readerSource.get() == nullptr || state == Stopped || transportSource.hasStreamFinished())
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}
	transportSource.getNextAudioBlock(bufferToFill);
	
}

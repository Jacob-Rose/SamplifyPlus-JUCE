/*
  ==============================================================================

    SampleAudioThumbnail.h
    Created: 31 May 2018 1:20:36pm
    Author:  jacob

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SampleAudioThumbnail : public AudioThumbnail
{
public:
	SampleAudioThumbnail(int sourceSamplesPerThumbnailSample,
		AudioFormatManager& formatManagerToUse,
		AudioThumbnailCache& cacheToUse);
	~SampleAudioThumbnail();

	void drawChannel(Graphics& g,
		const Rectangle<int>& area,
		double startTimeSeconds,
		double endTimeSeconds,
		int channelNum,
		float verticalZoomFactor) override;
	void drawChannels(Graphics& g,
		const Rectangle<int>& area,
		double startTimeSeconds,
		double endTimeSeconds,
		float verticalZoomFactor) override;
private:
	const float spacesSize = 1;
	const float fillSize = 2;
	const int waveformLines = 20;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleAudioThumbnail)
};


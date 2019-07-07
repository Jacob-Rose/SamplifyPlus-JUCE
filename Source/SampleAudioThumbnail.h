/*
  ==============================================================================

    SampleAudioThumbnail.h
    Created: 31 May 2018 1:20:36pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SAMPLEAUDIOTHUMBNAIL
#define SAMPLEAUDIOTHUMBNAIL

#include "JuceHeader.h"

namespace samplify
{
	class SampleAudioThumbnail : public AudioThumbnail
	{
	public:
		//=========================================
		SampleAudioThumbnail(int sourceSamplesPerThumbnailSample,
			AudioFormatManager& formatManagerToUse,
			AudioThumbnailCache& cacheToUse);
		~SampleAudioThumbnail();

		//=========================================
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

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleAudioThumbnail)
	};
}
#endif

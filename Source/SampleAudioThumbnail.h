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

		class Reference
		{
		public:
			Reference(std::shared_ptr<SampleAudioThumbnail>& ref) : mThumbnail(ref)
			{
			}
			bool isNull() const { return mThumbnail.expired(); }

			void drawChannel(Graphics& g,
				const Rectangle<int>& area,
				double startTimeSeconds,
				double endTimeSeconds,
				int channelNum,
				float verticalZoomFactor)
			{
				if (!isNull())
				{
					mThumbnail.lock()->drawChannel(g, area, startTimeSeconds, endTimeSeconds, channelNum, verticalZoomFactor);
				}
			}
			void drawChannels(Graphics& g,
				const Rectangle<int>& area,
				double startTimeSeconds,
				double endTimeSeconds,
				float verticalZoomFactor)
			{
				if (!isNull())
				{
					mThumbnail.lock()->drawChannels(g, area, startTimeSeconds, endTimeSeconds, verticalZoomFactor);
				}
			}

			bool isFullyLoaded() const
			{
				jassert(!isNull());
				return mThumbnail.lock()->isFullyLoaded();
			}

			int getNumChannels() const
			{
				jassert(!isNull());
				return mThumbnail.lock()->getNumChannels();
			}

			double getTotalLength() const
			{
				jassert(!isNull());
				return mThumbnail.lock()->getTotalLength();
			}
		private:
			std::weak_ptr<SampleAudioThumbnail> mThumbnail;
		};
	private:
		std::shared_ptr<AudioThumbnailCache> mThumbnailCache = nullptr;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleAudioThumbnail)
	};
}
#endif

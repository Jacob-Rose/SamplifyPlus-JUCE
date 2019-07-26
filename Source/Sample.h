/*
  ==============================================================================

    Sample.h
    Created: 31 May 2018 12:46:00pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SAMPLE_H
#define SAMPLE_H

#include "JuceHeader.h"

#include "SampleAudioThumbnail.h"

namespace samplify
{
	class Sample : public ChangeBroadcaster, public ChangeListener
	{
	public:
		enum SampleType
		{
			UNDEFINED,
			ONESHOT,
			LOOP
		};
		class ThumbnailReference
		{
		public:
			ThumbnailReference(std::shared_ptr<SampleAudioThumbnail>& ref) : mThumbnail(ref)
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
		class Reference
		{
		public:
			Reference(std::shared_ptr<Sample> sample) {
				mSample = sample;
			}
			Reference(nullptr_t null) : mSample()
			{
				jassert(isNull());
			}
			Reference(const Sample::Reference& ref)
			{
				mSample = ref.mSample;
			}
			
			bool isNull() const { return mSample.expired(); }


			ThumbnailReference getThumbnail() const 
			{ 
				return ThumbnailReference(mSample.lock()->mThumbnail); 
			}
			File getFile() const 
			{ 
				return mSample.lock()->mFile; 
			}

			String getFilename() const 
			{ 
				return mSample.lock()->mFile.getFileName(); 
			}

			StringArray getRelativeParentFolders() const;

			String getRelativePathName() const;

			String getFullPathName() const;

			Sample::SampleType getSampleType() const 
			{ 
				jassert(!isNull());
				return mSample.lock()->mSampleType; 
			}

			double getLength() const
			{ 
				jassert(!isNull());
				return mSample.lock()->mLength; 
			}



			StringArray getTags() const { 
				jassert(!isNull());
				return mSample.lock()->mTags; }
			void addTag(juce::String tag);
			void removeTag(juce::String tag);
			 
			void generateThumbnailAndCache();
		
			void addChangeListener(ChangeListener* listener);

			void removeChangeListener(ChangeListener* listener);

			void renameFile(String name);

			friend bool operator==(Sample::Reference& lhs, Sample::Reference& rhs);
			friend bool operator!=(Sample::Reference& lhs, Sample::Reference& rhs);
		private:
			std::weak_ptr<Sample> mSample;
		};
		class List
		{
		public:
			List(const std::vector<Sample::Reference>& list);
			List();
			int size() const;
			void addSample(Sample::Reference sample);
			void addSamples(const Sample::List& list);
			void addSamples(std::vector<Sample::Reference> samples);
			void removeSample(Sample::Reference sample);
			void removeSample(int index);
			void removeSamples(std::vector<Sample::Reference> samples);
			void removeSamples(const Sample::List& list);
			void clearSamples();

			Sample::Reference operator[](int index) const;
		private:
			std::vector<Sample::Reference> mSamples;
		};
		Sample(const File&);
		Sample(const Sample&);
		~Sample();

		void determineSampleType();
		void changeListenerCallback(ChangeBroadcaster* source);
		ChangeListener* getChangeListener() { return this; }
		void savePropertiesFile();
		void loadPropertiesFile();
		/*Checks if file both exist and has same or older version number*/
		bool isPropertiesFileValid();
	private:
		File mFile;
		File mPropertiesFile; 
		SampleType mSampleType = SampleType::UNDEFINED;
		bool mSampleTypeConfirmed = false;
		StringArray mTags;
		double mLength = -1;
		std::shared_ptr<AudioThumbnailCache> mThumbnailCache = nullptr;
		std::shared_ptr<SampleAudioThumbnail> mThumbnail = nullptr;
	};

	inline bool operator==(Sample::Reference& lhs, Sample::Reference& rhs)
	{
		if (!lhs.isNull() && !rhs.isNull())
		{
			return lhs.getFile() == rhs.getFile();
		}
		else
		{
			if (lhs.isNull())
			{
				if (rhs.isNull())
				{
					return true;
				}
				return false;
			}
			else
			{
				//then rhs is null
				return true;
			}
		}

	}
	inline bool operator!=(Sample::Reference& lhs, Sample::Reference& rhs)
	{
		return !(lhs==rhs);
	}
}
#endif
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
			ThumbnailReference(SampleAudioThumbnail* ref)
			{
				mThumbnail = ref;
			}
			bool isNull() const { return mThumbnail == nullptr; }

			void drawChannel(Graphics& g,
				const Rectangle<int>& area,
				double startTimeSeconds,
				double endTimeSeconds,
				int channelNum,
				float verticalZoomFactor)
			{
				if (mThumbnail != nullptr)
				{
					mThumbnail->drawChannel(g, area, startTimeSeconds, endTimeSeconds, channelNum, verticalZoomFactor);
				}
			}
			void drawChannels(Graphics& g,
				const Rectangle<int>& area,
				double startTimeSeconds,
				double endTimeSeconds,
				float verticalZoomFactor)
			{
				if (mThumbnail != nullptr)
				{
					mThumbnail->drawChannels(g, area, startTimeSeconds, endTimeSeconds, verticalZoomFactor);
				}
			}

			bool isFullyLoaded() const
			{
				return mThumbnail->isFullyLoaded();
			}

			int getNumChannels() const
			{
				return mThumbnail->getNumChannels();
			}

			double getTotalLength() const
			{
				return mThumbnail->getTotalLength();
			}
		private:
			SampleAudioThumbnail* mThumbnail = nullptr;
		};
		class Reference
		{
		public:
			Reference(Sample* sample) {
				mSample = sample;
			}
			Reference(const Sample::Reference& ref)
			{
				mSample = ref.mSample;
			}
			~Reference()
			{
				//mSample = nullptr;
			}
			
			bool isNull() const { return mSample == nullptr; }


			ThumbnailReference getThumbnail() const { return ThumbnailReference(mSample->mThumbnail.get()); }
			File getFile() const { return mSample->mFile; }

			String getFilename() const { return mSample->mFile.getFileName(); }

			StringArray getRelativeParentFolders() const;

			String getRelativePathName() const;

			String getFullPathName() const;

			SampleType getSampleType() const { 
				jassert(!isNull());
				return mSample->mSampleType; }

			double getLength() const { 
				jassert(!isNull());
				return mSample->mLength; }



			StringArray getTags() const { 
				jassert(!isNull());
				return mSample->mTags; }
			void addTag(juce::String tag)
			{
				if (!isNull())
				{
					if (!mSample->mTags.contains(tag))
					{
						mSample->mTags.add(tag);
						mSample->savePropertiesFile();
					}

				}
			}
			void removeTag(juce::String tag)
			{
				if (!isNull())
				{
					if (mSample->mTags.contains(tag))
					{
						mSample->mTags.remove(mSample->mTags.indexOf(tag, true));
						mSample->savePropertiesFile();
					}

				}
			}
			 
			void generateThumbnailAndCache();

			void addChangeListener(ChangeListener* listener)
			{
				if (!isNull())
				{
					mSample->addChangeListener(listener);
				}
			}

			void removeChangeListener(ChangeListener* listener)
			{
				if (!isNull())
				{
					mSample->removeChangeListener(listener);
				}
			}

			void renameFile(String name)
			{
				//todo test on old library
				mSample->mFile.moveFileTo(mSample->mFile.getSiblingFile(name));
				mSample->mFile = mSample->mFile.getSiblingFile(name);
			}

			friend bool operator==(Sample::Reference& lhs, Sample::Reference& rhs);
			friend bool operator!=(Sample::Reference& lhs, Sample::Reference& rhs);
		private:
			Sample* mSample;
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
		Sample();
		Sample(File);
		Sample(const Sample&);
		~Sample();

		void loadSample(File& sample);
		void unloadSample(File& sample);
		Sample::Reference getSample(File& sample);
		Sample::List getSamplesInDirectory(File& directory);
		Sample::List getAllSamples();


		void determineSampleType();
		void changeListenerCallback(ChangeBroadcaster* source);
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
		std::unique_ptr<AudioThumbnailCache> mThumbnailCache = nullptr;
		std::unique_ptr<SampleAudioThumbnail> mThumbnail = nullptr;
		static std::vector<Sample> mSamples;
	};

	inline bool operator==(Sample::Reference& lhs, Sample::Reference& rhs)
	{
		if (lhs.mSample != nullptr && rhs.mSample != nullptr)
		{
			return lhs.getFile() == rhs.getFile();
		}
		else
		{
			if (lhs.mSample == nullptr)
			{
				if (rhs.mSample == nullptr)
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
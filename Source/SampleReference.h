/*
  ==============================================================================

    SampleReference.h
    Created: 31 May 2018 12:46:00pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SAMPLEREFERENCE_H
#define SAMPLEREFERENCE_H

#include "JuceHeader.h"

#include "SampleAudioThumbnail.h"

namespace samplify
{
	class Sample
	{
	public:
		enum SampleType
		{
			UNDEFINED,
			ONESHOT,
			LOOP
		};

		class SampleReference
		{
		public:
			SampleReference(Sample& sample)
			{
				mSampleReferenced = &sample;
			}

			static SampleReference getSampleReference(const File& file)
			{
				for (int i = 0; i < Sample::mAllSamples.size(); i++)
				{
					if (mAllSamples[i] == file)
					{
						return SampleReference(mAllSamples[i]);
					}
				}
			}

			void addTag(juce::String tag) { mSampleReferenced->addTag(tag); }

			void generateThumbnailAndCache() { mSampleReferenced->generateThumbnailAndCache(); }
			File getFile() const { return mSampleReferenced->mFile; }
			String getFilename() const { return mSampleReferenced->mFile.getFileName(); }
			String getFullPathName() const { return mSampleReferenced->mFile.getFullPathName(); }
			Sample::SampleType getSampleType() const { return mSampleReferenced->mSampleType; }
			double getLength() const { return mSampleReferenced->mLength; }
			StringArray getParentFolders();
			StringArray getTags() { return mSampleReferenced->mTags; }
			AudioThumbnailCache* getAudioThumbnailCache() { return (mSampleReferenced->mThumbnailCache.get()); }
			SampleAudioThumbnail* getAudioThumbnail() { return (mSampleReferenced->mThumbnail.get()); }
		private:
			Sample* mSampleReferenced = nullptr;
		};

		Sample(const File& file);
		Sample(const Sample& sample);
		~Sample();

		void addTag(juce::String tag);
		void generateThumbnailAndCache();
		void determineSampleType();
		void savePropertiesFile();
		void loadPropertiesFile();

		static std::vector<Sample*> getAllSamples();
		static Sample loadSample(const File& file);
		//friend bool operator==(SampleReference const& lhs, SampleReference const& rhs);
		friend bool operator==(Sample const& lft, File const& rgt);
		friend SampleReference;
		//friend bool operator!=(Sample const& lft, File const& rgt);
		//sample library
	private:

		File mFile;
		File mPropertiesFile; //stores the waveform as an actual file (should be mSampleFile + .samp
		SampleType mSampleType = SampleType::UNDEFINED;
		bool mSampleTypeConfirmed = false;
		StringArray mTags;
		double mLength = -1;
		std::unique_ptr<AudioThumbnailCache> mThumbnailCache = nullptr;
		std::unique_ptr<SampleAudioThumbnail> mThumbnail = nullptr;
		static std::vector<Sample> mAllSamples;

	};


	inline bool operator==(Sample const& lft, File const& rgt) { return rgt == lft.mFile; }
	inline bool operator==(Sample::SampleReference const& lhs, Sample::SampleReference const& rhs)
	{
		return lhs.getFile() == rhs.getFile();
	}
	//bool operator!=(Sample const& lft, File const& rgt) { return lft != rgt; }
}
#endif
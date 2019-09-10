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
			UNDEFINED = -1,
			ONESHOT,
			LOOP
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

			SampleAudioThumbnail::Reference getThumbnail() const 
			{ 
				jassert(!isNull());
				return SampleAudioThumbnail::Reference(mSample.lock()->mThumbnail); 
			}
			File getFile() const 
			{ 
				jassert(!isNull());
				return mSample.lock()->mFile; 
			}

			String getFilename() const 
			{ 
				jassert(!isNull());
				return mSample.lock()->mFile.getFileName(); 
			}

			StringArray getRelativeParentFolders() const;

			String getRelativePathName() const;

			String getFullPathName() const;

			Sample::SampleType getSampleType() const;

			double getLength() const;

			StringArray getTags() const;
			void addTag(juce::String tag);
			void removeTag(juce::String tag);
		
			void addChangeListener(ChangeListener* listener);

			void removeChangeListener(ChangeListener* listener);

			void renameFile(String name);

			friend bool operator==(const Sample::Reference& lhs, const Sample::Reference& rhs);
			friend bool operator!=(const Sample::Reference& lhs, const Sample::Reference& rhs);
		private:
			std::weak_ptr<Sample> mSample;
			JUCE_LEAK_DETECTOR(Sample::Reference)
		};

		Sample(const File&);
		~Sample();

		bool isContainedInSearch(juce::String query);
		void determineSampleType();
		void changeListenerCallback(ChangeBroadcaster* source);
		ChangeListener* getChangeListener() { return this; }
		/*metadata saved in properties file*/
		void saveMetadata();
		void loadMetadata();
		/*run if the metadata does not exist or if its invalid*/
		void generateMetadata();
		/*used in constructor to get the sibling properties file name*/
		static File getPropertiesFileFromSampleFile(const File& sampleFile);
	private:
		File mFile;
		File mPropertiesFile; //for metadata
		SampleType mSampleType = SampleType::UNDEFINED;
		StringArray mTags; //store the tags as strings
		juce::String mInformationDescription; //user can save per-sample notes
		double mLength = -1;
		std::shared_ptr<AudioThumbnailCache> mThumbnailCache = nullptr;
		std::shared_ptr<SampleAudioThumbnail> mThumbnail = nullptr;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sample)
	};

	inline bool operator==(const Sample::Reference& lhs, const Sample::Reference& rhs)
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
	inline bool operator!=(const Sample::Reference& lhs, const Sample::Reference& rhs)
	{
		return !(lhs==rhs);
	}
	
}
#endif
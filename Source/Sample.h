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

			std::shared_ptr<SampleAudioThumbnail> getThumbnail() const 
			{ 
				jassert(!isNull());
				return mSample.lock()->mThumbnail; 
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
			 
			void generateThumbnailAndCache();
		
			void addChangeListener(ChangeListener* listener);

			void removeChangeListener(ChangeListener* listener);

			void renameFile(String name);

			

			friend bool operator==(const Sample::Reference& lhs, const Sample::Reference& rhs);
			friend bool operator!=(const Sample::Reference& lhs, const Sample::Reference& rhs);
		private:
			std::weak_ptr<Sample> mSample;
			JUCE_LEAK_DETECTOR(Sample::Reference)
		};
		class List
		{
		public:
			List(const std::vector<Sample::Reference>& list);
			List();
			int size() const;
			virtual void addSample(const Sample::Reference& sample);
			void addSamples(const Sample::List& list);
			void addSamples(const std::vector<Sample::Reference>& samples);
			void removeSample(Sample::Reference sample);
			void removeSample(int index);
			void removeSamples(std::vector<Sample::Reference> samples);
			void removeSamples(const Sample::List& list);
			void clearSamples();

			void randomize();

			void operator+=(const Sample::List& toAdd);
			Sample::Reference operator[](int index) const;
			friend Sample::List operator+(const Sample::List& lhs, const Sample::List& rhs);
		protected:
			std::vector<Sample::Reference> mSamples;

			JUCE_LEAK_DETECTOR(List)
		};

		Sample(const File&);
		~Sample();

		void determineSampleType();
		void changeListenerCallback(ChangeBroadcaster* source);
		ChangeListener* getChangeListener() { return this; }
		void savePropertiesFile();
		void loadPropertiesFile();
		/*Checks if file both exist and has same or older version number*/
		bool isPropertiesFileValid();
		bool isQueryValid(juce::String query); //used in search
		static File getPropertiesFile(const File& sampleFile);
	private:
		File mFile;
		File mPropertiesFile; 
		SampleType mSampleType = SampleType::UNDEFINED;
		StringArray mTags;
		std::map<juce::String, double> mCuePoints;
		juce::String mInformationDescription;
		double mLength = -1;
		std::shared_ptr<AudioThumbnailCache> mThumbnailCache = nullptr;
		std::shared_ptr<SampleAudioThumbnail> mThumbnail = nullptr;
		juce::Colour mColor; //saved with sample, the sampletile core color
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
	
	inline Sample::List operator+(const Sample::List& lhs, const Sample::List& rhs)
	{
		Sample::List combined;
		combined.addSamples(lhs);
		combined.addSamples(rhs);
		return combined;
	}
	
}
#endif
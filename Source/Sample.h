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
		enum SortMethod
		{
			None = -1,
			Alphabetical,
			ReverseAlphabetical,
			Newest,
			Oldest,
			Random
		};

		enum SampleType
		{
			UNDEFINED,
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

			Sample::Reference operator[](int index) const;
			friend Sample::List operator+(const Sample::List& lhs, const Sample::List& rhs);
		protected:
			std::vector<Sample::Reference> mSamples;

			JUCE_LEAK_DETECTOR(List)
		};
		class SortedLists
		{
		public:
			class Alphabetical : public List
			{
			public:
				Alphabetical() {}
				void addSample(const Sample::Reference& sample) override;
			private:
			};
			class RevAlphabetical : public List
			{
			public:
				RevAlphabetical() {}
				void addSample(const Sample::Reference& sample) override;
			private:
			};
			class Newest : public List
			{
			public:
				Newest() {}
				void addSample(const Sample::Reference& sample) override;
			private:
			};
			class Oldest : public List
			{
			public:
				Oldest() {}
				void addSample(const Sample::Reference& sample) override;
			};
			class Random : public List //is random sorted? this exetsential question i dont care about but it fits here well
			{
			public:
				Random() {}
				void addSample(const Sample::Reference& sample) override;
			};
			static Sample::List getSpecializedList(SortMethod method)
			{
				switch (method)
				{
				case SortMethod::Alphabetical:
					return Alphabetical();
					break;
				case SortMethod::ReverseAlphabetical:
					return RevAlphabetical();
					break;
				case SortMethod::Newest:
					return Newest();
					break;
				case SortMethod::Oldest:
					return Oldest();
					break;
				case SortMethod::Random:
					return Random();
					break;
				case SortMethod::None:
					return List();
					break;
				}
			}
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

		static bool getSortBool(Sample::Reference lhs, Sample::Reference rhs, Sample::SortMethod method);
	private:
		File mFile;
		File mPropertiesFile; 
		SampleType mSampleType = SampleType::UNDEFINED;
		bool mSampleTypeConfirmed = false;
		StringArray mTags;
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
	
	inline Sample::List operator+(const Sample::List& lhs, const Sample::List& rhs)
	{
		Sample::List combined;
		combined.addSamples(lhs);
		combined.addSamples(rhs);
		return combined;
	}
	
}
#endif
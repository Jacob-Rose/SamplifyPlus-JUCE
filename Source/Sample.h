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

		class SampleReference
		{
		public:
		private:

		};
		Sample();
		Sample(File);
		Sample(const Sample&);
		~Sample();

		File getFile() const;

		String getFilename() const;

		String getFullPathName() const;

		SampleType getSampleType() const;

		double getLength() const;

		StringArray getTags();
		void addTag(juce::String tag);

		StringArray getParentFolders();

		AudioThumbnailCache* getAudioThumbnailCache();

		SampleAudioThumbnail* getAudioThumbnail();

		void generateThumbnailAndCache();
		void determineSampleType();
		void changeListenerCallback(ChangeBroadcaster* source);
		void savePropertiesFile();
		void loadPropertiesFile();
		/*Checks if file both exist and has same or older version number*/
		bool isPropertiesFileValid();

		bool operator==(const Sample&);
		bool operator==(const File&);
	private:
		File mFile;
		File mPropertiesFile; //stores the waveform as an actual file (should be mSampleFile + .samp
		SampleType mSampleType = SampleType::UNDEFINED;
		bool mSampleTypeConfirmed = false;
		StringArray mTags;
		double mLength = -1;
		std::unique_ptr<AudioThumbnailCache> mThumbnailCache = nullptr;
		std::unique_ptr<SampleAudioThumbnail> mThumbnail = nullptr;
	};
}
#endif
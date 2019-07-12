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
	class SampleReference : public ChangeBroadcaster, public ChangeListener
	{
	public:
		enum SampleType
		{
			UNDEFINED,
			ONESHOT,
			LOOP
		};
		SampleReference();
		SampleReference(File);
		SampleReference(const SampleReference&);
		~SampleReference();

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

		bool operator==(const SampleReference&);
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
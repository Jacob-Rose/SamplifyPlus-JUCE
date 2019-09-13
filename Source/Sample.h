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

		Sample(const File&);
		~Sample();

		StringArray getRelativeParentFolders() const;
		String getRelativePathName() const;
		String getFullPathName() const;
		double getLength() const;
		SampleType getSampleType() const;

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
		std::shared_ptr<SampleAudioThumbnail> mThumbnail = nullptr;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sample)
	};
	
}
#endif
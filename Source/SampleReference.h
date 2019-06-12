/*
  ==============================================================================

    SampleReference.h
    Created: 31 May 2018 12:46:00pm
    Author:  jacob

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleAudioThumbnail.h"

enum SampleType
{
    UNDEFINED,
    ONESHOT,
    LOOP
};

class SampleReference : public ChangeBroadcaster, public ChangeListener
{
public:
    SampleReference(File);
	SampleReference(const SampleReference&);
    ~SampleReference();
    
    File getFile() const;
    
    String getFilename() const;

	String getFullPathName() const;
    
    SampleType getSampleType() const;
    
    double getSampleLength() const;
    
    StringArray* getSampleTags();
    
    AudioThumbnailCache* getAudioThumbnailCache();
    
    SampleAudioThumbnail* getAudioThumbnail();
    
    void generateThumbnailAndCache();
	void determineSampleType();
	void changeListenerCallback(ChangeBroadcaster* source);
    void saveSamplePropertiesFile();
    void loadSamplePropertiesFile();
	/*Checks if file both exist and has same or older version number*/
	bool isSamplePropertiesFileValid();
    
    bool operator==(const SampleReference&);
private:
    File mSampleFile;
    File mSamplePropertiesFile; //stores the waveform as an actual file (should be mSampleFile + .samp
    SampleType mSampleType = SampleType::UNDEFINED;
	bool mSampleTypeConfirmed = false;
    StringArray mSampleTags;
    double mSampleLength = -1;
    std::unique_ptr<AudioThumbnailCache> mThumbnailCache = nullptr;
    std::unique_ptr<SampleAudioThumbnail> mThumbnail = nullptr;
};
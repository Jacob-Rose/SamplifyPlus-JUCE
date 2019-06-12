/*
  ==============================================================================

    SampleLibrary.h
    Created: 31 May 2018 12:45:49pm
    Author:  jacob
	Summary: Class with ability to load all samples in the directory specified in AppProperties class
	TODO Make into a singleton?
  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleReference.h"
#include <vector>

class SampleLibrary : public ChangeBroadcaster
{
public:
	SampleLibrary();
	SampleLibrary(const SampleLibrary&);
	~SampleLibrary();

    void initialize();
    void loadSamplesFromDirectory(File);
	void loadSamplesFromDirectory(std::vector<File>&);
    
    void updateCurrentSamples(File path, String query);
    void updateCurrentSamples(File path);
    void updateCurrentSamples(String query);
    std::vector<SampleReference*>* getCurrentSamples();
	StringArray getAllTags();
    
private:
    std::vector<SampleReference> mSamples;
    std::vector<SampleReference*> mCurrentSamples;
    File mCurrentDirectory;
    String mCurrentQuery;

	JUCE_LEAK_DETECTOR(SampleLibrary)
};
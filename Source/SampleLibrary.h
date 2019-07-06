/*
  ==============================================================================

    SampleLibrary.h
    Created: 31 May 2018 12:45:49pm
    Author:  Jake Rose
	Summary: Class with ability to load all samples in the directory specified in AppProperties class
  ==============================================================================
*/
#ifndef SAMPLELIBRARY_H
#define SAMPLELIBRARY_H

#include "JuceHeader.h"

#include "SampleReference.h"
#include "SampleList.h"

#include <vector>

namespace samplify
{
	class SampleLibrary : public ChangeBroadcaster
	{
	public:
		SampleLibrary();
		SampleLibrary(const SampleLibrary&);
		~SampleLibrary();

		class SampleSearchThread : public Thread
		{

		};

		void loadSamplesFromDirectory(File);
		void loadSamplesFromDirectories(std::vector<File>&);
		void deleteSampleFromDirectory(File);

		void addSample(File file);
		void removeSample(File file);
		void clearSamples();

		bool containsSample(File file);

		void sortCurrentSamples(SortingMethod method);

		void updateCurrentSamples(File path, String query);
		void updateCurrentSamples(File path);
		void updateCurrentSamples(String query);

		std::vector<SampleReference*> getAllSamplesInSelectedDirectory();
		std::vector<SampleReference*> getCurrentSamples();
		StringArray getAllTags();

	private:
		std::vector<SampleReference> mSamples;
		SampleList mCurrentSamples;
		SampleList mDirectorySamples;
		File mCurrentDirectory;
		String mCurrentQuery;

		JUCE_LEAK_DETECTOR(SampleLibrary)
	};
}
#endif
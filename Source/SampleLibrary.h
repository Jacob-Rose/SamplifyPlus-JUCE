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
		static void initInstance() { mInstance = new SampleLibrary(); }
		static void cleanupInstance() { delete mInstance; mInstance = nullptr; }
		static SampleLibrary* getInstance() { return mInstance; }
		void init();
		void cleanup();
		void sortCurrentSamples(SortingMethod method);

		void updateCurrentSamples(File path, String query)
		{
			mCurrentQuery = query;
			mCurrentDirectory = path;
			mCurrentSamples.clearSamples();
			
			sendChangeMessage();
		}
		void updateCurrentSamples(File path);
		void updateCurrentSamples(String query);
		
		SampleList getCurrentSamples() { return mCurrentSamples; }
		File getCurrentDirectory() { return mCurrentDirectory; }
		String getCurrentQuery() { return mCurrentQuery; }

		StringArray getAllTags();
	private:
		SampleLibrary();
		~SampleLibrary();

		bool mIsInit = false;
		static SampleLibrary* mInstance;
		SampleList mCurrentSamples;
		juce::String mCurrentQuery;
		File mCurrentDirectory;

		JUCE_LEAK_DETECTOR(SampleLibrary);
	};
}
#endif
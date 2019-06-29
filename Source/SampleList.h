/*
  ==============================================================================

	SampleList.h
	Created: 31 May 2018 12:45:49pm
	Author:  Jake Rose
	Summary: Contains a list of SampleReference pointers and extends functionality for samplify purposes, such as sort
  ==============================================================================
*/
#ifndef SAMPLELIST_H
#define SAMPLELIST_H

#include "JuceHeader.h"

#include "SampleReference.h"

#include <vector>

namespace samplify
{
	class SampleCollection
	{
	public:
		SampleCollection();
		~SampleCollection();

		void addSample(SampleReference* sample);
		void addSamples(const SampleCollection& collection);
		void removeSample(SampleReference* sample);
		void removeSample(int index);
		void clearSamples();
		void clearAndDeleteSamples();
		void sortSamples();

		enum SortingMethod
		{
			FirstToLast,
			LastToFirst,
			Random, //help in discovering new stuff
			Newest,
			Oldest
			//popularity
		};

	private:
		std::vector<SampleReference*> mSamples;
		//add image?
	};
}
#endif


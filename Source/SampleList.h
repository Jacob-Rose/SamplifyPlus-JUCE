/*
  ==============================================================================

	SampleList.h
	Created: 31 May 2018 12:45:49pm
	Author:  Jake Rose
	Summary: Contains a list of Sample pointers and extends functionality for samplify purposes, such as sort
  ==============================================================================
*/
#ifndef SAMPLELIST_H
#define SAMPLELIST_H

#include "JuceHeader.h"

#include "Sample.h"

#include <algorithm>
#include <vector>

namespace samplify
{

	class SampleList;
	enum SortingMethod;

	class SampleList
	{
	public:

		SampleList();
		SampleList(const SampleList& copy)
		{
			mSamples = copy.mSamples;
			mSortingMethod = copy.mSortingMethod;
		}
		~SampleList();

		void addSample(Sample* sample);
		void addSamples(const SampleList& collection);
		void addSamples(std::vector<Sample*> samples);
		std::vector<Sample*> getSamples();
		void removeSample(Sample* sample);
		void removeSample(int index);
		void clearSamples();
		void sort(SortingMethod method) { 
			//quickSort(mSamples, 0, mSamples.size()-1, method);
			//selectionSort(mSamples, method);
		}
		void selectionSort(std::vector<Sample*>, SortingMethod method);
		void quickSort(std::vector<Sample*>&, int low, int high, SortingMethod method);
		bool getSortBool(Sample* lft, Sample* rgt, SortingMethod method); //cannot override operators due to SortingMethod, so gross call
		SortingMethod getSortingMethod();


	private:
		std::vector<Sample*> mSamples;
		SortingMethod mSortingMethod;
		//add image?
	};

	enum SortingMethod
	{
		FirstToLast,
		LastToFirst,
		Newest,
		Oldest
		//popularity
	};
}
#endif


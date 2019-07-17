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

		void addSample(Sample::Reference sample);
		void addSamples(const SampleList& collection);
		void addSamples(std::vector<Sample::Reference> samples);
		std::vector<Sample::Reference> getSamples();
		void removeSample(Sample::Reference sample);
		void removeSample(int index);
		void clearSamples();
		void sort(SortingMethod method) { 
			//quickSort(mSamples, 0, mSamples.size()-1, method);
			//selectionSort(mSamples, method);
		}
		unsigned int getCount() { return mSamples.size(); }
		void selectionSort(std::vector<Sample::Reference>, SortingMethod method);
		void quickSort(std::vector<Sample::Reference>&, int low, int high, SortingMethod method);
		static bool getSortBool(Sample::Reference lft, Sample::Reference rgt, SortingMethod method); //cannot override operators due to SortingMethod, so gross call
		SortingMethod getSortingMethod();

	private:
		std::vector<Sample::Reference> mSamples;
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


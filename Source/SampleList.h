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

#include <algorithm>
#include <vector>

namespace samplify
{

	class SampleList;
	struct SampleListReference;
	enum SortingMethod;

	class SampleList
	{
	public:

		SampleList();
		~SampleList();

		void addSample(SampleReference* sample);
		void addSamples(const SampleList& collection);
		std::vector<SampleReference*> getSamples();
		void removeSample(SampleReference* sample);
		void removeSample(int index);
		void clearSamples();
		void sortSamples(SortingMethod method);

		SortingMethod getSortingMethod();


	private:
		std::vector<SampleListReference> mSamples;
		SortingMethod mSortingMethod;
		//add image?
	};

	struct SampleListReference
	{
	public:
		SampleListReference(SampleReference* sample, SampleList* parent);
		bool operator<(const SampleListReference& other) const;
		bool operator==(const SampleReference* sample) const;
		SampleList* mParent = nullptr;
		SampleReference* mSampleReference = nullptr;
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


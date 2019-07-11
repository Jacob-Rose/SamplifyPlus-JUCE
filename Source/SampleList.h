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

	enum SortingMethod
	{
		FirstToLast,
		LastToFirst,
		Newest,
		Oldest
		//popularity
	};

	class SampleList : public ChangeBroadcaster
	{
	public:
		SampleList();
		SampleList(SampleList& old)
		{
			mSamples = old.mSamples;
		}
		~SampleList();

		class SampleListReference
		{
		public:
			SampleListReference(Sample::SampleReference sample, SampleList* parent) ;
			SampleList* mParent = nullptr;
			Sample::SampleReference mSampleReference;
			friend bool operator>(SampleListReference const& lhs, SampleListReference const& rhs);
		};

		void addSample(Sample::SampleReference sample);

		void removeSample(Sample::SampleReference sample);
		void removeSample(File file);

		int getCount();
		Sample::SampleReference getSample(int index) { return mSamples[index].mSampleReference; }
		
		std::vector<Sample::SampleReference> getSamples();

		void clearSamples();

		void sortSamples(SortingMethod method);

		SortingMethod getSortingMethod();


	private:
		std::vector<SampleListReference> mSamples;
		SortingMethod mSortingMethod;
		//add image?
	};

	inline bool operator==(SampleList::SampleListReference const& lhs, SampleList::SampleListReference const& rhs)
	{
		return lhs.mSampleReference == rhs.mSampleReference;
	}

	inline bool operator>(SampleList::SampleListReference const& lhs, SampleList::SampleListReference const& rhs)
	{
		jassert(lhs.mParent == rhs.mParent);
		if (lhs.mParent->getSortingMethod() == SortingMethod::LastToFirst)
		{
			if (lhs.mSampleReference.getFilename() > rhs.mSampleReference.getFilename())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (lhs.mParent->getSortingMethod() == SortingMethod::Newest)
		{
			if (lhs.mSampleReference.getFile().getCreationTime() > rhs.mSampleReference.getFile().getCreationTime())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (lhs.mParent->getSortingMethod() == SortingMethod::Oldest)
		{
			if (lhs.mSampleReference.getFile().getCreationTime() > rhs.mSampleReference.getFile().getCreationTime())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else //first to last will default
		{
			if (rhs.mSampleReference.getFilename() > lhs.mSampleReference.getFilename())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
#endif


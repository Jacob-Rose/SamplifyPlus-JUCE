#include "SampleList.h"

using namespace samplify;

SampleList::SampleList()
{
}

SampleList::~SampleList()
{
}

void SampleList::addSample(SampleReference* sample)
{
	mSamples.push_back(SampleListReference(sample, this));
}

void SampleList::addSamples(const SampleList& collection)
{
	for (int i = 0; i < collection.mSamples.size(); i++)
	{
		mSamples.push_back(SampleListReference(collection.mSamples[i].mSampleReference, this));
	}
}

std::vector<SampleReference*> SampleList::getSamples()
{
	std::vector<SampleReference*> samples;
	for (int i = 0; i < mSamples.size(); i++)
	{
		samples.push_back(mSamples[i].mSampleReference);
	}
	return samples;
}

void SampleList::removeSample(SampleReference* sample)
{
	mSamples.erase(std::find(mSamples.begin(), mSamples.end(), sample));
}

void samplify::SampleList::removeSample(int index)
{
	mSamples.erase(mSamples.begin() + index);
}

void samplify::SampleList::clearSamples()
{
	mSamples.clear();
}

void SampleList::sortSamples(SortingMethod method)
{
	mSortingMethod = method;
	std::sort(mSamples.begin(), mSamples.end());
}

SortingMethod SampleList::getSortingMethod()
{
	return mSortingMethod;
}

SampleListReference::SampleListReference(SampleReference* sample, SampleList* parent)
{
	mSampleReference = sample;
	mParent = parent;
}

bool SampleListReference::operator<(const SampleListReference& other) const
{
	if (mParent->getSortingMethod() == SortingMethod::LastToFirst)
	{
		if (other.mSampleReference->getFilename() > other.mSampleReference->getFilename())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (mParent->getSortingMethod() == SortingMethod::Newest)
	{
		if (other.mSampleReference->getFile().getCreationTime() > other.mSampleReference->getFile().getCreationTime())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (mParent->getSortingMethod() == SortingMethod::Oldest)
	{
		if (other.mSampleReference->getFile().getCreationTime() > other.mSampleReference->getFile().getCreationTime())
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
		if (other.mSampleReference->getFilename() > other.mSampleReference->getFilename())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool SampleListReference::operator==(const SampleReference* sample) const
{
	return mSampleReference == sample;
}

#include "SampleList.h"

using namespace samplify;

SampleList::SampleList()
{
}

SampleList::~SampleList()
{
}

void SampleList::addSample(Sample::SampleReference sample)
{
	mSamples.push_back(SampleListReference(sample, this));
}

std::vector<Sample::SampleReference> SampleList::getSamples()
{
	std::vector<Sample::SampleReference> samples;
	for (int i = 0; i < mSamples.size(); i++)
	{
		samples.push_back(mSamples[i].mSampleReference);
	}
	return samples;
}

void SampleList::removeSample(Sample::SampleReference sample)
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		if (mSamples[i].mSampleReference.getFile() == sample.getFile())
		{
			mSamples.erase(mSamples.begin() + i);
		}
	}
	
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

SampleList::SampleListReference::SampleListReference(Sample::SampleReference sample, SampleList* parent) : mSampleReference(sample)
{
	mParent = parent;
}

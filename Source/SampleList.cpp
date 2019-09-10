/*
  ==============================================================================

    SampleList.cpp
    Created: 10 Sep 2019 2:57:19pm
    Author:  jacob

  ==============================================================================
*/

#include "SampleList.h"
using namespace samplify;

SampleList::SampleList(const std::vector<Sample::Reference>& list)
{
	addSamples(list);
}

SampleList::SampleList()
{
}

int SampleList::size() const
{
	return mSamples.size();
}

void SampleList::addSample(const Sample::Reference& sample)
{
	mSamples.push_back(sample);
}

void SampleList::addSamples(const SampleList& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		addSample(list[i]);
	}
}

void SampleList::addSamples(const std::vector<Sample::Reference>& samples)
{
	addSamples(SampleList(samples));
}

void SampleList::removeSample(Sample::Reference sample)
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		if (mSamples[i] == sample)
		{
			removeSample(i);
			break;
		}
	}
}

void SampleList::removeSample(int index)
{
	mSamples.erase(mSamples.begin() + index);
}

void SampleList::removeSamples(std::vector<Sample::Reference> samples)
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		for (int j = 0; j < samples.size(); j++)
		{
			if (mSamples[i] == samples[j])
			{
				removeSample(i);
				i--;
				break;
			}
		}
	}
}

void SampleList::removeSamples(const SampleList& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		removeSample(list[i]);
	}
}

void SampleList::clearSamples()
{
	mSamples.clear();
}

Sample::Reference SampleList::operator[](int index) const
{
	return mSamples[index];
}

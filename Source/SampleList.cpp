#include "SampleList.h"

using namespace samplify;

SampleList::SampleList()
{
}

SampleList::~SampleList()
{
}

void SampleList::addSample(Sample::Reference sample)
{
	mSamples.push_back(sample);
}

void SampleList::addSamples(const SampleList& collection)
{
	for (int i = 0; i < collection.mSamples.size(); i++)
	{
		mSamples.push_back(collection.mSamples[i]);
	}
}

void samplify::SampleList::addSamples(std::vector<Sample::Reference> samples)
{
	for (int i = 0; i < samples.size(); i++)
	{
		addSample(samples[i]);
	}
}

std::vector<Sample::Reference> SampleList::getSamples()
{
	return mSamples;
}

void SampleList::removeSample(Sample::Reference sample)
{
	//mSamples.erase(std::find(mSamples.begin(), mSamples.end(), sample));
}

void samplify::SampleList::removeSample(int index)
{
	mSamples.erase(mSamples.begin() + index);
}

void samplify::SampleList::clearSamples()
{
	mSamples.clear();
}

void samplify::SampleList::selectionSort(std::vector<Sample::Reference> samples, SortingMethod method)
{
	std::vector<Sample::Reference> sorted;
	for (int i = 0; i < samples.size(); i++)
	{
		Sample::Reference lowest = samples[i];
		for (int j = 0; j < samples.size(); j++)
		{
			if (getSortBool(samples[i], lowest, method))
			{
				lowest = samples[j];
			}
		}
		sorted.push_back(lowest);
	}
	mSamples = sorted;
}

void samplify::SampleList::quickSort(std::vector<Sample::Reference>& list,int low, int high, SortingMethod method)
{
	/*
	int i = low;
	int j = high;

	Sample::Reference pivot = list[(low + high) / 2];

	while (i <= j)
	{
		while (getSortBool(list[i], pivot, method) && i < list.size()-1)
		{
			i++;
		}
		while (getSortBool(pivot, list[j], method) && j >= 0)
		{
			j--;
		}
		if (i <= j)
		{
			//swap(list[i], list[j]);
			Sample::Reference tmp = list[i];
			list[i] = list[j];
			list[j] = tmp;
			i++;
			j--;
		}
	}

  	if (low < j)
	{
		quickSort(list, low, j, method);
	}

	if (i < high)
	{
		quickSort(list, i, high, method);
	}
	mSortingMethod = method;
	*/
}

bool samplify::SampleList::getSortBool(Sample::Reference lhs, Sample::Reference rhs, SortingMethod method)
{
	if (method == SortingMethod::LastToFirst)
	{
		if (lhs.getFilename() < rhs.getFilename())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (method == SortingMethod::Newest)
	{
		if (lhs.getFile().getCreationTime() < rhs.getFile().getCreationTime())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (method == SortingMethod::Oldest)
	{
		if (lhs.getFile().getCreationTime() < rhs.getFile().getCreationTime())
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
		if (lhs.getFilename() < rhs.getFilename())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

SortingMethod SampleList::getSortingMethod()
{
	return mSortingMethod;
}

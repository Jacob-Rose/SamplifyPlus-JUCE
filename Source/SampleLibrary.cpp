#include "SampleLibrary.h"

using namespace samplify;

SampleLibrary::SampleLibrary()
{
}

SampleLibrary::SampleLibrary(const SampleLibrary&)
{
	
}

SampleLibrary::~SampleLibrary()
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		//delete mSamples[i];
	}
}


void SampleLibrary::addSample(File file)
{
	//check if already exist
	SampleReference ref(file);
	if(!containsSample(file))
		mSamples.push_back(ref);
}

void SampleLibrary::addSamples(std::vector<File> files)
{
	for (int i = 0; i < files.size(); i++)
	{
		addSample(files[i]);
	}
}

void SampleLibrary::addSamples(std::vector<SampleReference> files)
{
	for (int i = 0; i < files.size(); i++)
	{
		addSample(files[i]);
	}
}

void SampleLibrary::addSample(SampleReference& ref)
{
	if (!containsSample(ref.getFile()))
		mSamples.push_back(ref);
}

void SampleLibrary::removeSample(File file)
{
}

void SampleLibrary::clearSamples()
{
}

bool SampleLibrary::containsSample(File file)
{
	return false;
}

void SampleLibrary::sortCurrentSamples(SortingMethod method)
{
	mCurrentSamples.sortSamples(method);
}

void SampleLibrary::updateCurrentSamples(File path, String query)
{
	mCurrentSamples.clearSamples();
	mDirectorySamples.clearSamples();
	for (int i = 0; i < mSamples.size(); i++)
	{
		SampleReference* ref = &mSamples[i];
		if (ref->getFile().isAChildOf(path))
		{
			mDirectorySamples.addSample(ref);
			if (ref->getFilename().containsIgnoreCase(query))
			{
				mCurrentSamples.addSample(ref);
			}
		}
	}
	mCurrentQuery = query;
	mCurrentDirectory = path;
	sendChangeMessage();
}

void SampleLibrary::updateCurrentSamples(File path)
{
	updateCurrentSamples(path, mCurrentQuery);
}

void SampleLibrary::updateCurrentSamples(String query)
{
	updateCurrentSamples(mCurrentDirectory, query);
}

std::vector<SampleReference*> SampleLibrary::getAllSamplesInSelectedDirectory()
{
	return mDirectorySamples.getSamples();
}

std::vector<SampleReference*> SampleLibrary::getCurrentSamples()
{
	return mCurrentSamples.getSamples();
}

StringArray SampleLibrary::getAllTags()
{
	//todo sort on order
	StringArray tags = StringArray();
	for (int i = 0; i < mSamples.size(); i++)
	{
		StringArray sTags = mSamples[i].getTags();
		for (int j = 0; j < sTags.size(); j++)
		{
			if (!tags.contains(sTags[j]))
			{
				tags.add(sTags[j]);
			}
		}
		
	}
	return tags;
}

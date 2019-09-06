/*
  ==============================================================================

    SamplifyCore.cpp
    Created: 6 Sep 2019 1:04:31pm
    Author:  jacob

  ==============================================================================
*/

#include "SamplifyCore.h"
using namespace samplify;

std::shared_ptr<SamplifyCore> SamplifyCore::msInstance = nullptr;
samplify::SamplifyCore::SamplifyCore()
{
}
samplify::SamplifyCore::~SamplifyCore()
{
}
void SamplifyCore::createInstance()
{
	msInstance = std::make_shared<SamplifyCore>();
}
void SamplifyCore::init()
{
	if (!mIsInit)
	{
		mIsInit = true;
	}
}
void SamplifyCore::cleanup()
{
	if (mIsInit)
	{
		mIsInit = false;
	}
}
void SamplifyCore::addDirectory(const File& newDir)
{
	mSampleDirectories.push_back(SampleDirectory(newDir));
	sendChangeMessage();
}

void SamplifyCore::removeDirectory(const File& dirToRemove)
{
	//todo
}

Sample::List SamplifyCore::getAllSamples()
{
	Sample::List all;
	for (int i = 0; i < mSampleDirectories.size(); i++)
	{
		all.addSamples(mSampleDirectories[i].getAllNestedSamples());
	}
	return all;
}

Sample::List SamplifyCore::getSearchedSamples(juce::String query)
{
	Sample::List all;
	for (int i = 0; i < mSampleDirectories.size(); i++)
	{
		all.addSamples(mSampleDirectories[i].getAllSearchSamples(query));
	}
	return all;
}

std::promise<Sample::List> samplify::SamplifyCore::getSearchedSamplesAsync(juce::String query)
{
	std::promise<Sample::List> promise;
}

File SamplifyCore::browseForDirectory()
{
	FileChooser dirSelector(String("Select Music Directory"), File::getSpecialLocation(File::userHomeDirectory));
	if (dirSelector.browseForDirectory())
	{
		return dirSelector.getResult();
	}
	return File();
}

samplify::SampleDirectory::SampleDirectory(const File& file) : mDirectory(file)
{
	if (file.existsAsFile())
	{
		mStatus = Enabled;
		mLoadContainedSamples = true;
	}
	else
	{
		mStatus = NotLoaded;
		mLoadContainedSamples = false;
	}
}

SampleDirectory::~SampleDirectory()
{
}

Sample::List SampleDirectory::getAllNestedSamples()
{
	return getAllSearchSamples("");
}

Sample::List SampleDirectory::getAllSearchSamples(juce::String query)
{
	Sample::List newList;
	if (mStatus == Enabled || mStatus == Mixed)
	{
		if (mLoadContainedSamples)
		{
			for (int i = 0; i < mSamplesInDirectory.size(); i++)
			{
				if (mSamplesInDirectory[i]->isContainedInSearch(query))
				{
					newList.addSample(Sample::Reference(mSamplesInDirectory[i]));
				}
			}
		}
		for (int i = 0; i < mChildDirectories.size(); i++)
		{
			newList.addSamples(mChildDirectories[i].getAllNestedSamples());
		}
	}
	return newList;
}

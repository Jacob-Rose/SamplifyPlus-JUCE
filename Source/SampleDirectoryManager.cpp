/*
  ==============================================================================

    SampleDirectoryManager.cpp
    Created: 13 Sep 2019 5:44:08pm
    Author:  jacob

  ==============================================================================
*/

#include "SampleDirectoryManager.h"
using namespace samplify;

void SampleDirectoryManager::addDirectory(const File& dir)
{
	mRootDirectories.push_back(std::make_shared<SampleDirectory>(dir));
}

void SampleDirectoryManager::removeDirectory(const File& dir)
{
	for (int i = 0; i < mRootDirectories.size(); i++)
	{
		if (mRootDirectories[i]->getFile() == dir)
		{
			mRootDirectories.erase(mRootDirectories.begin() + i);
			break;
		}
	}
}

std::vector<File> samplify::SampleDirectoryManager::getDirectories()
{
	std::vector<File> dirs;
	for (int i = 0; i < mRootDirectories.size(); i++)
	{
		dirs.push_back(mRootDirectories[i]->getFile());
	}
	return dirs;
}

Sample::List samplify::SampleDirectoryManager::getAllSamples(std::vector<std::shared_ptr<SampleDirectory>>& dirs, juce::String query, bool ignoreCheckSystem)
{
	Sample::List list;
	for (int i = 0; i < dirs.size(); i++)
	{
		list += dirs[i]->getChildSamplesRecursive(ignoreCheckSystem);
	}
	return Sample::List();
}

std::future<Sample::List> SampleDirectoryManager::getAllSamplesAsync(juce::String query, bool ignoreCheckSystem) const
{
	return std::async(std::launch::async, getAllSamples, mRootDirectories, query, ignoreCheckSystem);
}

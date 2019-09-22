/*
  ==============================================================================

    SampleDirectory.cpp
    Created: 13 Sep 2019 5:43:59pm
    Author:  jacob

  ==============================================================================
*/

#include "SampleDirectory.h"
using namespace samplify;

SampleDirectory::SampleDirectory(File file)
{
	//add all child directories as sampleDirectory, then recursively all them do the same for all child folders
	DirectoryIterator dirIter = DirectoryIterator(file, false, "*", File::findDirectories);
	while (dirIter.next())
	{
		mChildDirectories.push_back(std::make_shared<SampleDirectory>(dirIter.getFile()));
	}

	//add all child samples in the actual folder
	DirectoryIterator sampleIter = DirectoryIterator(file, false, "*.wav", File::findFiles);
	while (sampleIter.next())
	{
		mChildSamples.push_back(std::make_shared<Sample>(sampleIter.getFile()));
	}

}

Sample::List samplify::SampleDirectory::getChildSamplesRecursive(bool ignoreCheckSystem)
{
	Sample::List list;
	for (int i = 0; i < mChildDirectories.size(); i++)
	{
		list += mChildDirectories[i]->getChildSamplesRecursive(ignoreCheckSystem);
	}
	if (ignoreCheckSystem || mIncludeChildSamples)
	{
		for (int i = 0; i < mChildSamples.size(); i++)
		{
			list.addSample(Sample::Reference(mChildSamples[i]));
		}
	}
	return list;
}

Sample::List samplify::SampleDirectory::getChildSamples()
{
	Sample::List list;
	for (int i = 0; i < mChildSamples.size(); i++)
	{
		list.addSample(Sample::Reference(mChildSamples[i]));
	}
	return list;
	
}

std::shared_ptr<SampleDirectory> samplify::SampleDirectory::getChildDirectory(int index)
{
	return mChildDirectories[index];
}

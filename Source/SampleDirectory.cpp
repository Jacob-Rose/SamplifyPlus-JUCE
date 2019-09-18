/*
  ==============================================================================

    SampleDirectory.cpp
    Created: 13 Sep 2019 5:43:59pm
    Author:  jacob

  ==============================================================================
*/

#include "SampleDirectory.h"
using namespace samplify;
<<<<<<< HEAD
SampleDirectory::SampleDirectory(File file)
=======

SampleDirectory::SampleDirectory(File dir)
>>>>>>> origin/implemention
{
	//add all child directories as sampleDirectory, then recursively all them do the same for all child folders
	DirectoryIterator dirIter = DirectoryIterator(file, false, "*", File::findDirectories);
	while (dirIter.next())
	{
		mChildDirectories.push_back(SampleDirectory(dirIter.getFile()));
	}

	//add all child samples in the actual folder
	DirectoryIterator sampleIter = DirectoryIterator(file, false, "*.wav", File::findFiles);
	while (sampleIter.next())
	{
		mChildSamples.push_back(std::make_shared<Sample>(sampleIter.getFile()));
	}

}

Sample::List samplify::SampleDirectory::getChildSamplesRecursive()
{
	return Sample::List();
}

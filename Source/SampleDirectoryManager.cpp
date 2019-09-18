/*
  ==============================================================================

    SampleDirectoryManager.cpp
    Created: 13 Sep 2019 5:44:08pm
    Author:  jacob

  ==============================================================================
*/

#include "SampleDirectoryManager.h"
using namespace samplify;

void SampleDirectoryManager::addDirectory()
{
}

void SampleDirectoryManager::removeDirectory()
{
}

Sample::List SampleDirectoryManager::getAllQueriedSamples(juce::String query) const
{
	return Sample::List();
}

Sample::List samplify::SampleDirectoryManager::getAllSamples(std::vector<SampleDirectory>& dir, juce::String query)
{
	return Sample::List();
}

std::future<Sample::List> samplify::SampleDirectoryManager::getAllQueriedSamplesAsync(juce::String query) const
{
	return std::async(std::launch::async, getAllSamples, mRootDirectories, query);
}

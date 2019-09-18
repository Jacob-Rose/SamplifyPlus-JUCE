/*
  ==============================================================================

    SampleDirectoryManager.h
    Created: 13 Sep 2019 5:44:07pm
    Author:  jacob

  ==============================================================================
*/

#ifndef SAMPLEDIRECTORYMANAGER_H
#define SAMPLEDIRECTORYMANAGER_H
#include <vector>
#include <future>

#include "SampleDirectory.h"
namespace samplify
{
	class SampleDirectoryManager : public ChangeBroadcaster
	{
	public:

		void addDirectory();
		void removeDirectory();

		Sample::List getAllQueriedSamples(juce::String query) const;
		static Sample::List getAllSamples(std::vector<SampleDirectory>& dir, juce::String query);
		std::future<Sample::List> getAllQueriedSamplesAsync(juce::String query) const;
	private:

		std::vector<SampleDirectory> mRootDirectories;
	};
}
#endif

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
		void addDirectory(const File& dir);
		void removeDirectory(const File& dir);
		int getCount() { return mRootDirectories.size(); }
		std::vector<File> getDirectories();
		File getRelativeDirectoryForFile(const File& sampleFile) const {
			for (int i = 0; i < mRootDirectories.size(); i++)
			{
				if (sampleFile.isAChildOf(mRootDirectories[i]->getFile()))
				{
					return mRootDirectories[i]->getFile();
				}
			}
			return File();
		}
		std::shared_ptr<SampleDirectory> getSampleDirectory(int index) { return mRootDirectories[index]; }

		static Sample::List getAllSamples(std::vector<std::shared_ptr<SampleDirectory>>& dir, juce::String query, bool ignoreCheckSystem);
		std::future<Sample::List> getAllSamplesAsync(juce::String query = "", bool ignoreCheckSystem = false) const;
	private:

		std::vector<std::shared_ptr<SampleDirectory>> mRootDirectories;
	};
}
#endif

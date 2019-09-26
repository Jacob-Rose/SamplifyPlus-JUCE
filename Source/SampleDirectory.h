/*
  ==============================================================================

    SampleDirectory.h
    Created: 13 Sep 2019 5:43:59pm
    Author:  jacob

  ==============================================================================
*/

#ifndef SAMPLEDIRECTORY_H
#define SAMPLEDIRECTORY_H
#include <JuceHeader.h>

#include <vector>

#include "Sample.h"

namespace samplify
{
	class SampleDirectory
	{
	public:
		enum CheckStatus
		{
			NotLoaded = -1,
			Enabled,
			Disabled,
			Mixed,
		};

		SampleDirectory(File file/*, std::weak_ptr<SampleDirectory parent*/);
		File getFile() const { return mDirectory; }
		Sample::List getChildSamplesRecursive(juce::String query, bool ignoreCheckSystem);
		Sample::List getChildSamples();

		void updateChildrenItems(CheckStatus checkStatus);

		CheckStatus getCheckStatus() { return mCheckStatus; }
		int getChildDirectoryCount() { return mChildDirectories.size(); }
		std::shared_ptr<SampleDirectory> getChildDirectory(int index);
		friend class DirectoryExplorerTreeViewItem;
		friend class SampleDirectoryManager; //used to optimze callback function later, not necessary though could call recursive
	private:
		CheckStatus mCheckStatus;
		File mDirectory;
		bool mIncludeChildSamples = true; //if the folder should load its own samples when getsamples is called
		std::vector<std::shared_ptr<Sample>> mChildSamples; //safer
		std::vector<std::shared_ptr<SampleDirectory>> mChildDirectories;
	};
}
#endif

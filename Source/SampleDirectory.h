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
#include "Sample.h"
#include <vector>
namespace samplify
{
	class SampleDirectory
	{
	public:
		SampleDirectory(File file);
	private:
		File mDirectory;
		bool mIncludeChildSamples; //if the folder should load its own samples when getsamples is called
		std::vector<std::shared_ptr<Sample>> mChildSamples;
		std::vector<SampleDirectory> mChildDirectories;
	};
}
#endif

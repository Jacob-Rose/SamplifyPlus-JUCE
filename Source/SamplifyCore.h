/*
  ==============================================================================

    SamplifyCore.h
    Created: 21 Sep 2019 5:35:42pm
    Author:  student

  ==============================================================================
*/

#ifndef SAMPLIFYCORE_H
#define SAMPLIFYCORE_H

#include "TagLibrary.h"
#include "SampleLibrary.h"
#include "SampleDirectoryManager.h"

namespace samplify
{
	class SamplifyCore
	{
	public:
		SamplifyCore();

		//============
		void init();
		void cleanup();
		//===============
		std::shared_ptr<SampleDirectoryManager> getDirectoryManager() { return mDirectoryManager; }
	private:
		bool mIsInit = false;
		std::shared_ptr<SampleDirectoryManager> mDirectoryManager;
	};
}

#endif

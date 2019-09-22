/*
  ==============================================================================

    SampleLibrary.h
    Created: 31 May 2018 12:45:49pm
    Author:  Jake Rose
	Summary: Class with ability to load all samples in the directory specified in AppProperties class
  ==============================================================================
*/
#ifndef SAMPLELIBRARY_H
#define SAMPLELIBRARY_H

#include "JuceHeader.h"

#include "Sample.h"

#include <vector>
#include "SampleDirectoryManager.h"

namespace samplify
{
	class SampleLibrary : public ChangeBroadcaster, public ChangeListener
	{
	public:
		SampleLibrary(std::shared_ptr<SampleDirectoryManager> manager);
		~SampleLibrary();

		void refreshCurrentSamples()
		{
			updateCurrentSamples(mCurrentQuery);
		}
		void updateCurrentSamples(String query);
		void changeListenerCallback(ChangeBroadcaster* source) override;
		Sample::List getCurrentSamples();

	private:
		std::unique_ptr<std::future<Sample::List>> updateSampleFuture;
		Sample::List mCurrentSamples;
		String mCurrentQuery;

		std::shared_ptr<SampleDirectoryManager> mDirectoryManager;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleLibrary)
	};
}
#endif
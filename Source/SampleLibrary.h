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

namespace samplify
{
	class SampleLibrary : public ChangeBroadcaster, public Thread::Listener
	{
	public:
		SampleLibrary();
		SampleLibrary(const SampleLibrary&);
		~SampleLibrary();

		void addSample(const File& file);
		void addSample(std::shared_ptr<Sample> ref);
		void addSamples(std::vector<File> files);
		void addSamples(std::vector<std::shared_ptr<Sample>> files);
		void removeSample(const File& file);
		//void clearSamples();

		bool containsSample(File file);

		class UpdateSamplesThread : public Thread
		{
		public:
			UpdateSamplesThread(SampleLibrary* parent) : Thread("Update Samples", 0)
			{
				mParent = parent;
				mSamples = parent->getAllSamples();
			}
			void run() override;
			friend class SampleLibrary;
		private:
			SampleLibrary* mParent;
			Sample::List mSamples;
		};

		void exitSignalSent() override;

		void updateCurrentSamples(File path, String query);
		void updateCurrentSamples(File path);
		void updateCurrentSamples(String query);
		void setCurrentSamples(Sample::List samples);
		Sample::List getCurrentSamples();
		Sample::List getAllSamples();
		StringArray getAllTags();

	private:
		UpdateSamplesThread* updateThread = nullptr;
		std::vector<std::shared_ptr<Sample>> mSamples;
		Sample::List mCurrentSamples;
		File mCurrentDirectory;
		String mCurrentQuery;

		JUCE_LEAK_DETECTOR(SampleLibrary)
	};
}
#endif
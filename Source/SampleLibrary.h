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

		void addSample(File file);
		void addSample(Sample& ref);
		void addSamples(std::vector<File> files);
		void addSamples(std::vector<Sample> files);
		//void removeSample(File file);
		void saveSamplePropertyFiles();
		//void clearSamples();

		bool containsSample(File file);

		class UpdateSamplesThread : public Thread
		{
		public:
			UpdateSamplesThread(SampleLibrary* parent) : Thread("Update Samples", 0)
			{
				mParent = parent;
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
		std::vector<Sample> mSamples;
		Sample::List mCurrentSamples;
		File mCurrentDirectory;
		String mCurrentQuery;

		JUCE_LEAK_DETECTOR(SampleLibrary)
	};
}
#endif
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

	class SampleDirectory
	{
	public:
		SampleDirectory(const File& file);
		~SampleDirectory();
		enum Status
		{
			NotLoaded = -1,
			Enabled,
			Disabled,
			Mixed
		};
		Sample::List getAllNestedSamples();
		Sample::List getAllSearchSamples(juce::String query);
		Status getStatus() { return mStatus; }
		File getDirectory() { return mDirectory; }
		bool shouldLoadContainedSamples() { return mLoadContainedSamples; }
	private:
		File mDirectory;
		Status mStatus;
		bool mLoadContainedSamples = true;
		std::vector<SampleDirectory> mChildDirectories;
		std::vector<std::shared_ptr<Sample>> mSamplesInDirectory;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleDirectory);
	};
	class SampleLibrary : public ChangeBroadcaster, public Thread::Listener, public ChangeListener
	{
	public:

		class LoadSamplesThread : public ThreadWithProgressWindow
		{
		public:
			LoadSamplesThread(File file) : ThreadWithProgressWindow("loading samples...", true, false),
				mDirectory(file) {}
			void run();
		private:
			File mDirectory;
		};

		SampleLibrary();
		~SampleLibrary();

		void addSample(const File& file);
		void addSample(std::shared_ptr<Sample> ref);
		void addSamples(std::vector<File> files);
		void addSamples(std::vector<std::shared_ptr<Sample>> files);
		void removeSample(const File& file);
		void sortSamples(Sample::SortMethod method);
		void randomizeSamples();
		//void clearSamples();

		bool containsSample(File file);

		class UpdateSamplesThread : public Thread
		{
		public:
			UpdateSamplesThread(SampleLibrary* parent) : Thread("updateSamples", 0)
			{
				mParent = parent;
				mSamples = mParent->getAllSamples();
			}
			void run() override;
			friend class SampleLibrary;
		private:
			SampleLibrary* mParent;
			Sample::List mSamples;
		};
	//todo add sorting

		void exitSignalSent() override;
		void checkThreadFinished();
		bool isUpdating() const { return currentUpdateThread.get() != nullptr; }
		void cancelUpdate() {
			currentUpdateThread.get()->stopThread(1000);
			currentUpdateThread.reset(nullptr);
		}
		void changeListenerCallback(ChangeBroadcaster* source) override;
		Sample::List getCurrentSamples();
		Sample::List getAllSamples();
		StringArray getAllTags();

	private:
		std::unique_ptr<Thread> currentUpdateThread = nullptr;
		std::vector<SampleDirectory> mSampleDirectories;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleLibrary)
	};
}
#endif
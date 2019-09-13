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
		
		class SortSamplesThread : public Thread
		{
		public:
			SortSamplesThread(SampleLibrary* parent, Sample::SortMethod method) : Thread("sortSamples", 0) {
				mParent = parent;
				mSamples = mParent->getCurrentSamples();
				mMethod = method;
			}
			void run() override;
			friend class SampleLibrary;
		private:
			Sample::SortMethod mMethod;
			SampleLibrary* mParent;
			Sample::List mSamples;
		};

		void exitSignalSent() override;
		void checkThreadFinished();
		void refreshCurrentSamples()
		{
			updateCurrentSamples(mCurrentDirectory, mCurrentQuery);
		}
		void updateCurrentSamples(File path, String query);
		void updateCurrentSamples(File path);
		void updateCurrentSamples(String query);
		void setCurrentSamples(Sample::List samples, bool shouldSendChangeMessage = true);

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
		std::vector<std::shared_ptr<Sample>> mSamples;
		Sample::List mCurrentSamples; //to remove
		File mCurrentDirectory;
		String mCurrentQuery;

		//SampleDirectoryManager mDirectoryManager;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleLibrary)
	};
}
#endif
/*
  ==============================================================================

    SampleLibrary.h
    Created: 31 May 2018 12:45:49pm
    Author:  Jake Rose
	Summary: This is the core system Samplify uses to load samples, and basically offers an interface for SamplifyProperties to access. 
	This way, this all is independent from SamplifyProperties and the actual execution
  ==============================================================================
*/
#ifndef SAMPLELIBRARY_H
#define SAMPLELIBRARY_H

#include "JuceHeader.h"

#include "SampleDirectory.h"

#include <vector>
#include <future>
#include <algorithm>

namespace samplify
{
	class SampleLibrary : public ChangeBroadcaster, public ChangeListener, public Timer
	{
	public:

		struct Tag
		{
			//simple holder for information, can be expanded later
			Tag(juce::String title, juce::Colour color) : mTitle(title), mColor(color) {}

			juce::String mTitle;
			juce::Colour mColor;
		};

		SampleLibrary();
		~SampleLibrary();

		void refreshCurrentSamples() { updateCurrentSamples(mCurrentQuery); }
		void updateCurrentSamples(String query);

		void sortSamples(SortingMethod method);

		Sample::List getCurrentSamples();
		String getCurrentQuery() { return mCurrentQuery; }

		StringArray getUsedTags(); //get tags that are currently connected to one or more samples

		void timerCallback() override;

		///Tag Library Merger - They are dependent on each other for results and modifications, so fuck it put them together
		void addTag(juce::String tag, Colour color);
		void addTag(juce::String tag);
		//void renameTag(juce::String currentTagName, juce::String desiredName);
		void deleteTag(juce::String tag);
		int getTagCount() { return mTags.size(); }
		std::vector<Tag> getTags() { return mTags; }
		StringArray getTagsStringArray();

		void setTagColor(juce::String tag, juce::Colour newColor);
		SampleLibrary::Tag getTag(juce::String tag);


		///Directory Manager Merger - Reduce dependencies, less pointers, easier saving, so fuck it put them together
		void addDirectory(const File& dir);
		std::vector<std::shared_ptr<SampleDirectory>> getDirectories() { return mDirectories; }
		void removeDirectory(const File& dir);
		int getDirectoryCount() { return mDirectories.size(); }

		File getRelativeDirectoryForFile(const File& sampleFile) const;

		void changeListenerCallback(ChangeBroadcaster* source) override;

		bool isAsyncValid() { return mUpdateSampleFuture.valid(); }

		//Get Samples
		Sample::List getAllSamplesInDirectories(juce::String query, bool ignoreCheckSystem);
		std::future<Sample::List> getAllSamplesInDirectories_Async(juce::String query = "", bool ignoreCheckSystem = false);

	private:
		std::future<Sample::List> mUpdateSampleFuture;
		bool mUpdatingSamples = false;
		bool mCancelUpdating = false;
		Sample::List mCurrentSamples;
		String mCurrentQuery;

		std::vector<Tag> mTags;
		//pointer necessary to keep the check system
		std::vector<std::shared_ptr<SampleDirectory>> mDirectories = std::vector<std::shared_ptr<SampleDirectory>>(); 

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleLibrary)
	};
}
#endif
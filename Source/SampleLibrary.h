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

#include "SampleDirectory.h"

#include <vector>
#include <future>
#include <algorithm>

namespace samplify
{

	class SampleLibrary : public ChangeBroadcaster, public ChangeListener
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

		void refreshCurrentSamples()
		{
			updateCurrentSamples(mCurrentQuery);
		}

		void updateCurrentSamples(String query);
		Sample::List getCurrentSamples();

		StringArray getUsedTags(); //get tags that are currently connected to one or more samples


		//Tag Library Merger - They are dependent on each other for results and modifications, so fuck it put them together
		void addTag(juce::String tag, Colour color);
		void addTag(juce::String tag);
		//void renameTag(juce::String currentTagName, juce::String desiredName);
		void deleteTag(juce::String tag);
		int getTagCount() { return mTags.size(); }
		std::vector<Tag> getTags() { return mTags; }

		void setTagColor(juce::String tag, juce::Colour newColor);
		juce::Colour getTagColor(juce::String tag);


		//Merge Directory Manager - Reduce dependencies, less pointers, easier saving, so fuck it put them together
		void addDirectory(const File& dir);
		std::vector<std::shared_ptr<SampleDirectory>> getDirectories() { return mDirectories; }
		void removeDirectory(const File& dir);
		int getDirectoryCount() { return mDirectories.size(); }


		File getRelativeDirectoryForFile(const File& sampleFile) const;

		void changeListenerCallback(ChangeBroadcaster* source) override;

		//Get Samples
		Sample::List getAllSamplesInDirectories(juce::String query, bool ignoreCheckSystem);
		std::future<Sample::List> getAllSamplesInDirectories_Async(juce::String query = "", bool ignoreCheckSystem = false);

	private:
		std::unique_ptr<std::future<Sample::List>> updateSampleFuture;
		Sample::List mCurrentSamples;
		String mCurrentQuery;

		std::vector<Tag> mTags;
		//pointer necessary to keep the check system
		std::vector<std::shared_ptr<SampleDirectory>> mDirectories = std::vector<std::shared_ptr<SampleDirectory>>(); 

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleLibrary)
	};
}
#endif
/*
  ==============================================================================

    AppProperties.h
    Created: 31 May 2018 3:16:14pm
    Author:  jacob

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <string>
#include "SampleLibrary.h"
#include <vector>
static class AppProperties
{
public:
	static void init()
	{
		mDirectories = std::vector<File>();
		mSelectedDirectory = File("");
		loadDirectories();
		mSampleLibrary.reset(new SampleLibrary());
	}
	static void loadDirectories();

	static File browseForDirectory();

	static void browseForDirectoryAndAdd();

	static std::vector<File> getDirectories()
	{
		return mDirectories;
	}

	static File getDefaultFile();

	static File getSelectedDirectory()
	{
		return mSelectedDirectory;
	}

	static SampleLibrary* getSampleLibrary()
	{
		return mSampleLibrary.get();
	}

	static void setSelectedDirectory(File directory)
	{
		mSelectedDirectory = directory;
		AppProperties::getSampleLibrary()->updateCurrentSamples(mSelectedDirectory);
	}

	static void saveDirectories();

	static void clearDirectories()
	{
		mDirectories.clear();
	}

	static void updateDirectories(std::vector<File> directories)
	{
		clearDirectories();
		mDirectories = directories;
	}
private:
	static std::vector<File> mDirectories;
	static File mSelectedDirectory;
	static std::unique_ptr<SampleLibrary> mSampleLibrary;
};
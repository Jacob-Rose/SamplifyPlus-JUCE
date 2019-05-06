/*
  ==============================================================================

    AppProperties.h
    Created: 31 May 2018 3:16:14pm
    Author:  Jacob Rose

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

	static void initInstance();
	static void cleanupInstance();
	
	void init();
	void cleanup();

	void loadDirectories();

	File browseForDirectory();

	void browseForDirectoryAndAdd();

	std::vector<File> getDirectories() { return mDirectories; }

	File getDefaultFile();

	File getSelectedDirectory() { return mSelectedDirectory;	}

	SampleLibrary* getSampleLibrary() {	return mSampleLibrary.get(); }

	void setSelectedDirectory(File directory);

	void saveDirectories();

	void clearDirectories();

	void updateDirectories(std::vector<File> directories);

private:
	AppProperties();
	~AppProperties();

	bool mIsInit = false;

	std::vector<File> mDirectories = std::vector<File>();
	File mSelectedDirectory = File("");
	std::unique_ptr<SampleLibrary> mSampleLibrary = nullptr;

	static std::unique_ptr<AppProperties> smAppProperties;
};
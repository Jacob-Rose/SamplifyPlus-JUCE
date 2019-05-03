/*
  ==============================================================================

    AppProperties.cpp
    Created: 31 May 2018 3:16:14pm
    Author:  jacob

  ==============================================================================
*/

#include "AppProperties.h"

std::vector<File> AppProperties::mDirectories = std::vector<File>();
File AppProperties::mSelectedDirectory = File("");
std::unique_ptr<SampleLibrary> AppProperties::mSampleLibrary = nullptr;

File AppProperties::browseForDirectory()
{
	FileChooser dirSelector(String("Select Music Directory"), File::getSpecialLocation(File::userHomeDirectory));
	dirSelector.browseForDirectory();
	return dirSelector.getResult();
}

void AppProperties::browseForDirectoryAndAdd()
{
	File dir = browseForDirectory();
	mDirectories.push_back(dir);
}

File AppProperties::getDefaultFile()
{
	PropertiesFile::Options options;
	options.applicationName = ProjectInfo::projectName;
	options.commonToAllUsers = false;
	options.filenameSuffix = ".settings";
	File propFile = options.getDefaultFile();
	return propFile;
}

void AppProperties::loadDirectories()
{
	/*
	File propFile = getDefaultFile();
	if (propFile.existsAsFile())
	{
		StringArray propFileLines;
		propFile.readLines(propFileLines);
		int dirCount = std::stoi(propFileLines[0].toStdString());
		for (int i = 0; i < dirCount; i++)
		{
			mDirectories.push_back(File(propFileLines[i + 1]));
		}
		if (mDirectories.size() == 0)
		{
			browseForDirectoryAndAdd();
		}
	}
	else
	{
		browseForDirectoryAndAdd();
	}
	*/
	//this is my personal storage, current bug in debug is that the properties file does not save, so just loading mine directly
	mDirectories.push_back(File("D:\\Users\\jacob\\Music\\Music Assets\\LOOPMASTERS 4Gig Pack"));
}

void AppProperties::saveDirectories()
{
	File propFile = getDefaultFile();
	if (propFile.existsAsFile())
	{
		propFile.deleteFile();
	}
	propFile.create();
	propFile.appendText(String(mDirectories.size()) + '\n');
	for (int i = 0; i < mDirectories.size(); i++)
	{
		propFile.appendText(mDirectories[i].getFullPathName() + "\n");
	}
}
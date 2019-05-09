/*
  ==============================================================================

    AppProperties.cpp
    Created: 31 May 2018 3:16:14pm
    Author:  jacob

  ==============================================================================
*/

#include "AppProperties.h"

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

void AppProperties::initInstance()
{
	if (smAppProperties == nullptr)
	{
		smAppProperties = new AppProperties();
		smAppProperties->init();
	}
}

void AppProperties::cleanupInstance()
{
	if (smAppProperties != nullptr)
	{
		smAppProperties->saveDirectories();
		delete smAppProperties;
		smAppProperties = nullptr;
	}

}

AppProperties* AppProperties::getInstance()
{
	return smAppProperties;
}

void AppProperties::init()
{
	mDirectories = std::vector<File>();
	mSelectedDirectory = File("");
	loadDirectories();
	mSampleLibrary.reset(new SampleLibrary());
}

void AppProperties::cleanup()
{
	mSampleLibrary = nullptr;
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
	mDirectories.push_back(File("C:\\Users\\jacob\\Music\\stems"));
}

void AppProperties::setSelectedDirectory(File directory)
{
	mSelectedDirectory = directory;
	AppProperties::getSampleLibrary()->updateCurrentSamples(mSelectedDirectory);
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

void AppProperties::clearDirectories()
{
	mDirectories.clear();
}

void AppProperties::updateDirectories(std::vector<File> directories)
{
	clearDirectories();
	mDirectories = directories;
}

AppProperties::~AppProperties()
{
}

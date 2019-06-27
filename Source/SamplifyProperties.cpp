#include "SamplifyProperties.h"

SamplifyProperties* SamplifyProperties::smAppProperties = nullptr;

SamplifyProperties::SamplifyProperties()
{
	PropertiesFile::Options propFileOptions = PropertiesFile::Options();
	propFileOptions.applicationName = "SamplifyPlus";
	propFileOptions.commonToAllUsers = false;
	propFileOptions.filenameSuffix = ".settings";
	propFileOptions.ignoreCaseOfKeyNames = true;
	propFileOptions.storageFormat = PropertiesFile::StorageFormat::storeAsXML;
	mApplicationProperties.setStorageParameters(propFileOptions);
}

SamplifyProperties::~SamplifyProperties()
{
	cleanup();
}

File SamplifyProperties::browseForDirectory()
{
	FileChooser dirSelector(String("Select Music Directory"), File::getSpecialLocation(File::userHomeDirectory));
	dirSelector.browseForDirectory();
	return dirSelector.getResult();
}

void SamplifyProperties::browseForDirectoryAndAdd()
{
	File dir = browseForDirectory();
	mDirectories.push_back(dir);
}




void SamplifyProperties::cleanupInstance()
{
	if (smAppProperties != nullptr)
	{
		smAppProperties->saveDirectoriesToPropertiesFile();
		delete smAppProperties;
		smAppProperties = nullptr;
	}

}

void SamplifyProperties::initInstance()
{
	if (smAppProperties == nullptr)
	{
		smAppProperties = new SamplifyProperties();
		smAppProperties->init();
	}
}

SamplifyProperties* SamplifyProperties::getInstance()
{
	return smAppProperties;
}

void SamplifyProperties::init()
{
	mDirectories = std::vector<File>();
	mSelectedDirectory = File("");
	loadDirectoriesFromPropertiesFile();
	mSampleLibrary.reset(new SampleLibrary());
	mSampleLibrary->loadSamplesFromDirectory(mDirectories[0]);
}

void SamplifyProperties::cleanup()
{
	if (mIsInit)
	{
		mSampleLibrary = nullptr;
	}
	
}

void SamplifyProperties::setDirectories(std::vector<File> directories)
{
	clearDirectories();
	mDirectories = directories;
}

void SamplifyProperties::loadDirectoriesFromPropertiesFile()
{
	PropertiesFile* propFile = mApplicationProperties.getUserSettings();
	if (propFile->isValidFile())
	{
		StringArray propFileLines;
		
		int dirCount = propFile->getIntValue("directory count");
		for (int i = 0; i < dirCount; i++)
		{
			mDirectories.push_back(File(propFile->getValue("directory "+ i)));
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
}

void SamplifyProperties::setSelectedDirectory(File directory)
{
	mSelectedDirectory = directory;
	SamplifyProperties::getSampleLibrary()->updateCurrentSamples(mSelectedDirectory);
}

void SamplifyProperties::saveDirectoriesToPropertiesFile()
{
	PropertiesFile* propFile = mApplicationProperties.getUserSettings();
	if (!propFile->isValidFile())
	{
		propFile->getFile().deleteFile();
	}
	if (propFile->isValidFile())
	{
		propFile->setValue("directory count", (int)mDirectories.size());
		for (int i = 0; i < mDirectories.size(); i++)
		{
			propFile->setValue("directory " + i, mDirectories[i].getFullPathName());
		}
		propFile->saveIfNeeded();
	}
}

void SamplifyProperties::addTag(juce::String text, Colour color)
{
	mSampleTagColors[text] = color;
}

Colour SamplifyProperties::getTagColor(juce::String text)
{
	return mSampleTagColors[text];
}

void SamplifyProperties::clearDirectories()
{
	mDirectories.clear();
}




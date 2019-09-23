#include "SamplifyProperties.h"
#include "SamplifyMainComponent.h"

using namespace samplify;

SamplifyProperties* SamplifyProperties::smAppProperties = nullptr;

SamplifyProperties::SamplifyProperties()
{
	PropertiesFile::Options propFileOptions = PropertiesFile::Options();
	propFileOptions.applicationName = "SamplifyPlus";
	propFileOptions.commonToAllUsers = false;
	propFileOptions.filenameSuffix = ".settings";
	propFileOptions.ignoreCaseOfKeyNames = true;
	propFileOptions.storageFormat = PropertiesFile::StorageFormat::storeAsXML;
	setStorageParameters(propFileOptions);
}

SamplifyProperties::~SamplifyProperties()
{
	cleanup();
}

File SamplifyProperties::browseForDirectory()
{
	FileChooser dirSelector(String("Select Music Directory"), File::getSpecialLocation(File::userHomeDirectory));
	if (dirSelector.browseForDirectory())
	{
		return dirSelector.getResult();
	}
	return File();
}

void SamplifyProperties::browseForDirectoryAndAdd()
{
	File dir = browseForDirectory();
	if (dir.exists())
	{
		mDirectoryManager->addDirectory(dir);
	}
}

void SamplifyProperties::cleanupInstance()
{
	if (smAppProperties != nullptr)
	{
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
	mDirectoryManager = std::make_shared<SampleDirectoryManager>();
	mSampleLibrary = std::make_shared<SampleLibrary>(mDirectoryManager);
	mTagLibrary = std::make_shared<TagLibrary>();
	loadPropertiesFile();
	mIsInit = true;
}

void SamplifyProperties::cleanup()
{
	if (mIsInit)
	{
		mAudioPlayer->stop();
		mIsInit = false;
	}
}

void SamplifyProperties::loadPropertiesFile()
{
	PropertiesFile* propFile = getUserSettings();
	if (propFile->isValidFile())
	{
		StringArray propFileLines;
		//load dirs
		int dirCount = propFile->getIntValue("directory count");
		for (int i = 0; i < dirCount; i++)
		{
			mDirectoryManager->addDirectory(File(propFile->getValue("directory "+ i)));
		}
		if (mDirectoryManager->getCount() == 0)
		{
			browseForDirectoryAndAdd();
		}
		//load tags
		int tagCount = propFile->getIntValue("tag count");
		for (int i = 0; i < tagCount; i++)
		{
			String tag = propFile->getValue("tag " + i);
			jassert(tag != "");
			Colour color = Colour::fromString(propFile->getValue("tag " + tag));
			mTagLibrary->addTag(tag, color);
		}
	}
	else
	{
		browseForDirectoryAndAdd();
	}
}

void SamplifyProperties::savePropertiesFile()
{
	saveDirectoriesInPropertiesFile();
	
}

void SamplifyProperties::saveDirectoriesInPropertiesFile()
{
	PropertiesFile* propFile = getUserSettings();
	if (propFile->isValidFile())
	{
		propFile->setValue("directory count", (int)mDirectoryManager->getCount());
		for (int i = 0; i < mDirectoryManager->getCount(); i++)
		{
			propFile->setValue("directory " + i, mDirectoryManager->getSampleDirectory(i)->getFile().getFullPathName());
		}
		propFile->saveIfNeeded();
	}
	else
	{
		throw "Properties File is not valid file";
	}

}

void samplify::SamplifyProperties::saveTagsInPropertiesFile()
{
	mTagLibrary->saveTags(getUserSettings());
}

void SamplifyProperties::changeListenerCallback(ChangeBroadcaster* source)
{
	if (DirectoryLibrary * dirLib = dynamic_cast<DirectoryLibrary*>(source))
	{
		saveDirectoriesInPropertiesFile();
	}
}


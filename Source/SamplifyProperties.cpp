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
		mSampleLibrary->addDirectory(dir);
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
	mSampleLibrary = std::make_shared<SampleLibrary>();
	//mTagLibrary = std::make_shared<TagLibrary>();
	loadPropertiesFile();
	mIsInit = true;
}

void SamplifyProperties::cleanup()
{
	if (mIsInit)
	{
		mAudioPlayer->stop();
		savePropertiesFile();
		closeFiles();
		mIsInit = false;
	}
}

void SamplifyProperties::loadPropertiesFile()
{
	PropertiesFile* propFile = getUserSettings();
	if (propFile->isValidFile())
	{
		//load dirs
		int dirCount = propFile->getIntValue("directory count");
		if (dirCount == 0)
		{
			browseForDirectoryAndAdd();
		}
		else
		{
			for (int i = 0; i < dirCount; i++)
			{
				mSampleLibrary->addDirectory(File(propFile->getValue("directory " + String(i))));
			}
		}
		
		
		//load tags
		int tagCount = propFile->getIntValue("tag count");
		for (int i = 0; i < tagCount; i++)
		{
			String tag = propFile->getValue("tag " + String(i));
			jassert(tag != "");
			Colour color = Colour::fromString(propFile->getValue("tag " + tag));
			mSampleLibrary->addTag(tag, color);
		}
	}
	else
	{
		browseForDirectoryAndAdd();
	}
}

void SamplifyProperties::savePropertiesFile()
{
	PropertiesFile* propFile = getUserSettings();
	if (propFile->isValidFile())
	{
		//Save Dirs
		std::vector<std::shared_ptr<SampleDirectory>> dirs = mSampleLibrary->getDirectories();
		propFile->setValue("directory count", (int)dirs.size());
		for (int i = 0; i < dirs.size(); i++)
		{
			propFile->setValue("directory " + String(i), dirs[i]->getFile().getFullPathName());
		}

		//Save Tags
		int tagCount = 0;
		StringArray usedTags = mSampleLibrary->getUsedTags();
		std::vector<SampleLibrary::Tag> allTags = mSampleLibrary->getTags();
		std::vector<SampleLibrary::Tag>::iterator it = allTags.begin();
		while (it != allTags.end())
		{
			if (usedTags.contains(it->mTitle))
			{
				propFile->setValue("tag " + String(tagCount), it->mTitle);
				propFile->setValue("tag " + it->mTitle, it->mColor.toString());
				tagCount++;
			}
		}
		propFile->setValue("tag count", tagCount);
		propFile->saveIfNeeded();
	}
	else
	{
		throw "Properties File is not valid file";
	}
}

void SamplifyProperties::changeListenerCallback(ChangeBroadcaster* source)
{
	savePropertiesFile();
}


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
	mDirectoryLibrary.addChangeListener(&mSampleLibrary);
	mDirectoryLibrary.addChangeListener(this);
}

SamplifyProperties::~SamplifyProperties()
{
	cleanup();
}

File SamplifyProperties::browseForDirectory()
{
	
}

void SamplifyProperties::browseForDirectoryAndAdd()
{
	File dir = browseForDirectory();
	if (dir.exists())
	{
		mDirectoryLibrary.addDirectory(dir);
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
			mDirectoryLibrary.addDirectory(File(propFile->getValue("directory "+ i)));
		}
		if (mDirectoryLibrary.getDirectories().size() == 0)
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
			mTagLibrary.addTag(tag, color);
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
		propFile->setValue("directory count", (int)mDirectoryLibrary.getDirectories().size());
		for (int i = 0; i < mDirectoryLibrary.getDirectories().size(); i++)
		{
			propFile->setValue("directory " + i, mDirectoryLibrary.getDirectories()[i].getFullPathName());
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
	PropertiesFile* propFile = getUserSettings();
	if (propFile->isValidFile())
	{
		int tagCount = 0;
		StringArray usedTags = mSampleLibrary.getAllTags();
		std::map<String, Colour>::iterator it = mTagLibrary.mSampleTagColors.begin();
		while (it != mTagLibrary.mSampleTagColors.end())
		{
			if (usedTags.contains(it->first))
			{
				propFile->setValue("tag " + String(tagCount), &it->first);
				propFile->setValue("tag " + it->first, it->second.toString());
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
	if (DirectoryLibrary * dirLib = dynamic_cast<DirectoryLibrary*>(source))
	{
		saveDirectoriesInPropertiesFile();
	}
}

void SamplifyProperties::TagLibrary::addTag(juce::String text, Colour color)
{
	mSampleTagColors[text] = color;
}

void SamplifyProperties::TagLibrary::addTag(juce::String text)
{
	Random& r = Random::getSystemRandom();
	addTag(text, Colour(juce::uint8(r.nextInt(Range(0, 256))),
		juce::uint8(r.nextInt(Range(0, 256))),
		juce::uint8(r.nextInt(Range(0, 256)))));
}

void SamplifyProperties::TagLibrary::renameTag(juce::String currentTagName, juce::String desiredName)
{
	Sample::List allSamps = SamplifyProperties::getInstance()->mSampleLibrary.getAllSamples();
	Sample::List taggedSamps;
	for (int i = 0; i < allSamps.size(); i++)
	{
		//does nothing if not contained
		if (allSamps[i].getTags().contains(currentTagName))
		{
			taggedSamps.addSample(allSamps[i]);
			allSamps[i].removeTag(currentTagName);
		}
	}
	std::map<juce::String, Colour>::iterator it = mSampleTagColors.find(currentTagName);
	if (it != mSampleTagColors.end())
	{
		mSampleTagColors[desiredName] = it->second;
		mSampleTagColors.erase(it);
		//heavily specialized line, im sorry
	}
	for (int i = 0; i < taggedSamps.size(); i++)
	{
		taggedSamps[i].addTag(desiredName);
	}
}

void SamplifyProperties::TagLibrary::deleteTag(juce::String tag)
{
	Sample::List allSamps = SamplifyProperties::getInstance()->mSampleLibrary.getAllSamples();
	for (int i = 0; i < allSamps.size(); i++)
	{
		//does nothing if not contained
		allSamps[i].removeTag(tag);
	}
	std::map<juce::String, Colour>::iterator it = mSampleTagColors.find(tag);
	if (it != mSampleTagColors.end())
	{
		mSampleTagColors.erase(it);
		//heavily specialized line, im sorry
		SamplifyMainComponent::getInstance()->getFilterExplorer().getTagExplorer().resized();
	}
}

Colour SamplifyProperties::TagLibrary::getTagColor(juce::String text)
{
	if (mSampleTagColors.find(text) != mSampleTagColors.end())
	{
		return mSampleTagColors[text];
	}
	else
	{
		addTag(text);
		return mSampleTagColors[text];
	}
}



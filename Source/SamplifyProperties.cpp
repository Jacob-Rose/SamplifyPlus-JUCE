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
	mApplicationProperties.setStorageParameters(propFileOptions);
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
	return File::nonexistent;
}

void SamplifyProperties::browseForDirectoryAndAdd()
{
	File dir = browseForDirectory();
	if (dir != File::nonexistent)
	{
		addDirectory(dir);
	}
}

void SamplifyProperties::cleanupInstance()
{
	if (smAppProperties != nullptr)
	{
		smAppProperties->savePropertiesFile();
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
	mSampleLibrary.reset(new SampleLibrary());
	loadPropertiesFile();
}

void SamplifyProperties::cleanup()
{
	if (mIsInit)
	{
		mSampleLibrary.reset(nullptr);
	}
}

void SamplifyProperties::removeDirectory(File dir)
{
	for (int i = 0; i < mDirectories.size(); i++)
	{
		if (mDirectories[i] == dir)
		{
			mDirectories.erase(mDirectories.begin() + i);
			Sample::List allSamps = mSampleLibrary.get()->getAllSamples();
			for (int i = 0; i < allSamps.size(); i++)
			{
				if (allSamps[i].getFile().isAChildOf(dir))
				{
					mSampleLibrary.get()->removeSample(allSamps[i].getFile());
					i--;
				}
			}
			return;
		}
	}
}

void SamplifyProperties::addDirectory(File dir)
{
	mDirectories.push_back(dir);
	loadSamplesFromDirectory(dir);
}

void SamplifyProperties::setDirectories(std::vector<File> directories)
{
	clearDirectories();
	mDirectories = directories;
}

void SamplifyProperties::loadPropertiesFile()
{
	PropertiesFile* propFile = mApplicationProperties.getUserSettings();
	if (propFile->isValidFile())
	{
		StringArray propFileLines;
		//load dirs
		int dirCount = propFile->getIntValue("directory count");
		for (int i = 0; i < dirCount; i++)
		{
			addDirectory(File(propFile->getValue("directory "+ i)));
		}
		if (mDirectories.size() == 0)
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
			addTag(tag, color);
		}
	}
	else
	{
		browseForDirectoryAndAdd();
	}
}

void SamplifyProperties::loadSamplesFromDirectory(File& file)
{
	LoadSamplesThread loadSamplesThread(file);
	loadSamplesThread.runThread();
}

void SamplifyProperties::loadSamplesFromDirectories(std::vector<File>& dirs)
{
	for (int i = 0; i < dirs.size(); i++)
	{
		loadSamplesFromDirectory(dirs[i]);
	}
}

void SamplifyProperties::savePropertiesFile()
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
		int tagCount = 0;
		StringArray usedTags = mSampleLibrary->getAllTags();
		std::map<String, Colour>::iterator it = mSampleTagColors.begin();
		while (it != mSampleTagColors.end())
		{
			if (usedTags.contains(it->first))
			{
				propFile->setValue("tag " + String(tagCount), &it->first);
				propFile->setValue("tag " + it->first, &it->second.toString());
				tagCount++;
			}
		}
		propFile->setValue("tag count", tagCount);
		propFile->saveIfNeeded();
	}
}

void SamplifyProperties::addTag(juce::String text, Colour color)
{
	mSampleTagColors[text] = color;
}

void SamplifyProperties::addTag(juce::String text)
{
	Random& r = Random::getSystemRandom();
	addTag(text, Colour(juce::uint8(r.nextInt(Range(0, 256))),
		juce::uint8(r.nextInt(Range(0, 256))),
		juce::uint8(r.nextInt(Range(0, 256)))));
}

void SamplifyProperties::deleteTag(juce::String tag)
{
	Sample::List allSamps = mSampleLibrary.get()->getAllSamples();
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

Colour SamplifyProperties::getTagColor(juce::String text)
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

void SamplifyProperties::clearDirectories()
{
	for (int i = 0; i < mDirectories.size(); i++)
	{
		//the function modifies mDirectories
		removeDirectory(mDirectories[0]);
	}
}

void SamplifyProperties::LoadSamplesThread::run()
{
	DirectoryIterator iterator(mDirectory, true, "*.wav");
	int count = 0;
	std::vector<File> mFiles;

	while (iterator.next())
	{
		if (threadShouldExit())
			break;
		//todo remove counter
		mFiles.push_back(iterator.getFile());
		setProgress(iterator.getEstimatedProgress());
		count++;
		setStatusMessage("loading... " + iterator.getFile().getFileName());
	}

	for (int i = 0; i < count; i++)
	{
		std::shared_ptr<Sample> ref = std::make_shared<Sample>(mFiles[i]);
		SamplifyProperties::getInstance()->getSampleLibrary()->addSample(ref);
		setProgress(((float)i)/count);
		setStatusMessage("calculating sample info...");
	}

}

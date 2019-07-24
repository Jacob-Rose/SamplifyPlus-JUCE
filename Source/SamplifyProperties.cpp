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
	loadDirectoriesFromPropertiesFile();
	mSampleLibrary.reset(new SampleLibrary());
	LoadSamplesThread loadSamplesThread(mDirectories[0]);
	loadSamplesThread.runThread();
}

void SamplifyProperties::cleanup()
{
	if (mIsInit)
	{
		mSampleLibrary.reset(nullptr);
	}
	
}

void samplify::SamplifyProperties::removeDirectory(File dir)
{
	bool found = false;
	for (int i = 0; i < mDirectories.size(); i++)
	{
		if (mDirectories[i] == dir)
		{
			mDirectories.erase(mDirectories.begin() + i);
			found = true;
		}
	}
	Sample::List allSamps = mSampleLibrary.get()->getAllSamples();
	for (int i = 0; i < allSamps.size(); i++)
	{
		if (allSamps[i].getFile().isAChildOf(dir))
		{
			allSamps.removeSample(i);
			i--;
		}
	}
}

void samplify::SamplifyProperties::addDirectory(File dir)
{
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

void SamplifyProperties::loadSamplesFromDirectory(File& file)
{
}

void SamplifyProperties::loadSamplesFromDirectories(std::vector<File>& dirs)
{
	for (int i = 0; i < dirs.size(); i++)
	{
		loadSamplesFromDirectory(dirs[i]);
	}
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
	mDirectories.clear();
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
		Sample ref(mFiles[i]);
		SamplifyProperties::getInstance()->getSampleLibrary()->addSample(ref);
		setProgress(((float)i)/count);
		setStatusMessage("calculating sample info...");
	}

}

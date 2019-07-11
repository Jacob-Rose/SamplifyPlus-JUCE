#include "SamplifyProperties.h"

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
	if (!mIsInit)
	{
		mDirectories = std::vector<File>();
		mSelectedDirectory = File("");
		loadDirectoriesFromPropertiesFile();
		LoadSamplesThread loadSamplesThread(mDirectories[0]);
		loadSamplesThread.runThread();
		mIsInit = true;
	}
	
}

void SamplifyProperties::cleanup()
{
	if (mIsInit)
	{
		
	}
	
}

void SamplifyProperties::removeDirectory(File dir)
{
	if (std::find(mDirectories.begin(), mDirectories.end(), dir) == mDirectories.end())
	{
		mDirectories.push_back(dir);
	}
}

void SamplifyProperties::addDirectory(File dir)
{
	if (std::find(mDirectories.begin(), mDirectories.end(), dir) == mDirectories.end())
	{
		mDirectories.push_back(dir);
	}
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

void samplify::SamplifyProperties::deleteTag(juce::String tag)
{
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

void SamplifyProperties::openRemoveDirectoryDialogue()
{
	ComboBox box;
	std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectories();
	for (int i = 0; i < dirs.size(); i++)
	{
		box.addItem(dirs[i].getFileName(), i);
	}
	DialogWindow::showModalDialog("Select Directory To Delete", &box, nullptr, Colours::black, true);
	removeDirectory(dirs.at(box.getSelectedId()));
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
		Sample::SampleReference sample = Sample::loadSample(mFiles[i]);
		setProgress(((float)i)/count);
		setStatusMessage("calculating..." + sample.getFilename());
	}

}

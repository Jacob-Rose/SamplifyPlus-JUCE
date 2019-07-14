#include "Sample.h"
#include <string>
#include "SamplifyProperties.h"

using namespace samplify;

Sample::Sample() : mFile(""), mPropertiesFile("")
{
	delete this;
}

Sample::Sample(File file) : mFile(file), mPropertiesFile(file.getFullPathName() + ".samplify")
{
	loadPropertiesFile();
	if (isPropertiesFileValid())
	{
		loadPropertiesFile();
	}
	else
	{
		determineSampleType();
	}
}

Sample::Sample(const Sample& s) : Sample(s.getFile())
{
	mFile = s.mFile;
	mPropertiesFile = s.mPropertiesFile;
	mLength = s.mLength;
	mSampleType = s.mSampleType;
	mSampleTypeConfirmed = s.mSampleTypeConfirmed;
	mTags = s.mTags;
	mThumbnail = nullptr;
	mThumbnailCache = nullptr;
}

bool Sample::isPropertiesFileValid()
{
	return mPropertiesFile.existsAsFile();
}
Sample::~Sample()
{
	mThumbnailCache.reset(nullptr);
	mThumbnail.reset(nullptr);
	
}

File Sample::getFile() const
{
	return mFile;
}

String Sample::getFilename() const
{
	return mFile.getFileName();
}

String Sample::getFullPathName() const
{
	String path = mFile.getFullPathName();
	std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectories();
	for (int i = 0; i < dirs.size(); i++)
	{
		if (path.contains(dirs[i].getFullPathName()))
		{
			return path.substring(dirs[i].getFullPathName().length());
		}
	}
	return mFile.getFullPathName();
}

Sample::SampleType Sample::getSampleType() const
{
	return mSampleType;
}

double Sample::getLength() const
{
	return mLength;
}

StringArray Sample::getTags()
{
	return mTags;
}

void samplify::Sample::addTag(juce::String tag)
{
	if (!mTags.contains(tag))
	{
		mTags.add(tag);
	}
}

StringArray Sample::getParentFolders()
{
	StringArray folders;
	File file(mFile);
	File root;
	std::vector<File> rootDirs = SamplifyProperties::getInstance()->getDirectories();
	for (int i = 0; i < rootDirs.size(); i++)
	{
		if (file.isAChildOf(rootDirs[i]))
		{
			root = rootDirs[i];
			break;
		}
	}
	while (mFile.isAChildOf(root))
	{
		mFile = mFile.getParentDirectory();
		folders.add(mFile.getFileName());
	}
	return folders;
}

AudioThumbnailCache* Sample::getAudioThumbnailCache()
{
	return mThumbnailCache.get();
}

SampleAudioThumbnail * Sample::getAudioThumbnail()
{
	return mThumbnail.get();
}

void Sample::generateThumbnailAndCache()
{
	mThumbnailCache.reset(new AudioThumbnailCache(1));
	AudioFormatManager* afm = SamplifyProperties::getInstance()->getAudioPlayer()->getFormatManager();
	mThumbnail.reset(new SampleAudioThumbnail(512, *afm, *mThumbnailCache));
	mThumbnail->addChangeListener(this);

	AudioFormatReader* reader = afm->createReaderFor(mFile);
	if (reader != nullptr)
	{
		mThumbnail->setSource(new FileInputSource(mFile));
		mLength = (float)reader->lengthInSamples / reader->sampleRate;
	}
	delete reader;
	
}

void Sample::determineSampleType()
{
	if (mLength > 4.0)
	{
		mSampleType = SampleType::LOOP;
	}
	else
	{
		mSampleType = SampleType::ONESHOT;
	}
}

void Sample::changeListenerCallback(ChangeBroadcaster * source)
{
	determineSampleType();
	sendChangeMessage();
}


void Sample::savePropertiesFile()
{
	if (mPropertiesFile.existsAsFile())
	{
		mPropertiesFile.deleteFile();
	}
	mPropertiesFile.create(); 
	mPropertiesFile.appendText(String(ProjectInfo::versionNumber) + "\n");
	mPropertiesFile.appendText(String(mTags.size()) + "\n");
	for (int i = 0; i < mTags.size(); i++)
	{
		mPropertiesFile.appendText(mTags[i] + "\n");
	}
	
}

void Sample::loadPropertiesFile()
{
	if (mPropertiesFile.existsAsFile())
	{
		StringArray propFileLines;
		mPropertiesFile.readLines(propFileLines);
		if (std::stoi(propFileLines[0].toStdString()) <= ProjectInfo::versionNumber)
		{
			int tagCount = std::stoi(propFileLines[1].toStdString());
			for (int i = 0; i < tagCount; i++)
			{
				mTags.add(propFileLines[i + 2]);
			}
		}
		else
		{
			//mPropertiesFile.deleteFile();
		}
	}
}

bool Sample::operator==(const Sample& other)
{
	if (mFile == other.mFile)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Sample::operator==(const File& other)
{
	return other == mFile;
}

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

Sample::Sample(const Sample& s)
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

AudioThumbnailCache* Sample::getAudioThumbnailCache()
{
	return mThumbnailCache.get();
}

SampleAudioThumbnail * Sample::getAudioThumbnail()
{
	return mThumbnail.get();
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
				String tag = propFileLines[i + 2];
				if (!mTags.contains(tag))
				{
					mTags.add(tag);
				}
				
			}
		}
		else
		{
			//mPropertiesFile.deleteFile();
		}
	}
}



StringArray samplify::Sample::Reference::getRelativeParentFolders() const
{
	StringArray folders;
	File file(mSample->mFile);
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
	while (mSample->mFile.isAChildOf(root))
	{
		mSample->mFile = mSample->mFile.getParentDirectory();
		folders.add(mSample->mFile.getFileName());
	}
	return folders;
}

String samplify::Sample::Reference::getRelativePathName() const
{
	String path = mSample->mFile.getFullPathName();
	std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectories();
	for (int i = 0; i < dirs.size(); i++)
	{
		if (path.contains(dirs[i].getFullPathName()))
		{
			return path.substring(dirs[i].getFullPathName().length());
		}
	}
	return mSample->mFile.getFullPathName();
}



void samplify::Sample::Reference::generateThumbnailAndCache()
{
	if (mSample->mThumbnail == nullptr)
	{
		mSample->mThumbnailCache.reset(new AudioThumbnailCache(1));
		AudioFormatManager* afm = SamplifyProperties::getInstance()->getAudioPlayer()->getFormatManager();
		mSample->mThumbnail.reset(new SampleAudioThumbnail(512, *afm, *mSample->mThumbnailCache));
		mSample->mThumbnail->addChangeListener(mSample);

		AudioFormatReader* reader = afm->createReaderFor(mSample->mFile);
		if (reader != nullptr)
		{
			mSample->mThumbnail->setSource(new FileInputSource(mSample->mFile));
			mSample->mLength = (float)reader->lengthInSamples / reader->sampleRate;
		}
		delete reader;
	}
}
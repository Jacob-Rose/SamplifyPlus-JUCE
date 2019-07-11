#include "SampleReference.h"
#include <string>
#include "SamplifyProperties.h"
#include "SampleList.h"

using namespace samplify;

std::vector<Sample> Sample::mAllSamples = std::vector<Sample>();

Sample::Sample(const File& file) : mFile(file), mPropertiesFile(file.getFullPathName() + ".samplify")
{
	loadPropertiesFile();
	if (mPropertiesFile.existsAsFile())
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


Sample::~Sample()
{
	mThumbnailCache.reset(nullptr);
	mThumbnail.reset(nullptr);
	
}

void Sample::addTag(juce::String tag)
{
	if (!mTags.contains(tag))
	{
		mTags.add(tag);
	}
}

StringArray Sample::SampleReference::getParentFolders()
{
	StringArray folders;
	File file(mSampleReferenced->mFile);
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
	while (mSampleReferenced->mFile.isAChildOf(root))
	{
		file = mSampleReferenced->mFile.getParentDirectory();
		folders.add(file.getFileName());
	}
	return folders;
}


void Sample::generateThumbnailAndCache()
{
	mThumbnailCache.reset(new AudioThumbnailCache(1));
	AudioFormatManager* afm = SamplifyProperties::getInstance()->getAudioPlayer()->getFormatManager();
	mThumbnail.reset(new SampleAudioThumbnail(512, *afm, *mThumbnailCache));

	AudioFormatReader* reader = afm->createReaderFor(mFile);
	if (reader != nullptr)
	{
		mThumbnail->setSource(new FileInputSource(mFile));
		mLength = (float)reader->lengthInSamples / reader->sampleRate;
		determineSampleType();
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
			mPropertiesFile.deleteFile();
		}
	}
}


std::vector<Sample*> Sample::getAllSamples()
{
	std::vector<Sample*> list;
	for (int i = 0; i < mAllSamples.size(); i++)
	{
		list.push_back(&mAllSamples[i]);
	}
	return list;
}

Sample Sample::loadSample(const File& file)
{
	bool contains = false;
	for (int i = 0; i < mAllSamples.size(); i++)
	{
		if (mAllSamples[i] == file)
		{
			contains = true;
			break;
		}
	}
	jassert(contains);
	Sample sample(file);
	mAllSamples.push_back(sample);
	return sample;
}
#include "SampleReference.h"
#include <string>
#include "SamplifyProperties.h"

using namespace samplify;

SampleReference::SampleReference() : mFile(""), mPropertiesFile("")
{
	delete this;
}

SampleReference::SampleReference(File file) : mFile(file), mPropertiesFile(file.getFullPathName() + ".samplify")
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

SampleReference::SampleReference(const SampleReference& s) : SampleReference(s.getFile())
{
	
}

bool SampleReference::isPropertiesFileValid()
{
	return mPropertiesFile.existsAsFile();
}
SampleReference::~SampleReference()
{
	mThumbnailCache.reset(nullptr);
	mThumbnail.reset(nullptr);
	
}

File SampleReference::getFile() const
{
	return mFile;
}

String SampleReference::getFilename() const
{
	return mFile.getFileName();
}

String SampleReference::getFullPathName() const
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

SampleReference::SampleType SampleReference::getSampleType() const
{
	return mSampleType;
}

double SampleReference::getLength() const
{
	return mLength;
}

StringArray SampleReference::getTags()
{
	return mTags;
}

void samplify::SampleReference::addTag(juce::String tag)
{
	if (!mTags.contains(tag))
	{
		mTags.add(tag);
	}
}

AudioThumbnailCache* SampleReference::getAudioThumbnailCache()
{
	return mThumbnailCache.get();
}

SampleAudioThumbnail * SampleReference::getAudioThumbnail()
{
	return mThumbnail.get();
}

void SampleReference::generateThumbnailAndCache()
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

void SampleReference::determineSampleType()
{
	if (mLength > 1.5)
	{
		//TODO
		mSampleType = SampleType::LOOP;
	}
	else
	{
		mSampleType = SampleType::ONESHOT;
	}
}

void SampleReference::changeListenerCallback(ChangeBroadcaster * source)
{
	determineSampleType();
	sendChangeMessage();
}


void SampleReference::savePropertiesFile()
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

void SampleReference::loadPropertiesFile()
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

bool SampleReference::operator==(const SampleReference& other)
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

bool SampleReference::operator==(const File& other)
{
	return other == mFile;
}

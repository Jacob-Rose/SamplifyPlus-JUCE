#include "SampleReference.h"
#include <string>
#include "SamplifyProperties.h"

SampleReference::SampleReference(File file) : mSampleFile(file), mSamplePropertiesFile(file.getFullPathName() + ".samplify"), mSampleTags()
{
	loadSamplePropertiesFile();
	if (isSamplePropertiesFileValid())
	{
		loadSamplePropertiesFile();
	}
	else
	{
		determineSampleType();
	}
}

SampleReference::SampleReference(const SampleReference& s) : SampleReference(s.getFile())
{
	
}

bool SampleReference::isSamplePropertiesFileValid()
{
	return mSamplePropertiesFile.existsAsFile();
}
SampleReference::~SampleReference()
{
	mThumbnailCache.reset(nullptr);
	mThumbnail.reset(nullptr);
	
}

File SampleReference::getFile() const
{
	return mSampleFile;
}

String SampleReference::getFilename() const
{
	return mSampleFile.getFileName();
}

String SampleReference::getFullPathName() const
{
	String path = mSampleFile.getFullPathName();
	std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectories();
	for (int i = 0; i < dirs.size(); i++)
	{
		if (path.contains(dirs[i].getFullPathName()))
		{
			return path.substring(dirs[i].getFullPathName().length());
		}
	}
	return mSampleFile.getFullPathName();
}

SampleType SampleReference::getSampleType() const
{
	return mSampleType;
}

double SampleReference::getSampleLength() const
{
	return mSampleLength;
}

StringArray* SampleReference::getSampleTags()
{
	return &mSampleTags;
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

	AudioFormatReader* reader = afm->createReaderFor(mSampleFile);
	if (reader != nullptr)
	{
		mThumbnail->setSource(new FileInputSource(mSampleFile));
		mSampleLength = (float)reader->lengthInSamples / reader->sampleRate;
	}
	delete reader;
	
}

void SampleReference::determineSampleType()
{
	if (mSampleLength > 1.5)
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


void SampleReference::saveSamplePropertiesFile()
{
	if (mSamplePropertiesFile.existsAsFile())
	{
		mSamplePropertiesFile.deleteFile();
	}
	mSamplePropertiesFile.create(); 
	mSamplePropertiesFile.appendText(String(ProjectInfo::versionNumber) + "\n");
	mSamplePropertiesFile.appendText(String(mSampleTags.size()) + "\n");
	for (int i = 0; i < mSampleTags.size(); i++)
	{
		mSamplePropertiesFile.appendText(mSampleTags[i] + "\n");
	}
	
}

void SampleReference::loadSamplePropertiesFile()
{
	if (mSamplePropertiesFile.existsAsFile())
	{
		StringArray propFileLines;
		mSamplePropertiesFile.readLines(propFileLines);
		if (std::stoi(propFileLines[0].toStdString()) <= ProjectInfo::versionNumber)
		{
			int tagCount = std::stoi(propFileLines[1].toStdString());
			for (int i = 0; i < tagCount; i++)
			{
				mSampleTags.add(propFileLines[i + 2]);
			}
		}
		else
		{
			mSamplePropertiesFile.deleteFile();
		}
	}
}

bool SampleReference::operator==(const SampleReference& other)
{
	if (mSampleFile == other.mSampleFile)
	{
		return true;
	}
	else
	{
		return false;
	}
}

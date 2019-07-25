#include "Sample.h"
#include <string>
#include "SamplifyProperties.h"

using namespace samplify;

Sample::Sample() : mFile(""), mPropertiesFile("")
{
	delete this;
}

Sample::Sample(const File& file) : mFile(file), mPropertiesFile(file.getFullPathName() + ".samplify")
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
	if (mThumbnail.get() != nullptr)
	{
		float peak = mThumbnail.get()->getApproximatePeak();
		float lhsMin, lhsMax, rhsMin, rhsMax;
		mThumbnail.get()->getApproximateMinMax(0, (mThumbnail.get()->getTotalLength() / 8)*7, 0, lhsMin, lhsMax);
		mThumbnail.get()->getApproximateMinMax((mThumbnail.get()->getTotalLength() / 8)*7, mThumbnail.get()->getTotalLength() / 8, 0, rhsMin, rhsMax);
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



StringArray Sample::Reference::getRelativeParentFolders() const
{
	jassert(!isNull());
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

String Sample::Reference::getRelativePathName() const
{
	jassert(!isNull());
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

String Sample::Reference::getFullPathName() const
{
	jassert(!isNull());
	return mSample->mFile.getFullPathName();
}



void Sample::Reference::generateThumbnailAndCache()
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

Sample::List::List(const std::vector<Sample::Reference>& list)
{
	addSamples(list);
}

Sample::List::List()
{
}

int Sample::List::size() const
{
	return mSamples.size();
}

void Sample::List::addSample(Sample::Reference sample)
{
	mSamples.push_back(sample);
}

void Sample::List::addSamples(const Sample::List& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		addSample(list[i]);
	}
}

void Sample::List::addSamples(std::vector<Sample::Reference> samples)
{
	addSamples(Sample::List(samples));
}

void Sample::List::removeSample(Sample::Reference sample)
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		if (mSamples[i] == sample)
		{
			removeSample(i);
			i--;
			break;
		}
	}
}

void Sample::List::removeSample(int index)
{
	mSamples.erase(mSamples.begin() + index);
}

void Sample::List::removeSamples(std::vector<Sample::Reference> samples)
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		for (int j = 0; j < samples.size(); j++)
		{
			if (mSamples[i] == samples[j])
			{
				removeSample(i);
				i--;
				break;
			}
		}
	}
}

void Sample::List::removeSamples(const Sample::List& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		removeSample(list[i]);
	}
}

void Sample::List::clearSamples()
{
	mSamples.clear();
}

Sample::Reference Sample::List::operator[](int index) const
{
	return mSamples[index];
}

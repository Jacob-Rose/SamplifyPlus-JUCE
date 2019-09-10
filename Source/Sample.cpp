#include "Sample.h"
#include <string>
#include "SamplifyProperties.h"

using namespace samplify;

Sample::Sample(const File& file) : 
	mFile(file),
	mPropertiesFile(getPropertiesFileFromSampleFile(mFile))
{
	if (mPropertiesFile.exists())
	{
		loadMetadata(); //if metadata is invalid, generateFirstMetadata() is called
	}
	else
	{
		generateMetadata();
	}
}

Sample::~Sample()
{
}

bool samplify::Sample::isContainedInSearch(juce::String query)
{
	if (mFile.getFileName().containsIgnoreCase(query) 
		|| mTags.contains(query) 
		|| mTags.contains(query.substring(1)))
	{
		return true;
	}
	return false;
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
		mThumbnail.get()->getApproximateMinMax(0, (mThumbnail.get()->getTotalLength() / 4), 0, lhsMin, lhsMax);
		mThumbnail.get()->getApproximateMinMax((mThumbnail.get()->getTotalLength() / 4)*3, mThumbnail.get()->getTotalLength() / 4, 0, rhsMin, rhsMax);
		if (lhsMax / 4 > rhsMax)
		{
			mSampleType = SampleType::ONESHOT;
		}
		else
		{
			mSampleType = SampleType::LOOP;
		}
	}
}

void Sample::changeListenerCallback(ChangeBroadcaster * source)
{
	determineSampleType();
	sendChangeMessage();
}


void Sample::saveMetadata()
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
	mPropertiesFile.appendText("#TAGEND");
}

void Sample::loadMetadata()
{
	jassert(mPropertiesFile.exists());
	StringArray propFileLines;
	mPropertiesFile.readLines(propFileLines);
	int savedVersion = std::stoi(propFileLines[0].toStdString());
	if (ProjectInfo::versionNumber == savedVersion)
	{
		int line = 1;
		while (propFileLines[line].toStdString() != "#TAGEND")
		{
			mTags.add(propFileLines[line]);
			line++;
		}
		line++;
	}
	else
	{
		mPropertiesFile.deleteFile();
	}
	
}

void Sample::generateMetadata()
{
}



StringArray Sample::Reference::getRelativeParentFolders() const
{
	jassert(!isNull());
	std::shared_ptr<Sample> sample = mSample.lock();
	StringArray folders;
	File file(sample->mFile);
	File root;
	std::vector<File> rootDirs = SamplifyProperties::getInstance()->getDirectoryLibrary().getDirectories();
	for (int i = 0; i < rootDirs.size(); i++)
	{
		if (file.isAChildOf(rootDirs[i]))
		{
			root = rootDirs[i];
			break;
		}
	}
	while (file.isAChildOf(root))
	{
		file = file.getParentDirectory();
		folders.add(file.getFileName());
	}
	return folders;
}

String Sample::Reference::getRelativePathName() const
{
	jassert(!isNull());
	std::shared_ptr<Sample> sample = mSample.lock();
	String path = sample->mFile.getFullPathName();
	std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectoryLibrary().getDirectories();
	for (int i = 0; i < dirs.size(); i++)
	{
		if (path.contains(dirs[i].getFullPathName()))
		{
			return path.substring(dirs[i].getFullPathName().length());
		}
	}
	return sample->mFile.getFullPathName();
}

String Sample::Reference::getFullPathName() const
{
	jassert(!isNull());
	return mSample.lock()->mFile.getFullPathName();
}

Sample::SampleType Sample::Reference::getSampleType() const
{
	jassert(!isNull());
	return mSample.lock()->mSampleType;
}

double Sample::Reference::getLength() const
{
	jassert(!isNull());
	return mSample.lock()->mLength;
}



StringArray Sample::Reference::getTags() const
{
	jassert(!isNull());
	return mSample.lock()->mTags;
}

void Sample::Reference::addTag(juce::String tag)
{
	if (!isNull())
	{
		std::shared_ptr<Sample> sample = mSample.lock();
		if (!sample->mTags.contains(tag))
		{
			sample->mTags.add(tag);
			sample->saveMetadata();
		}
	}
}

void Sample::Reference::removeTag(juce::String tag)
{
	if (!isNull())
	{
		std::shared_ptr<Sample> sample = mSample.lock();
		if (sample->mTags.contains(tag))
		{
			sample->mTags.remove(sample->mTags.indexOf(tag, true));
			sample->saveMetadata();
		}
	}
}

void Sample::Reference::addChangeListener(ChangeListener* listener)
{
	if (!isNull())
	{
		std::shared_ptr<Sample> sample = mSample.lock();
		sample->addChangeListener(listener);
	}
}

void Sample::Reference::removeChangeListener(ChangeListener* listener)
{
	if (!isNull())
	{
		std::shared_ptr<Sample> sample = mSample.lock();
		sample->removeChangeListener(listener);
	}
}

void Sample::Reference::renameFile(String name)
{
	//todo test on old library
	std::shared_ptr<Sample> sample = mSample.lock();
	sample->mFile.moveFileTo(sample->mFile.getSiblingFile(name));
	sample->mFile = sample->mFile.getSiblingFile(name);
}

File Sample::getPropertiesFileFromSampleFile(const File& sampleFile)
{
	String rootDir = sampleFile.getRelativePathFrom(SamplifyProperties::getInstance()->getDirectoryLibrary().getRelativeDirectoryForFile(sampleFile));
	File f(SamplifyProperties::getInstance()->getUserSettings()->getFile().getParentDirectory().getFullPathName()
		+ File::getSeparatorString()
		+ rootDir.substring(0,rootDir.lastIndexOf("."))
		+ ".samplify");
	return f;
}
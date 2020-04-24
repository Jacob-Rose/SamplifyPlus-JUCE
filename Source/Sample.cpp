#include "Sample.h"
#include <string>
#include "SamplifyProperties.h"

using namespace samplify;

Sample::Sample(const File& file) : mFile(file),
mPropertiesFile(getPropertiesFile(mFile))
{
	if (mPropertiesFile.exists())
	{
		loadPropertiesFile();
	}
	else
	{
		//todo add what to do for new files
		determineSampleType();
	}
}

bool Sample::isPropertiesFileValid()
{
	return mPropertiesFile.existsAsFile();
}
bool samplify::Sample::isQueryValid(juce::String query)
{
	if(mFile.getFullPathName().containsIgnoreCase(query) || mTags.contains("#" + query))
	{
		return true;
	}
	return false;
}
Sample::~Sample()
{
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


void Sample::savePropertiesFile()
{
	if (mPropertiesFile.existsAsFile())
	{
		mPropertiesFile.deleteFile();
	}
	if (mPropertiesFile.create().ok)
	{
		mPropertiesFile.appendText(String(ProjectInfo::versionNumber) + "\n");
		mPropertiesFile.appendText(String(mTags.size()) + "\n");
		for (int i = 0; i < mTags.size(); i++)
		{
			mPropertiesFile.appendText(mTags[i] + "\n");
		}
		mPropertiesFile.appendText("#TAGEND");
		//TODO save cue points
		mPropertiesFile.appendText("#CUEEND");
		mPropertiesFile.appendText(mColor.toString());
	}
	
}

void Sample::loadPropertiesFile()
{
	jassert(mPropertiesFile.exists());
	StringArray propFileLines;
	mPropertiesFile.readLines(propFileLines);
	int savedVersion = std::stoi(propFileLines[0].toStdString());
	int line = 1; //read the first line to verify version
	while (propFileLines[line].toStdString() != "#TAGEND")
	{
		mTags.add(propFileLines[line]);
		line++;
	}
	line++; 
	while (propFileLines[line].toStdString() != "#CUEEND")
	{
		juce::String cueName = propFileLines[line];
		line++;
		double cueTime = std::stod(propFileLines[line].toStdString());
		line++;
		mCuePoints[cueName] = cueTime;
	}
	line++; //cueend line++
	mColor = Colour::fromString(propFileLines[line]);
	line++;
}



StringArray Sample::Reference::getRelativeParentFolders() const
{
	jassert(!isNull());
	std::shared_ptr<Sample> sample = mSample.lock();
	StringArray folders;
	File file(sample->mFile);
	File root;
	std::vector<File> rootDirs = SamplifyProperties::getInstance()->getSampleDirectoryManager()->getDirectories();
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
	std::vector<File> dirs = SamplifyProperties::getInstance()->getSampleDirectoryManager()->getDirectories();
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
			sample->savePropertiesFile();
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
			sample->savePropertiesFile();
		}
	}
}
void Sample::Reference::generateThumbnailAndCache()
{
	if (!isNull())
	{
		std::shared_ptr<Sample> sample = mSample.lock();
		sample->mThumbnailCache = std::make_shared<AudioThumbnailCache>(1);
		AudioFormatManager* afm = SamplifyProperties::getInstance()->getAudioPlayer()->getFormatManager();
		sample->mThumbnail = std::make_shared<SampleAudioThumbnail>(512, *afm, *sample->mThumbnailCache);
		sample->mThumbnail->addChangeListener(sample->getChangeListener());
		AudioFormatReader* reader = afm->createReaderFor(sample->mFile);
		if (reader != nullptr)
		{
			sample->mThumbnail->setSource(new FileInputSource(sample->mFile));
			sample->mLength = (float)reader->lengthInSamples / reader->sampleRate;
		}
		delete reader;
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

File samplify::Sample::getPropertiesFile(const File& sampleFile)
{
	String rootDir = sampleFile.getRelativePathFrom(SamplifyProperties::getInstance()->getSampleDirectoryManager()->getRelativeDirectoryForFile(sampleFile));
	File f(SamplifyProperties::getInstance()->getUserSettings()->getFile().getParentDirectory().getFullPathName()
		+ File::getSeparatorString()
		+ rootDir.substring(0,rootDir.lastIndexOf("."))
		+ ".samplify");
	return f;
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

void Sample::List::addSample(const Sample::Reference& sample)
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

void Sample::List::addSamples(const std::vector<Sample::Reference>& samples)
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

void samplify::Sample::List::randomize()
{
	std::vector<Sample::Reference> newSamps;
	Sample::List currentSamples = mSamples;
	srand(time(NULL));
	while (currentSamples.size() > 0)
	{
		int ranInt = rand();
		int randIndex = ranInt % currentSamples.size();
		newSamps.push_back(currentSamples[randIndex]);
		currentSamples.removeSample(randIndex);
	}
	mSamples = newSamps;
}

void Sample::List::operator+=(const Sample::List& toAdd)
{
	addSamples(toAdd);
}

Sample::Reference Sample::List::operator[](int index) const
{
	return mSamples[index];
}
/*
bool Sample::getSortBool(Sample::Reference lhs, Sample::Reference rhs, Sample::SortMethod method)
{
	switch (method)
	{
	case Sample::SortMethod::Alphabetical:
		if (lhs.getFilename().compareNatural(rhs.getFilename()) > 0)
		{
			return true;
		}
		return false;
		break;
	case Sample::SortMethod::ReverseAlphabetical:
		if (rhs.getFilename().compareNatural(rhs.getFilename()) < 0)
		{
			return true;
		}
		return false;
		break;
	case Sample::SortMethod::Newest:
		if (lhs.getFile().getCreationTime() > rhs.getFile().getCreationTime())
		{
			return true;
		}
		return false;
		break;
	case Sample::SortMethod::Oldest:
		if (lhs.getFile().getCreationTime() < rhs.getFile().getCreationTime())
		{
			return true;
		}
		return false;
		break;
	case Sample::SortMethod::Random:
		return false;
		break;
	}
}
void Sample::SortedLists::Alphabetical::addSample(const Sample::Reference& sample)
{
	int index = 0;
	while (mSamples.size() > index && getSortBool(sample, mSamples[index], SortMethod::Alphabetical))
	{
		index++;
	}
	mSamples.insert(mSamples.begin() + index, sample);
}

void Sample::SortedLists::RevAlphabetical::addSample(const Sample::Reference& sample)
{
	int index = 0;
	while (mSamples.size() > index && getSortBool(sample, mSamples[index], SortMethod::ReverseAlphabetical))
	{
		index++;
	}
	mSamples.insert(mSamples.begin() + index, sample);
}

void Sample::SortedLists::Newest::addSample(const Sample::Reference& sample)
{
	int index = 0;
	while (mSamples.size() > index && getSortBool(sample, mSamples[index], SortMethod::Newest))
	{
		index++;
	}
	mSamples.insert(mSamples.begin() + index, sample);
}

void Sample::SortedLists::Oldest::addSample(const Sample::Reference& sample)
{
	int index = 0;
	while (mSamples.size() > index && getSortBool(sample, mSamples[index], SortMethod::Oldest))
	{
		index++;
	}
	mSamples.insert(mSamples.begin() + index, sample);
}
void Sample::SortedLists::Random::addSample(const Sample::Reference& sample)
{
	/*todo fix bug
	if (mSamples.size() > 0)
	{
		int randPlace = rand() % mSamples.size();
		mSamples.insert(mSamples.begin() + randPlace, sample);
	}
	else
	{
		mSamples.push_back(sample);
	}
	
	mSamples.push_back(sample);
}
*/
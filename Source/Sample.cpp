#include "Sample.h"
#include <string>
#include "SamplifyProperties.h"

using namespace samplify;

Sample::Sample(const File& file) : mFile(file)
{
	mPropertiesFile.reset(getPropertiesFile(mFile));
	if (mPropertiesFile->isValidFile())
	{
		loadPropertiesFile();
	}
	else
	{
		//todo add what to do for new files

	}
}

Sample::~Sample()
{

}

bool Sample::isPropertiesFileValid()
{
	return mPropertiesFile->isValidFile();
}

bool Sample::isQueryValid(juce::String query)
{
	if(mFile.getFullPathName().containsIgnoreCase(query) || mTags.contains("#" + query))
	{
		return true;
	}
	return false;
}

/* deprecated
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
*/

void Sample::changeListenerCallback(ChangeBroadcaster * source)
{
	sendChangeMessage();
}

PropertiesFile* Sample::getPropertiesFile(const File& sampleFile)
{
	String folderName = sampleFile.getFullPathName();
	PropertiesFile::Options options = PropertiesFile::Options();
	//options.folderName = sampleFile.get
	options.applicationName = "SampleProperties";
	options.filenameSuffix = ".sample";
	options.commonToAllUsers = false;
	options.folderName = "SamplifyPlus";
	options.osxLibrarySubFolder = "Application Support/SamplifyPlus";
	/*
	else if (SystemStats::getOperatingSystemType() == SystemStats::OperatingSystemType::Windows)
	{
	}
	*/
	String path = options.getDefaultFile().getFullPathName();
	PropertiesFile* file = new PropertiesFile(File(path.substring(0, path.length() - options.getDefaultFile().getFileName().length()) + String(File::getSeparatorString()) + "SampleProperties").getChildFile(sampleFile.getFullPathName().removeCharacters("\\:") + ".sample"), options);
	return file;
}

void Sample::savePropertiesFile()
{
	if (mPropertiesFile->isValidFile())
	{
		mPropertiesFile->clear();
		mPropertiesFile->setValue("VersionNumber", String(ProjectInfo::versionNumber));
		mPropertiesFile->setValue("TagCount", (int)mTags.size());
		for (int i = 0; i < mTags.size(); i++)
		{
			mPropertiesFile->setValue("Tag" + String(i), mTags[i]);
		}
		mPropertiesFile->setValue("Color", mColor.toString());
		mPropertiesFile->setValue("Description", mInformationDescription);
	}

}

void Sample::loadPropertiesFile()
{
	if (mPropertiesFile->isValidFile())
	{
		if (mPropertiesFile->getValue("VersionNumber") == String(ProjectInfo::versionNumber))
		{
			int count = mPropertiesFile->getIntValue("TagCount");
			for (int i = 0; i < count; i++)
			{
				String tag = mPropertiesFile->getValue("Tag" + String(i));
				mTags.add(tag);
			}
			mColor = Colour::fromString(mPropertiesFile->getValue("Color"));
			mInformationDescription = mPropertiesFile->getValue("Description");
		}
		else
		{
			mPropertiesFile->clear();
			//In future, offer port for each version here
		}
	}
}



StringArray Sample::Reference::getRelativeParentFolders() const
{
	jassert(!isNull());
	std::shared_ptr<Sample> sample = mSample.lock();
	StringArray folders;
	File file(sample->mFile);
	File root = SamplifyProperties::getInstance()->getSampleLibrary()->getRelativeDirectoryForFile(file);
	while (file.isAChildOf(root))
	{
		file = file.getParentDirectory();
		folders.add(file.getFileName());
	}
	return folders;
}


Sample::Reference::Reference(std::shared_ptr<Sample> sample)
{
	mSample = sample;
}

Sample::Reference::Reference(nullptr_t null) : mSample() { jassert(isNull()); }

Sample::Reference::Reference(const Sample::Reference& ref)
{
	mSample = ref.mSample;
}

std::shared_ptr<SampleAudioThumbnail> Sample::Reference::getThumbnail() const
{
	jassert(!isNull());
	return mSample.lock()->mThumbnail;
}

File Sample::Reference::getFile() const
{
	jassert(!isNull());
	return mSample.lock()->mFile;
}

String Sample::Reference::getInfoText() const
{
	jassert(!isNull());
	return mSample.lock()->mInformationDescription;
}

void Sample::Reference::setInfoText(String newText) const
{
	jassert(!isNull());
	newText = newText.removeCharacters("\n"); //prevent errors, might need to remove more too
	std::shared_ptr<Sample> sample = mSample.lock();

	sample->mInformationDescription =newText;
	sample->savePropertiesFile();
}

Colour samplify::Sample::Reference::getColor() const
{
	jassert(!isNull());
	return mSample.lock()->mColor;
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
	std::shared_ptr<Sample> sample = mSample.lock();
	if (!isNull() && sample->mThumbnail == nullptr)
	{
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

void Sample::List::addSample(const Sample::Reference sample)
{
	if (mListSortingMethod == SortingMethod::None)
	{
		mSamples.push_back(sample);
		return;
	}
	for (int i = 0; i < mSamples.size(); i++)
	{
		if (mSamples[i].getValueForSortType(mListSortingMethod) < sample.getValueForSortType(mListSortingMethod))
		{
			mSamples.insert(mSamples.begin() + i, sample);
			return;
		}
	}
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

void Sample::List::clear()
{
	mSamples.clear();
}

void samplify::Sample::List::sort(SortingMethod method)
{
	if (method == SortingMethod::Random)
	{
		randomize();
	}
	else if (method == SortingMethod::Newest 
		|| method == SortingMethod::Oldest)
	{
		quickSort(method, 0, mSamples.size() - 1);
	}
}
//https://www.geeksforgeeks.org/quick-sort/
void Sample::List::quickSort(SortingMethod method, int low, int high)
{
	if (low < high)
	{
		int pivotIndex = partition(method, low, high);

		quickSort(method, low, pivotIndex - 1);
		quickSort(method, pivotIndex + 1, high);
	}

}
//https://www.geeksforgeeks.org/quick-sort/
int Sample::List::partition(SortingMethod method, int low, int high)
{
	int pivotIndex = ((high - low)/2) + low;
	float pivotValue = mSamples[pivotIndex].getValueForSortType(method);

	int i = (low - 1);

	for (int j = low; j <= high -1; j++)
	{
		float val = mSamples[j].getValueForSortType(method);
		if (val <= pivotValue)
		{
			i++;
			Sample::Reference tmp = mSamples[i];
			mSamples[i] = mSamples[j];
			mSamples[j] = tmp;
		}
	}
	Sample::Reference tmp = mSamples[(i + 1)];
	mSamples[i+1] = mSamples[pivotIndex];
	mSamples[pivotIndex] = tmp;
	return i + 1;
}

float Sample::getValueForSortType(SortingMethod method) const
{
	if (method == SortingMethod::Newest)
	{
		return mFile.getCreationTime().toMilliseconds();
	}
	else if (method == SortingMethod::Oldest)
	{
		return -mFile.getCreationTime().toMilliseconds();
	}
	return 0.0f;
}

void Sample::List::randomize()
{
	srand(time(NULL));

	for (int i = 0; i < mSamples.size(); i++)
	{
		int ranInt = rand();
		int randIndex = ranInt % mSamples.size();
		if (i != randIndex) //just in case
		{
			Sample::Reference tmp = mSamples[i];
			mSamples[i] = mSamples[randIndex];
			mSamples[randIndex] = tmp;
		}
	}
}


void Sample::List::operator+=(const Sample::List& toAdd)
{
	addSamples(toAdd);
}

Sample::Reference Sample::List::operator[](int index) const
{
	return mSamples[index];
}

void Sample::List::operator=(const Sample::List& other)
{
	mSamples = other.mSamples;
}

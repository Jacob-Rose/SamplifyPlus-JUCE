#include "SampleLibrary.h"
#include "SamplifyMainComponent.h"

using namespace samplify;

SampleLibrary::SampleLibrary()
{
}

SampleLibrary::~SampleLibrary()
{
}

void SampleLibrary::addSample(const File& file)
{
	//check if already exist
	if (!containsSample(file))
	{
		std::shared_ptr<Sample> ref = std::make_shared<Sample>(file);
		mSamples.push_back(ref);
	}
}

void SampleLibrary::addSamples(std::vector<File> files)
{
	for (int i = 0; i < files.size(); i++)
	{
		addSample(files[i]);
	}
}

void SampleLibrary::addSamples(std::vector<std::shared_ptr<Sample>> files)
{
	for (int i = 0; i < files.size(); i++)
	{
		addSample(files[i]);
	}
}

void SampleLibrary::removeSample(const File& ref)
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		if (Sample::Reference(mSamples[i]).getFile() == ref)
		{
			mSamples.erase(mSamples.begin() + i);
			return;
		}
	}
}

void SampleLibrary::sortSamples(Sample::SortMethod method)
{
	if (currentUpdateThread.get() != nullptr)
	{
		currentUpdateThread->stopThread(1000);
	}
	currentUpdateThread.reset(new SortSamplesThread(this, method));
	currentUpdateThread->startThread();
	currentUpdateThread->addListener(this);
}

void SampleLibrary::randomizeSamples()
{
	Sample::List newSamps;
	Sample::List currentSamples = getCurrentSamples();
	srand(time(NULL));
	while(currentSamples.size() > 0)
	{
		int ranInt = rand();
		int randIndex = ranInt % currentSamples.size();
		newSamps.addSample(currentSamples[randIndex]);
		currentSamples.removeSample(randIndex);
	}
	setCurrentSamples(newSamps);
}

void SampleLibrary::addSample(std::shared_ptr<Sample> ref)
{
	if (!containsSample(Sample::Reference(ref).getFile()))
	{
		mSamples.push_back(ref);
		sendChangeMessage();
	}
}

bool SampleLibrary::containsSample(File file)
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		if (Sample::Reference(mSamples[i]).getFile() == file)
		{
			return true;
		}
	}
	return false;
}


void SampleLibrary::exitSignalSent()
{
	sendChangeMessage();
}



void SampleLibrary::checkThreadFinished()
{
	if (currentUpdateThread.get() != nullptr && !currentUpdateThread.get()->isThreadRunning())
	{
		currentUpdateThread.reset(nullptr);
	}
}

void SampleLibrary::updateCurrentSamples(File path, String query)
{
	mCurrentQuery = query;
	mCurrentDirectory = path;
	if (currentUpdateThread != nullptr)
	{
		currentUpdateThread->stopThread(1000);
	}
	//std::weak_ptr<SampleLibrary> sharedThis = std::make_shared<SampleLibrary>(this);
	currentUpdateThread.reset(new UpdateSamplesThread(this));
	currentUpdateThread->startThread();
	currentUpdateThread->addListener(this);
}

void SampleLibrary::updateCurrentSamples(File path)
{
	updateCurrentSamples(path, mCurrentQuery);
}

void SampleLibrary::updateCurrentSamples(String query)
{
	updateCurrentSamples(mCurrentDirectory, query);
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* source)
{
	if (DirectoryLibrary* dirLib = dynamic_cast<DirectoryLibrary*>(source))
	{
		//directory added
		if (dirLib->getDirectories().size() > dirLib->getLastDirectories().size())
		{
			std::vector<File> addedDirs = dirLib->getDirectories();
			for (int i = 0; i < dirLib->getLastDirectories().size(); i++)
			{
				for (int j = 0; j < addedDirs.size(); j++)
				{
					if (addedDirs[j] == dirLib->getLastDirectories()[i])
					{
						addedDirs.erase(addedDirs.begin() + j);
					}
				}
			}
			for (int i = 0; i < addedDirs.size(); i++)
			{
				if (addedDirs[i].exists())
				{
					LoadSamplesThread loadSamplesThread(addedDirs[i]);
					loadSamplesThread.runThread();
				}
				else
				{
					AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Directory No Longer Exist", "The directory has failed to be found, please retry the program to load it in, for now the samples have not been loaded");
				}
			}
		}
	}
}

Sample::List SampleLibrary::getCurrentSamples()
{
	if (currentUpdateThread != nullptr && !currentUpdateThread->isThreadRunning())
	{
		currentUpdateThread.get()->waitForThreadToExit(1000);
		currentUpdateThread.reset(nullptr);
	}
	return mCurrentSamples;
}

Sample::List SampleLibrary::getAllSamples()
{
	checkThreadFinished();
	Sample::List list;
	for (int i = 0; i < mSamples.size(); i++)
	{
		Sample::Reference ref = Sample::Reference(mSamples[i]);
		list.addSample(ref);
	}
	return list;
}

StringArray SampleLibrary::getAllTags()
{
	//todo sort on order
	StringArray tags = StringArray();
	for (int i = 0; i < mSamples.size(); i++)
	{
		StringArray sTags = Sample::Reference(mSamples[i]).getTags();
		for (int j = 0; j < sTags.size(); j++)
		{
			if (!tags.contains(sTags[j]))
			{
				tags.add(sTags[j]);
			}
		}
	}
	return tags;
}

void SampleLibrary::setCurrentSamples(Sample::List samples, bool shouldSendChangeMessage)
{
	mCurrentSamples = samples;
	checkThreadFinished();
	
	if (shouldSendChangeMessage)
	{
		sendChangeMessage();
	}
}

void SampleLibrary::UpdateSamplesThread::run()
{
	bool successful = true;
	Sample::SortMethod method = (Sample::SortMethod::None);
	if(SamplifyMainComponent::getInstance() != nullptr)
	{
		method = SamplifyMainComponent::getInstance()->getSampleExplorer().getCurrentSort();
	}
	auto newList = Sample::SortedLists::getSpecializedList(method);
	for (int i = 0; i < mSamples.size() && !threadShouldExit(); i++)
	{
		Sample::Reference ref = Sample::Reference(mParent->mSamples[i]);
		if (ref.getFile().isAChildOf(mParent->mCurrentDirectory) || !mParent->mCurrentDirectory.exists())
		{
			bool isValid = true;
			String tmpQuery = mParent->mCurrentQuery;
			while (tmpQuery.containsChar('#'))
			{
				int first = tmpQuery.indexOfChar('#');
				int last = tmpQuery.substring(first, tmpQuery.length() - first).indexOfChar(' ');
				juce::String tag;
				if (last == -1)
				{
					tag = tmpQuery.substring(first + 1, tmpQuery.length() - (first));
					tmpQuery = "";
				}
				else
				{
					tag = tmpQuery.substring(first + 1, last);
					tmpQuery = tmpQuery.substring(last + 1, tmpQuery.length() - (last + 1));
				}
				isValid = ref.getTags().contains(tag) && isValid;
			}
			if (ref.getFullPathName().containsIgnoreCase(tmpQuery) && isValid)
			{
				newList.addSample(ref);
			}
		}
	}
	if (!threadShouldExit())
	{
		mParent->setCurrentSamples(newList);
		signalThreadShouldExit();
	}
}

void SampleLibrary::SortSamplesThread::run()
{
	Sample::List sorted = Sample::SortedLists::getSpecializedList(mMethod);
	while(mSamples.size() > 0 && !currentThreadShouldExit())
	{
		sorted.addSample(mSamples[0]);
		mSamples.removeSample(0);
		mParent->setCurrentSamples(sorted, false);
	}
	if (threadShouldExit())
	{
		mParent->setCurrentSamples(sorted + mSamples, false);
	}
	else
	{
		mParent->setCurrentSamples(sorted, false);
	}
}

void SampleLibrary::LoadSamplesThread::run()
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
		std::shared_ptr<Sample> ref = std::make_shared<Sample>(mFiles[i]);
		SamplifyProperties::getInstance()->getSampleLibrary().addSample(ref);
		setProgress(((float)i) / count);
		setStatusMessage("calculating sample info...");
	}
	SamplifyProperties::getInstance()->getSampleLibrary().refreshCurrentSamples();
}
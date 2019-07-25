#include "SampleLibrary.h"

using namespace samplify;

SampleLibrary::SampleLibrary()
{
}

SampleLibrary::SampleLibrary(const SampleLibrary&)
{
	
}

SampleLibrary::~SampleLibrary()
{

}


void SampleLibrary::addSample(File file)
{
	//check if already exist
	Sample ref(file);
	if(!containsSample(file))
		mSamples.push_back(ref);
}

void SampleLibrary::addSamples(std::vector<File> files)
{
	for (int i = 0; i < files.size(); i++)
	{
		addSample(files[i]);
	}
}

void SampleLibrary::addSamples(std::vector<Sample> files)
{
	for (int i = 0; i < files.size(); i++)
	{
		addSample(files[i]);
	}
}

void SampleLibrary::removeSample(const File& ref)
{
	/*
	for (int i = 0; i < mSamples.size(); i++)
	{
		if (Sample::Reference(&mSamples[i]).getFile() == ref)
		{
			mSamples.erase(mSamples.begin() + i);
			return;
		}
	}
	*/
}

void SampleLibrary::addSample(Sample& ref)
{
	if (!containsSample(Sample::Reference(&ref).getFile()))
		mSamples.push_back(ref);
	sendChangeMessage();
}

void SampleLibrary::saveSamplePropertyFiles()
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		mSamples[i].savePropertiesFile();
	}
}

bool SampleLibrary::containsSample(File file)
{
	return false;
}


void SampleLibrary::exitSignalSent()
{
	sendChangeMessage();
}

void SampleLibrary::updateCurrentSamples(File path, String query)
{
	mCurrentSamples.clearSamples();
	Sample::List allSamples;
	for (int i = 0; i < mSamples.size(); i++)
	{
		allSamples.addSample(&mSamples[i]);
	}
	mCurrentQuery = query;
	mCurrentDirectory = path;
	if (updateThread != nullptr)
	{
		updateThread->stopThread(1000);
		delete updateThread;
	}
	updateThread = new UpdateSamplesThread(this);
	updateThread->startThread();
	updateThread->addListener(this);

}

void SampleLibrary::updateCurrentSamples(File path)
{
	updateCurrentSamples(path, mCurrentQuery);
}

void SampleLibrary::updateCurrentSamples(String query)
{
	updateCurrentSamples(mCurrentDirectory, query);
}

Sample::List SampleLibrary::getCurrentSamples()
{
	if (updateThread != nullptr)
	{
		if (!updateThread->isThreadRunning())
		{
			delete updateThread;
			updateThread = nullptr;
		}
	}

	return mCurrentSamples;
}

Sample::List samplify::SampleLibrary::getAllSamples()
{
	Sample::List list;
	for (int i = 0; i < mSamples.size(); i++)
	{
		list.addSample(Sample::Reference(&mSamples[i]));
	}
	return list;
}

StringArray SampleLibrary::getAllTags()
{
	//todo sort on order
	StringArray tags = StringArray();
	for (int i = 0; i < mSamples.size(); i++)
	{
		StringArray sTags = Sample::Reference(&mSamples[i]).getTags();
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

void SampleLibrary::setCurrentSamples(Sample::List samples)
{
	mCurrentSamples = samples;
	sendChangeMessage();
}

void SampleLibrary::UpdateSamplesThread::run()
{
	for (int i = 0; i < mParent->mSamples.size(); i++)
	{
		if (threadShouldExit())
			break;
		Sample::Reference ref = Sample::Reference(&mParent->mSamples[i]);
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
			if (isValid)
			{
				int i = 0;
			}
			if (ref.getFullPathName().containsIgnoreCase(tmpQuery) && isValid)
			{
				mParent->mCurrentSamples.addSample(ref);
				//mParent->setCurrentSamples();
			}

		}
	}
	signalThreadShouldExit();
}
/*
void samplify::SampleLibrary::SortSamplesThread::run()
{
	SampleList sorted;
	SampleList oldList;
	for (int i = 0; i < mParent->mSamples.size(); i++)
	{
		oldList.addSample(&mParent->mSamples[i]);
	}
	for (int i = 0; i < oldList.getSamples().size(); i++)
	{
		if (threadShouldExit())
		{
			mParent->setCurrentSamples(oldList);
			break;
		}
		Sample* lowest = &mParent->mSamples[i];
		for (int j = 0; j < mParent->mSamples.size(); j++)
		{
			if (SampleList::getSortBool(&mParent->mSamples[i], lowest, mMethod))
			{
				lowest = &mParent->mSamples[j];
			}
		}
		mParent->setCurrentSamples(sorted);
	}

}
*/

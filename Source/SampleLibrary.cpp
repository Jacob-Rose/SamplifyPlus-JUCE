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
	for (int i = 0; i < mSamples.size(); i++)
	{
		//delete mSamples[i];
	}
}


void SampleLibrary::loadSamplesFromDirectory(File path)
{
	File file(path);
	DirectoryIterator iterator(file, true, "*.wav");
	/*auto* dw = new SampleLoadWindow();
	Rectangle<int> area(0, 0, 300, 400);

	RectanglePlacement placement((true ? RectanglePlacement::xLeft
		: RectanglePlacement::xRight)
		| RectanglePlacement::yTop
		| RectanglePlacement::doNotResize);

	auto result = placement.appliedTo(area, Desktop::getInstance().getDisplays()
		.getMainDisplay().userArea.reduced(20));
		*/
	while (iterator.next())
	{
		//todo remove counter
		SampleReference ref(iterator.getFile());
		if (std::find(mSamples.begin(), mSamples.end(), ref) == mSamples.end()) {
			mSamples.push_back(ref);
			//dw->updatePercent(iterator.getEstimatedProgress());
			//dw->repaint();
		}
	}
	updateCurrentSamples("");
	//delete dw;
}

void SampleLibrary::loadSamplesFromDirectories(std::vector<File>& dirs)
{
	for (int i = 0; i < dirs.size(); i++)
	{
		loadSamplesFromDirectory(dirs[i]);
	}
}

void samplify::SampleLibrary::addSample(File file)
{
	//check if already exist
	mSamples.push_back(SampleReference(file));
}

void samplify::SampleLibrary::removeSample(File file)
{
}

void samplify::SampleLibrary::clearSamples()
{
}

void samplify::SampleLibrary::sortCurrentSamples(SortingMethod method)
{
	mCurrentSamples.sortSamples(method);
}

void SampleLibrary::updateCurrentSamples(File path, String query)
{
	mCurrentSamples.clearSamples();
	mDirectorySamples.clearSamples();
	for (int i = 0; i < mSamples.size(); i++)
	{
		SampleReference* ref = &mSamples[i];
		if (ref->getFile().isAChildOf(path))
		{
			mDirectorySamples.addSample(ref);
			if (ref->getFilename().containsIgnoreCase(query))
			{
				mCurrentSamples.addSample(ref);
			}
		}
	}
	mCurrentQuery = query;
	mCurrentDirectory = path;
	sendChangeMessage();
}

void SampleLibrary::updateCurrentSamples(File path)
{
	updateCurrentSamples(path, mCurrentQuery);
}

void SampleLibrary::updateCurrentSamples(String query)
{
	updateCurrentSamples(mCurrentDirectory, query);
}

std::vector<SampleReference*> samplify::SampleLibrary::getAllSamplesInSelectedDirectory()
{
	return mDirectorySamples.getSamples();
}

std::vector<SampleReference*> SampleLibrary::getCurrentSamples()
{
	return mCurrentSamples.getSamples();
}

StringArray SampleLibrary::getAllTags()
{
	//todo sort on order
	StringArray tags = StringArray();
	for (int i = 0; i < mSamples.size(); i++)
	{
		StringArray sTags = mSamples[i].getTags();
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

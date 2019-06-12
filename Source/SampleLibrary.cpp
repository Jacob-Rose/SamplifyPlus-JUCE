/*
  ==============================================================================

    SampleLibrary.cpp
    Created: 31 May 2018 12:45:49pm
    Author:  jacob

  ==============================================================================
*/

#include "SampleLibrary.h"
#include "SamplifyProperties.h"

SampleLibrary::SampleLibrary()
{
	initialize();
}

SampleLibrary::SampleLibrary(const SampleLibrary &)
{
	
}

SampleLibrary::~SampleLibrary()
{
	for (int i = 0; i < mSamples.size(); i++)
	{
		//delete mSamples[i];
	}
}

void SampleLibrary::initialize()
{
	loadSamplesFromDirectory(SamplifyProperties::getInstance()->getDirectories());

	SampleLibrary::updateCurrentSamples("");
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
		SampleReference ref(iterator.getFile());
		if (std::find(mSamples.begin(), mSamples.end(), ref) == mSamples.end()) {
			mSamples.push_back(ref);
			//dw->updatePercent(iterator.getEstimatedProgress());
			//dw->repaint();
		}
	}
	//delete dw;
}

void SampleLibrary::loadSamplesFromDirectory(std::vector<File>& dirs)
{
	for (int i = 0; i < dirs.size(); i++)
	{
		loadSamplesFromDirectory(dirs[i]);
	}
}

void SampleLibrary::updateCurrentSamples(File path, String query)
{
	mCurrentSamples.clear();
	for (int i = 0; i < mSamples.size(); i++)
	{
		SampleReference* ref = &mSamples[i];
		if (ref->getFile().getFullPathName().toStdString().find(path.getFullPathName().toStdString()) != std::string::npos)
		{
			if (ref->getFilename().toLowerCase().toStdString().find(query.toLowerCase().toStdString()) != std::string::npos)
			{
				mCurrentSamples.push_back(ref);
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

std::vector<SampleReference*>* SampleLibrary::getCurrentSamples()
{
	return &mCurrentSamples;
}

StringArray SampleLibrary::getAllTags()
{
	//todo sort on order
	StringArray tags = StringArray();
	for (int i = 0; i < mSamples.size(); i++)
	{
		StringArray* sTags = mSamples[i].getSampleTags();
		for (int j = 0; j < sTags->size(); j++)
		{
			if (!tags.contains((*sTags)[j]))
			{
				tags.add((*sTags)[j]);
			}
		}
		
	}
	return tags;
}

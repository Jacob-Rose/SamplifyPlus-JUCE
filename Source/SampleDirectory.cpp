/*
  ==============================================================================

    SampleDirectory.cpp
    Created: 13 Sep 2019 5:43:59pm
    Author:  jacob

  ==============================================================================
*/

#include "SampleDirectory.h"
using namespace samplify;

SampleDirectory::SampleDirectory(File file)
{
	if (file.exists())
	{
		mCheckStatus = CheckStatus::Enabled;
	}
	else
	{
		mCheckStatus = CheckStatus::NotLoaded;
	}
	//add all child directories as sampleDirectory, then recursively all them do the same for all child folders
	DirectoryIterator dirIter = DirectoryIterator(file, false, "*", File::findDirectories);
	while (dirIter.next())
	{
		std::shared_ptr<SampleDirectory> sampDir = std::make_shared<SampleDirectory>(dirIter.getFile());
		sampDir->addChangeListener(this);
		mChildDirectories.push_back(sampDir);
	}

	//add all child samples in the actual folder
	DirectoryIterator sampleIter = DirectoryIterator(file, false, "*.wav", File::findFiles);
	while (sampleIter.next())
	{
		mChildSamples.push_back(std::make_shared<Sample>(sampleIter.getFile()));
	}
	mDirectory = file;
}

Sample::List samplify::SampleDirectory::getChildSamplesRecursive(juce::String query, bool ignoreCheckSystem)
{
	Sample::List list;
	if (mCheckStatus == CheckStatus::Disabled || mCheckStatus == CheckStatus::NotLoaded)
	{
		return list;
	}

	for (int i = 0; i < mChildDirectories.size(); i++)
	{
		list += mChildDirectories[i]->getChildSamplesRecursive(query, ignoreCheckSystem);
	}
	if (ignoreCheckSystem || mIncludeChildSamples)
	{
		for (int i = 0; i < mChildSamples.size(); i++)
		{
			if(mChildSamples[i]->isQueryValid(query))
				list.addSample(Sample::Reference(mChildSamples[i]));
		}
	}
	return list;
}

Sample::List samplify::SampleDirectory::getChildSamples()
{
	Sample::List list;
	for (int i = 0; i < mChildSamples.size(); i++)
	{
		list.addSample(Sample::Reference(mChildSamples[i]));
	}
	return list;
	
}

void SampleDirectory::updateChildrenItems(CheckStatus checkStatus)
{
	mCheckStatus = checkStatus;
	for (int i = 0; i < mChildDirectories.size(); i++)
	{
		mChildDirectories[i]->setCheckStatus(checkStatus);
	}
}

void samplify::SampleDirectory::changeListenerCallback(ChangeBroadcaster* source)
{
	sendChangeMessage();
}

void SampleDirectory::cycleCurrentCheck()
{
	switch (mCheckStatus)
	{
	case CheckStatus::Mixed:
	case CheckStatus::Enabled:
		setCheckStatus(CheckStatus::Disabled);
		break;
	case CheckStatus::Disabled:
		setCheckStatus(CheckStatus::Enabled);
		break;
	}
	
}

void samplify::SampleDirectory::setCheckStatus(CheckStatus newCheckStatus)
{
	updateChildrenItems(newCheckStatus);
	sendChangeMessage();
}

void samplify::SampleDirectory::recursiveRefresh()
{
	for (int i = 0; i < mChildDirectories.size(); i++)
	{
		mChildDirectories[i]->recursiveRefresh();
	}
	bool foundDisabled = false;
	bool foundEnabled = false;
	for (int i = 0; i < mChildDirectories.size(); i++)
	{
		if (mChildDirectories[i]->getCheckStatus() == CheckStatus::Mixed)
		{
			foundDisabled = true;
			foundEnabled = true;
		}
		else if (mChildDirectories[i]->getCheckStatus() == CheckStatus::Disabled)
		{
			foundDisabled = true;
		}
		else if (mChildDirectories[i]->getCheckStatus() == CheckStatus::Enabled)
		{
			foundEnabled = true;
		}
		if (foundEnabled && foundDisabled)
		{
			mCheckStatus = CheckStatus::Mixed;
		}
		else if (foundEnabled)
		{
			mCheckStatus = CheckStatus::Enabled;
		}
		else if (foundDisabled)
		{
			mCheckStatus = CheckStatus::Disabled;
		}
	}
}

std::shared_ptr<SampleDirectory> samplify::SampleDirectory::getChildDirectory(int index)
{
	return mChildDirectories[index];
}

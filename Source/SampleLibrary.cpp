#include "SampleLibrary.h"
#include "SamplifyMainComponent.h"

using namespace samplify;

SampleLibrary::SampleLibrary()
{
	
}

SampleLibrary::~SampleLibrary()
{
}

void SampleLibrary::updateCurrentSamples(String query)
{
	mCurrentQuery = query;
	/*
	if (updateSampleFuture != nullptr)
	{
		updateSampleFuture->_Abandon();
		updateSampleFuture = nullptr;
	}
	updateSampleFuture = std::make_unique<std::future<Sample::List>>(getAllSamplesInDirectories_Async(query));
	updateSampleFuture->wait();
	if (updateSampleFuture->valid())
		mCurrentSamples = updateSampleFuture->get();
		*/
	mCurrentSamples = getAllSamplesInDirectories(query, false);
	sendChangeMessage();
}

void SampleLibrary::addDirectory(const File& dir)
{
	std::shared_ptr<SampleDirectory> sampDir = std::make_shared<SampleDirectory>(dir);
	sampDir->addChangeListener(this);
	mDirectories.push_back(sampDir);
}

void SampleLibrary::removeDirectory(const File& dir)
{
}

File samplify::SampleLibrary::getRelativeDirectoryForFile(const File& sampleFile) const
{
	for (int i = 0; i < mDirectories.size(); i++)
	{
		if (sampleFile.isAChildOf(mDirectories[i]->getFile()))
		{
			return sampleFile.getRelativePathFrom(mDirectories[i]->getFile());
		}
	}
	return File();
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* source)
{
	refreshCurrentSamples();
}

Sample::List SampleLibrary::getCurrentSamples()
{
	return mCurrentSamples;
}

StringArray samplify::SampleLibrary::getUsedTags()
{
	StringArray tags;
	Sample::List allSamps = getAllSamplesInDirectories("", true);
	for (int i = 0; i < allSamps.size(); i++)
	{
		StringArray sampleTags = allSamps[i].getTags();
		for (int j = 0; j < sampleTags.size(); j++)
		{
			tags.addIfNotAlreadyThere(sampleTags[j]);
		}
	}
	return tags;
}

void SampleLibrary::addTag(juce::String text, Colour color)
{
	mTags.push_back(Tag(text, color));
}

void SampleLibrary::addTag(juce::String text)
{
	Random& r = Random::getSystemRandom();
	addTag(text, Colour(juce::uint8(r.nextInt(Range(0, 256))),
		juce::uint8(r.nextInt(Range(0, 256))),
		juce::uint8(r.nextInt(Range(0, 256)))));
}

void SampleLibrary::deleteTag(juce::String tag)
{
	Sample::List allSamps = getAllSamplesInDirectories("", true);
	for (int i = 0; i < allSamps.size(); i++)
	{
		//does nothing if not contained
		if (allSamps[i].getTags().contains(tag))
		{
			allSamps[i].removeTag(tag);
		}
		
	}
	for (int i = 0; i < mTags.size(); i++)
	{
		if (mTags[i].mTitle == tag)
		{
			mTags.erase(mTags.begin() + i);
		}
		//todo
		//SamplifyMainComponent::getInstance()->getFilterExplorer().getTagExplorer().resized();
	}
}

void SampleLibrary::setTagColor(juce::String tag, juce::Colour newColor)
{
	//todo
}

Sample::List SampleLibrary::getAllSamplesInDirectories(juce::String query, bool ignoreCheckSystem)
{
	Sample::List list;
	for (int i = 0; i < mDirectories.size(); i++)
	{
		list += mDirectories[i]->getChildSamplesRecursive(query, ignoreCheckSystem);
	}
	return list;
}
/*
std::future<Sample::List> SampleLibrary::getAllSamplesInDirectories_Async( juce::String query, bool ignoreCheckSystem)
{
	return std::async(std::launch::async, &getAllSamplesInDirectories, this, query, ignoreCheckSystem); //getAllSamplesInDirectories(query, getCount); });
}
*/
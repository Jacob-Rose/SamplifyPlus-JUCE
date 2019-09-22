/*
  ==============================================================================

    TagLibrary.cpp
    Created: 21 Sep 2019 5:39:32pm
    Author:  student

  ==============================================================================
*/

#include "TagLibrary.h"

#include "Sample.h"

using namespace samplify;

void TagLibrary::addTag(juce::String text, Colour color)
{
	mSampleTagColors[text] = color;
}

void TagLibrary::addTag(juce::String text)
{
	Random& r = Random::getSystemRandom();
	addTag(text, Colour(juce::uint8(r.nextInt(Range(0, 256))),
		juce::uint8(r.nextInt(Range(0, 256))),
		juce::uint8(r.nextInt(Range(0, 256)))));
}

void TagLibrary::renameTag(juce::String currentTagName, juce::String desiredName)
{
	/*
	Sample::List allSamps = SamplifyProperties::getInstance()->mSampleLibrary.getAllSamples();
	Sample::List taggedSamps;
	for (int i = 0; i < allSamps.size(); i++)
	{
		//does nothing if not contained
		if (allSamps[i].getTags().contains(currentTagName))
		{
			taggedSamps.addSample(allSamps[i]);
			allSamps[i].removeTag(currentTagName);
		}
	}
	std::map<juce::String, Colour>::iterator it = mSampleTagColors.find(currentTagName);
	if (it != mSampleTagColors.end())
	{
		mSampleTagColors[desiredName] = it->second;
		mSampleTagColors.erase(it);
		//heavily specialized line, im sorry
	}
	for (int i = 0; i < taggedSamps.size(); i++)
	{
		taggedSamps[i].addTag(desiredName);
	}
	*/
}

void TagLibrary::deleteTag(juce::String tag)
{
	/*
	Sample::List allSamps = SamplifyProperties::getInstance()->mSampleLibrary.getAllSamples();
	for (int i = 0; i < allSamps.size(); i++)
	{
		//does nothing if not contained
		allSamps[i].removeTag(tag);
	}
	std::map<juce::String, Colour>::iterator it = mSampleTagColors.find(tag);
	if (it != mSampleTagColors.end())
	{
		mSampleTagColors.erase(it);
		//heavily specialized line, im sorry
		SamplifyMainComponent::getInstance()->getFilterExplorer().getTagExplorer().resized();
	}
	*/
}

void TagLibrary::saveTags(PropertiesFile* propFile)
{
	/*
	if (propFile->isValidFile())
	{
		int tagCount = 0;
		StringArray usedTags = mSampleLibrary.getAllTags();
		std::map<String, Colour>::iterator it = mTagLibrary.mSampleTagColors.begin();
		while (it != mTagLibrary.mSampleTagColors.end())
		{
			if (usedTags.contains(it->first))
			{
				propFile->setValue("tag " + String(tagCount), &it->first);
				propFile->setValue("tag " + it->first, it->second.toString());
				tagCount++;
			}
		}
		propFile->setValue("tag count", tagCount);
		propFile->saveIfNeeded();
	}
	else
	{
		throw "Properties File is not valid file";
	}
	*/
}

Colour TagLibrary::getTagColor(juce::String text)
{
	if (mSampleTagColors.find(text) != mSampleTagColors.end())
	{
		return mSampleTagColors[text];
	}
	else
	{
		addTag(text);
		return mSampleTagColors[text];
	}
}
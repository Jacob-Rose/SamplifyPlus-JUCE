#include "TagExplorer.h"
#include "SamplifyProperties.h"
#include "SampleReference.h"

TagExplorer::TagExplorer()
{
	mNewButtonTag.setButtonText("New Tag");
	mNewButtonTag.onClick = [this] {addNewTag(); };
	addAndMakeVisible(mNewButtonTag);
}

TagExplorer::~TagExplorer()
{
}

void TagExplorer::updateTags(juce::String newSearch)
{
	StringArray allTags = SamplifyProperties::getInstance()->getSampleLibrary()->getAllTags();
	for (int i = 0; i < mNewTags.getTags().size(); i++)
	{
		if (mNotContainedTags.contains(mNewTags[i]))
		{
			mNewTags.remove(i);
			i--;
		}
	}
	for (int i = 0; i < mContainedTags.size(); i++)
	{
		if (!mNotContainedTags[i].contains(newSearch))
		{
			mNotContainedTags.remove(i);
			i--;
		}
		else
		{
			std::vector<SampleReference*> samples = SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples();
			for (int j = 0; j < samples.size(); j++)
			{
				if (samples[j]->getSampleTags()->contains(mNotContainedTags[i], true))
				{
					mContainedTags.add(mNotContainedTags[i]);
					mNotContainedTags.remove(i);
					i--;
					break;
				}
			}
		}
	}
}

void TagExplorer::resized()
{
	mNewButtonTag.setBounds(0,getHeight()-30, getWidth(), 30);
}

void TagExplorer::paint(Graphics&)
{
}

void TagExplorer::addNewTag()
{
}

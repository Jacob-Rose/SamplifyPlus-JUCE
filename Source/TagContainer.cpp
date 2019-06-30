#include "TagContainer.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

TagContainer::TagContainer()
{
}

TagContainer::~TagContainer()
{
	for (int i = 0; i < mUsedSampleTags.size(); i++)
	{
		delete mUsedSampleTags[i];
		mUsedSampleTags[i] = nullptr;
	}
	mUsedSampleTags.clear();
}

void TagContainer::paint(Graphics& g)
{
	tagFont = g.getCurrentFont().withHeight(SAMPLE_TAG_FONT_SIZE);
}

void TagContainer::resized()
{
	updateItems();
}

void TagContainer::updateItems()
{
	int padding = SAMPLE_TAG_TEXT_PADDING;
	int fontHeight = tagFont.getHeight();
	int boxHeight = fontHeight + padding;

	int currentWidth = 0.0f;
	int line = 0;
	for (int i = 0; i < mCurrentTags.size(); i++)
	{
		float fontWidth = tagFont.getStringWidthFloat(mCurrentTags[i]);
		float boxWidth = fontWidth + (padding * 2);
		if (currentWidth + boxWidth < getWidth())
		{
			TagTile* tag;
			if (mUsedSampleTags.size() > i)
			{
				tag = mUsedSampleTags[i];
			}
			else
			{
				tag = new TagTile(mCurrentTags[i]);
				addAndMakeVisible(tag);
				mUsedSampleTags.push_back(tag);
			}
			tag->setBounds(currentWidth, line * boxHeight, boxWidth, boxHeight);
			tag->setTag(mCurrentTags[i]);
			currentWidth += boxWidth + padding;
		}
		else
		{
			if (currentWidth != 0.0f)
			{
				line++;
				currentWidth = 0.0f;
				i--; //redo this next line
			}
		}
	}
	mLineCount = line + 1;
}

void TagContainer::clearTags()
{
	mCurrentTags.clear();
	updateItems();
}

StringArray TagContainer::getTags()
{
	return mCurrentTags;
}

void TagContainer::setTags(StringArray newTags)
{
	mCurrentTags = newTags;
	updateItems();
}

void TagContainer::removeTag(juce::String tag)
{
	mCurrentTags.removeString(tag, true);
	updateItems();
}

void TagContainer::addTag(juce::String tag)
{
	mCurrentTags.add(tag);
	updateItems();
}

int TagContainer::calculateAllRowsHeight()
{
	if (mUsedSampleTags.size() > 0)
	{
		return mUsedSampleTags[0]->getHeight() * mLineCount;
	}
	else
	{
		return 0;
	}
}

int TagContainer::getRowCount()
{
	return mLineCount;
}

void TagContainer::extendItems()
{
	mMaxItems += 20;
}
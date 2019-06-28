#include "TagContainer.h"
#include "SamplifyColorPallete.h"

using namespace samplify;

TagContainer::~TagContainer()
{
}

void TagContainer::paint(Graphics& g)
{
	updateItems(g);
}

void TagContainer::resized()
{
}

void TagContainer::updateItems(Graphics& g)
{
	int padding = SAMPLE_TAG_TEXT_PADDING;
	int fontHeight = g.getCurrentFont().getHeight();
	int boxHeight = fontHeight + padding;

	int currentWidth = 0.0f;
	int line = 0;
	for (int i = 0; i < mCurrentTags.size(); i++)
	{
		float fontWidth = g.getCurrentFont().getStringWidthFloat(mCurrentTags[i]);
		float boxWidth = fontWidth + (padding * 2);
		if (currentWidth + boxWidth < getWidth())
		{
			//draw the box bih
			TagTile* tag;
			if (mUsedSampleTags.size() > i)
			{
				tag = mUsedSampleTags[i];
				tag->setTag(mCurrentTags[i]);
			}
			else
			{
				tag = new TagTile(mCurrentTags[i]);
				mUsedSampleTags.push_back(tag);
			}

			tag->setBounds(currentWidth, line * boxHeight, boxWidth, boxHeight);

			currentWidth += fontWidth + padding;
		}
		else
		{
			line++;
			currentWidth = 0.0f;
			i--; //redo this next line
		}
		mLineCount = line;
		setBounds(0, 0, getWidth(), line * boxHeight);
	}
}

void TagContainer::clearItems()
{
}

StringArray TagContainer::getTags()
{
	return mCurrentTags;
}

void TagContainer::setItems(StringArray newTags)
{
	mCurrentTags = newTags;
	repaint();
}

void TagContainer::removeTag(juce::String tag)
{
	mCurrentTags.removeString(tag, true);
	repaint();
}

void TagContainer::addTag(juce::String tag)
{
	mCurrentTags.add(tag);
	repaint();
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

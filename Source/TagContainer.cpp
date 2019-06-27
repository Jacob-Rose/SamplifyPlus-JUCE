#include "TagContainer.h"

TagContainer::~TagContainer()
{
}

void TagContainer::paint(Graphics&)
{
}

void TagContainer::resized()
{
}

void TagContainer::updateItems()
{
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
	return 0;
}

int TagContainer::calculateRowCount()
{
	return 0;
}

int TagContainer::calculateColumnCount()
{
	return 0;
}

void TagContainer::extendItems()
{
	mMaxItems += 20;
}

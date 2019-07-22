#include "TagExplorer.h"
#include "SamplifyProperties.h"
#include "SamplifyLookAndFeel.h"
#include "Sample.h"

using namespace samplify;

TagExplorer::TagExplorer()
{
	mNewButtonTag.setButtonText("New Tag");
	mNewButtonTag.onClick = [this] {addNewTag(); };
	addAndMakeVisible(mNewButtonTag);
	addAndMakeVisible(mTagViewport);
	mTagViewport.addAndMakeVisible(mTagsContainer);
	mTagViewport.setViewedComponent(&mTagsContainer);
	//mTagViewport.setViewedComponent()
	mTagViewport.setScrollBarsShown(true, false, true, false);
}

TagExplorer::~TagExplorer()
{
}

void TagExplorer::resized()
{
	mNewButtonTag.setBounds(0,getHeight()-30, getWidth(), 30);
	mTagViewport.setBounds(0, 0, getWidth(), getHeight() - 30);
	mTagsContainer.setBounds(mTagViewport.getBounds());
}

void TagExplorer::paint(Graphics& g)
{

}

void TagExplorer::addNewTag()
{
	TextEditor* te = new TextEditor();
	te->setBounds(0,0,200,30);
	//change from nullptr to the maincompoent to make it overlay?
	DialogWindow::showModalDialog("New Tag Name", te,nullptr, getLookAndFeel().findColour(MAIN_BASE_COLOR_ID), true,false);
	mTagsContainer.addNewTag(te->getText());
	delete te;

}

void samplify::TagExplorer::Container::removeNewTag(juce::String tag)
{
	mNewTags.removeTag(tag);
}

void TagExplorer::Container::paint(Graphics& g)
{
	g.fillAll(Colours::bisque);
	String firstLine = "New Tags:";
	float cHeight = 0.0f;
	g.drawText(firstLine, Rectangle<float>(0.0f, cHeight, getWidth(), infoFont.getHeight()), Justification::centred);
	String secondLine = "Directory Contained:";
	cHeight += mNewTags.getHeight();
	cHeight += infoFont.getHeight();
	g.drawText(secondLine, Rectangle<float>(0.0f, cHeight, getWidth(), infoFont.getHeight()), Justification::centred);
	String thirdLine = "Outside Tags:";
	cHeight += mContainedTags.getHeight();
	cHeight += infoFont.getHeight();
	g.drawText(thirdLine, Rectangle<float>(0.0f, cHeight, getWidth(), infoFont.getHeight()), Justification::centred);
}

void TagExplorer::Container::updateTags(juce::String newSearch)
{
	StringArray allTags = SamplifyProperties::getInstance()->getSampleLibrary()->getAllTags();
	//remove all new tags that have been used now
	resetTags();
	StringArray passedTags; //in dir
	StringArray failedTags; //not in dir
	//StringArray currentSampleTags = SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples();
	Sample::List currentSamps = SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples();
	
	for (int i = 0; i < allTags.size(); i++)
	{
		if (allTags[i].contains(newSearch))
		{
			bool found = false;
			for (int j = 0; j < currentSamps.size(); j++)
			{
				StringArray sampTags = currentSamps[j].getTags();
				if (sampTags.contains(allTags[i]))
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				passedTags.add(allTags[i]);
			}
			else
			{
				failedTags.add(allTags[i]);
			}
		}
	}
	
	mContainedTags.setTags(passedTags);
	mNotContainedTags.setTags(failedTags);
	updateTagContainerBounds();
}

void TagExplorer::Container::updateTagContainerBounds()
{
	float textHeight = infoFont.getHeight();
	float cHeight = 0.0f;
	cHeight += textHeight;
	mNewTags.setBounds(0, cHeight, getWidth(), getHeight());
	mNewTags.setTopLeftPosition(0, cHeight);
	cHeight += mNewTags.getBounds().getHeight();
	cHeight += textHeight;
	mContainedTags.setBounds(0, cHeight, getWidth(), getHeight());
	mContainedTags.setTopLeftPosition(0, cHeight);
	cHeight += mContainedTags.getBounds().getHeight();
	cHeight += textHeight;
	mNotContainedTags.setBounds(0, cHeight, getWidth(), getHeight());
	mNotContainedTags.setTopLeftPosition(0, cHeight);
	cHeight += mNotContainedTags.getBounds().getHeight();
	setBounds(0, 0, getWidth(),cHeight);
	repaint();
}

void TagExplorer::Container::resetTags()
{
	StringArray allTags = SamplifyProperties::getInstance()->getSampleLibrary()->getAllTags();
	for (int i = 0; i < mNewTags.getTags().size(); i++)
	{
		if (allTags.contains(mNewTags.getTags()[i]))
		{
			mContainedTags.addTag(mNewTags.getTags()[i]);
			mNewTags.removeTag(mNewTags.getTags()[i]);
			i--;
		}
	}
	for (int i = 0; i < mContainedTags.getTags().size(); i++)
	{
		if (!allTags.contains(mContainedTags.getTags()[i]))
		{
			mContainedTags.removeTag(mContainedTags.getTags()[i]);
			i--;
		}
	}
	for (int i = 0; i < mNotContainedTags.getTags().size(); i++)
	{
		if (!allTags.contains(mNotContainedTags.getTags()[i]))
		{
			mNotContainedTags.removeTag(mNotContainedTags.getTags()[i]);
			i--;
		}
	}
}

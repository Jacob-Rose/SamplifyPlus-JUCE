/*
  ==============================================================================

    TagExplorer_V2.cpp
    Created: 30 May 2020 6:33:22pm
    Author:  jacob

  ==============================================================================
*/

#include "TagExplorer_V2.h"
#include "SamplifyLookAndFeel.h"
#include "SamplifyProperties.h"

using namespace samplify;

TagExplorer_V2::TagExplorer_V2() : mTagsContainer(false)
{
	mNewButtonTag.setButtonText("New Tag");
	mNewButtonTag.onClick = [this] {addNewTag(); };
	addAndMakeVisible(mNewButtonTag);
	addAndMakeVisible(mTagViewport);
	mTagViewport.addAndMakeVisible(mTagsContainer);
	mTagViewport.setViewedComponent(&mTagsContainer);
	//mTagViewport.setViewedComponent()
	mTagViewport.setScrollBarsShown(true, false, true, false);
	SamplifyProperties::getInstance()->getSampleLibrary()->addChangeListener(this);
}

TagExplorer_V2::~TagExplorer_V2()
{
}

void TagExplorer_V2::resized()
{
	mNewButtonTag.setBounds(0, getHeight() - 30, getWidth(), 30);
	mTagViewport.setBounds(0, 0, getWidth(), getHeight() - 30);
	mTagsContainer.setBounds(mTagViewport.getBounds());
}

void TagExplorer_V2::paint(Graphics&)
{
}

void TagExplorer_V2::addNewTag()
{
	TextEditor te = TextEditor();
	te.setBounds(0, 0, 200, 30);
	//change from nullptr to the maincompoent to make it overlay?
	DialogWindow::showModalDialog("New Tag Name", &te, nullptr, getLookAndFeel().findColour(ResizableWindow::backgroundColourId), true, false);
	SamplifyProperties::getInstance()->getSampleLibrary()->addTag(te.getText());
	mTagsContainer.setTags(SamplifyProperties::getInstance()->getSampleLibrary()->getTagsStringArray());
}

void samplify::TagExplorer_V2::updateTags(juce::String query)
{
	StringArray tags = SamplifyProperties::getInstance()->getSampleLibrary()->getTagsStringArray();
	StringArray cTags = StringArray(); //to fix loop delete issues
	for (int i = 0; i < tags.size(); i++)
	{
		if (tags[i].containsIgnoreCase(query))
		{
			cTags.add(tags[i]);
		}
	}
	mTagsContainer.setTags(cTags);
}

void TagExplorer_V2::changeListenerCallback(ChangeBroadcaster* source)
{
	updateTags("");
}

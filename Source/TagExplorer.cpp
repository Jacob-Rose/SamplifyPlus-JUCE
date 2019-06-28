#include "TagExplorer.h"
#include "SamplifyProperties.h"
#include "SamplifyColorPallete.h"
#include "SampleReference.h"

using namespace samplify;

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
	//StringArray currentSampleTags = SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples();
	for (int i = 0; i < mNewTags.getTags().size(); i++)
	{
		if (mNotContainedTags.getTags().contains(mNewTags.getTags()[i]))
		{
			mNewTags.removeTag(mNewTags.getTags()[i]);
			i--;
		}
	}
	for (int i = 0; i < allTags.size(); i++)
	{
		if (allTags[i].contains(newSearch))
		{
			//if()
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
	TextEditor* te = new TextEditor();
	te->setBounds(0,0,200,30);
	//change from nullptr to the maincompoent to make it overlay?
	DialogWindow::showModalDialog("New Tag Name", te,nullptr, SAMPLE_TILE_COLOR_BG_DEFAULT, true,false);
	mNewTags.addTag(te->getText());

}

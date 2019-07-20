#include "FilterExplorer.h"

using namespace samplify;
//==============================================================================
FilterExplorer::FilterExplorer()
{
	mSearchBar.addListener(this);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	addAndMakeVisible(mSearchBar);
	addAndMakeVisible(mTagExplorer);
}

FilterExplorer::~FilterExplorer()
{
}

void FilterExplorer::paint (Graphics& g)
{

}

void FilterExplorer::resized()
{
	mSearchBar.setBounds(0, 0, getWidth(), 30);
	mTagExplorer.setBounds(0,30,getWidth(), getHeight()-30);
}

void FilterExplorer::textEditorTextChanged(TextEditor& te)
{
	mTagExplorer.getTagContainer()->updateTags(te.getText());
}
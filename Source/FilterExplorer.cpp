/*
  ==============================================================================

    FilterExplorer.cpp
    Created: 31 May 2018 1:21:30pm
    Author:  jacob

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterExplorer.h"

//==============================================================================
FilterExplorer::FilterExplorer()
{
	mSearchBox.addListener(this);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

FilterExplorer::~FilterExplorer()
{
}

void FilterExplorer::paint (Graphics& g)
{
	/*
	float xPos = 0;
	float yPos = 0;
	for (int i = 0; i < mLatestTags.size(); i++)
	{
		float width = g.getCurrentFont().getStringWidthFloat(mLatestTags[i]);
		float height = g.getCurrentFont().getHeight();
		Rectangle<float> area = Rectangle<float>(xPos, yPos, width, height);
		g.drawText(mLatestTags[i], area, Justification::centredLeft);
		xPos += width;
		if (xPos > getBounds().getWidth())
		{
			xPos = 0;
			yPos += height;
		}
	}
	*/
}

void FilterExplorer::resized()
{
	mSearchBox.setBounds(Rectangle<int>(0, 0, getWidth(), 40));
}

void FilterExplorer::textEditorTextChanged(TextEditor&)
{
}
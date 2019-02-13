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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

FilterExplorer::~FilterExplorer()
{
}

void FilterExplorer::paint (Graphics& g)
{
    
}

void FilterExplorer::resized()
{
	mSearchBox.setBounds(Rectangle<int>(0, 0, getWidth(), 40));
}

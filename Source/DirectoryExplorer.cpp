/*
  ==============================================================================

    DirectoryExplorer.cpp
    Created: 31 May 2018 1:22:06pm
    Author:  jacob

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DirectoryExplorer.h"

//==============================================================================
DirectoryExplorer::DirectoryExplorer()
{
	mDirectoryTree.reset(new DirectoryExplorerTreeView());
	addAndMakeVisible(*mDirectoryTree);
}

DirectoryExplorer::~DirectoryExplorer()
{
}

void DirectoryExplorer::paint (Graphics& g)
{
    
}

void DirectoryExplorer::resized()
{
	mDirectoryTree->setBounds(getLocalBounds());
}

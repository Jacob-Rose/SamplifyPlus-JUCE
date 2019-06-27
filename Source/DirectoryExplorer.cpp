#include "DirectoryExplorer.h"

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

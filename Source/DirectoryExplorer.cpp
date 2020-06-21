#include "DirectoryExplorer.h"
#include "SamplifyProperties.h"
#include "Icons.h"

using namespace samplify;

DirectoryExplorer::DirectoryExplorer()
{
	setScrollBarsShown(true, true, true, true);
	addAndMakeVisible(mDirectoryTree);
	refresh();
}

DirectoryExplorer::~DirectoryExplorer()
{
	mDirectoryTree.deleteRootItem();
}


void DirectoryExplorer::paint (Graphics& g)
{
    
}

void DirectoryExplorer::resized()
{
	mDirectoryTree.setBounds(getLocalBounds());
}

void DirectoryExplorer::refresh()
{
	TreeViewItem* root = mDirectoryTree.getRootItem();
	if (root == nullptr)
	{
		root = new DirectoryExplorerTreeViewItem("All Directories");
		mDirectoryTree.setRootItem(root);
	}
	else
	{
		root->clearSubItems();
	}
	std::vector<std::shared_ptr<SampleDirectory>> dirs = SamplifyProperties::getInstance()->getSampleLibrary()->getDirectories();
	for (int i = 0; i < dirs.size(); i++)
	{
		DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(dirs[i]);
		root->addSubItem(item);
	}
	root->setSelected(true, true);
}

void DirectoryExplorer::changeListenerCallback(ChangeBroadcaster* source)
{
	refresh();
}

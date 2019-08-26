#include "DirectoryExplorer.h"
#include "SamplifyProperties.h"

using namespace samplify;

DirectoryExplorer::DirectoryExplorer()
{
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
	std::vector<File> paths = SamplifyProperties::getInstance()->getDirectoryLibrary().getDirectories();
	for (int i = 0; i < paths.size(); i++)
	{
		DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(paths[i]);
		root->addSubItem(item);
	}
	root->setSelected(true, true);
}

void DirectoryExplorer::changeListenerCallback(ChangeBroadcaster* source)
{
	if (DirectoryLibrary * dl = dynamic_cast<DirectoryLibrary*>(source))
	{
		refresh();
	}
}
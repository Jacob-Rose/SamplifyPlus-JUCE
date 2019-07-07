#include "DirectoryExplorer.h"
#include "SamplifyProperties.h"

using namespace samplify;

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

DirectoryExplorer::DirectoryExplorerTreeView::DirectoryExplorerTreeView()
{
	DirectoryExplorerTreeViewItem* root = new DirectoryExplorerTreeViewItem("All Directories");
	setRootItem(root);
	root->setSelected(true, true);
	std::vector<File> paths = SamplifyProperties::getInstance()->getDirectories();
	for (int i = 0; i < paths.size(); i++)
	{
		DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(paths[i]);
		root->addSubItem(item);
	}
}

DirectoryExplorer::DirectoryExplorerTreeView::~DirectoryExplorerTreeView()
{
	deleteRootItem();

}
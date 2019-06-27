#include "DirectoryExplorerTreeView.h"
#include "DirectoryExplorerTreeViewItem.h"
#include "SamplifyProperties.h"

//==============================================================================
DirectoryExplorerTreeView::DirectoryExplorerTreeView()
{
	DirectoryExplorerTreeViewItem* root = new DirectoryExplorerTreeViewItem();
	setRootItem(root);

	std::vector<File> paths = SamplifyProperties::getInstance()->getDirectories();
	for (int i = 0; i < paths.size(); i++)
	{
		DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(paths[i]);
		root->addSubItem(item);
	}
}

DirectoryExplorerTreeView::~DirectoryExplorerTreeView()
{
	deleteRootItem();

}
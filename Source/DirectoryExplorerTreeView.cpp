/*
  ==============================================================================

    DirectoryExplorerTreeView.cpp
    Created: 31 May 2018 1:22:15pm
    Author:  jacob

  ==============================================================================
*/

#include "DirectoryExplorerTreeView.h"
#include "DirectoryExplorerTreeViewItem.h"
#include "AppProperties.h"

//==============================================================================
DirectoryExplorerTreeView::DirectoryExplorerTreeView()
{
	DirectoryExplorerTreeViewItem* root = new DirectoryExplorerTreeViewItem();
	setRootItem(root);

	std::vector<File> paths = AppProperties::getInstance()->getDirectories();
	for (int i = 0; i < paths.size(); i++)
	{
		DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(paths[i]);
		root->addSubItem(item);
	}
}

DirectoryExplorerTreeView::~DirectoryExplorerTreeView()
{
}
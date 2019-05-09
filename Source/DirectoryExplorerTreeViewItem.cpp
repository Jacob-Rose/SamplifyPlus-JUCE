/*
  ==============================================================================

    DirectoryExplorerTreeViewItem.cpp
    Created: 31 May 2018 1:22:28pm
    Author:  jacob

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DirectoryExplorerTreeViewItem.h"
#include "AppProperties.h"
DirectoryExplorerTreeViewItem::DirectoryExplorerTreeViewItem(File file)
{
	mFile = file;
}

DirectoryExplorerTreeViewItem::DirectoryExplorerTreeViewItem()
{
	mShouldUseFile = false;
}

DirectoryExplorerTreeViewItem::~DirectoryExplorerTreeViewItem()
{
	
}

bool DirectoryExplorerTreeViewItem::mightContainSubItems()
{
	if (mShouldUseFile)
	{
		return mFile.containsSubDirectories();
	}
	else
	{
		return true;
	}
}

String DirectoryExplorerTreeViewItem::getName()
{
	if (mShouldUseFile)
	{
		return mFile.getFileName();
	}
	else
	{
		return "";
	}
}

void DirectoryExplorerTreeViewItem::paintItem(Graphics & g, int width, int height)
{
	if (isSelected())
	{
		g.fillAll(Colours::aqua);
	}
	g.setColour(Colours::grey);
	g.setFont(12);
	g.drawText(mFile.getFileName(), 0, 0, width, height, Justification::centredLeft, true);
}

void DirectoryExplorerTreeViewItem::itemOpennessChanged(bool isNowOpen)
{
	if (isNowOpen)
	{
		// if we've not already done so, we'll now add the tree's sub-items. You could
		// also choose to delete the existing ones and refresh them if that's more suitable
		// in your app.
		if (getNumSubItems() == 0)
		{
			Array<File> files;
			int childDirCount = mFile.findChildFiles(files, 1, false);
			for (int i = 0; i < childDirCount; i++)
			{
				addSubItem(new DirectoryExplorerTreeViewItem(files[i]));
			}
		}
	}
	else
	{
		// in this case, we'll leave any sub-items in the tree when the node gets closed,
		// though you could choose to delete them if that's more appropriate for
		// your application.
	}
}

void DirectoryExplorerTreeViewItem::itemSelectionChanged(bool isNowSelected)
{
	if (isNowSelected)
	{
		AppProperties::getInstance()->setSelectedDirectory(mFile);
	}
}
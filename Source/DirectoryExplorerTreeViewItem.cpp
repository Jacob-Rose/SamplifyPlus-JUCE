#include "DirectoryExplorerTreeViewItem.h"
#include "SamplifyProperties.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

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
	int subItemCount = getNumSubItems();
	for (int i = 0; i < subItemCount; i++)
	{
		removeSubItem(0, true); 
	}
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
	//check if not added yet, dont draw
	if (getOwnerView() != nullptr)
	{
		if (isSelected())
		{
			Colour c = getOwnerView()->getLookAndFeel().findColour(MAIN_ACCENT_COLOR);
			g.setColour(c);
			g.fillRoundedRectangle(0, 0, width, height, 1.0f);
			if (c.getPerceivedBrightness() > 0.5f)
			{
				g.setColour(Colours::black);
			}
			else
			{
				g.setColour(Colours::white);
			}
		}
		else
		{

			if (getOwnerView()->getLookAndFeel().findColour(MAIN_BASE_COLOR).getPerceivedBrightness() > 0.5f)
			{
				g.setColour(Colours::black);
			}
			else
			{
				g.setColour(Colours::white);
			}
		}
		g.setFont(12);
		g.drawText(mFile.getFileName(), 0, 0, width, height, Justification::centredLeft, true);
	}

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
				DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(files[i]);  
				addSubItem(item);
			}
		}
	}
}

void DirectoryExplorerTreeViewItem::itemSelectionChanged(bool isNowSelected)
{
	if (isNowSelected)
	{
		SamplifyProperties::getInstance()->setSelectedDirectory(mFile);
	}
}
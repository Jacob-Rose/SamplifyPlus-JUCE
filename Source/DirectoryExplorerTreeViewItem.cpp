#include "DirectoryExplorerTreeViewItem.h"
#include "SamplifyMainComponent.h"
#include "SamplifyProperties.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

DirectoryExplorerTreeViewItem::DirectoryExplorerTreeViewItem(File file)
{
	mFile = file;
	mShouldUseFile = true;
}

DirectoryExplorerTreeViewItem::DirectoryExplorerTreeViewItem(String string)
{
	mText = string;
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

bool DirectoryExplorerTreeViewItem::isInterestedInFileDrag(const StringArray& files)
{
	bool allDirs = true;
	for (int i = 0; i < files.size(); i++)
	{
		allDirs = File(files[i]).isDirectory() && allDirs;
	}
	return allDirs;
}

void DirectoryExplorerTreeViewItem::filesDropped(const StringArray& files, int x, int y)
{
	//todo handle adding directories
}

void samplify::DirectoryExplorerTreeViewItem::setName(String name)
{
	mFile = File(name);
}

String DirectoryExplorerTreeViewItem::getName()
{
	if (mShouldUseFile)
	{
		return mFile.getFileName();
	}
	else
	{
		return mText;
	}
}

void DirectoryExplorerTreeViewItem::paintItem(Graphics & g, int width, int height)
{
	//check if not added yet, dont draw
	if (getOwnerView() != nullptr)
	{
		if (isSelected())
		{
			Colour c = getOwnerView()->getLookAndFeel().findColour(MAIN_ACCENT_COLOR_ID);
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

			if (getOwnerView()->getLookAndFeel().findColour(MAIN_BASE_COLOR_ID).getPerceivedBrightness() > 0.5f)
			{
				g.setColour(Colours::black);
			}
			else
			{
				g.setColour(Colours::white);
			}
		}
		g.setFont(12);
		Rectangle<float> checkBoxRect = Rectangle<float>(0, 0, height, height);
		float checkBoxCornerWidth = 4.0f;
		switch (mCheckStatus)
		{
		case NotLoaded:
			g.setColour(Colours::lightpink);
			break;
		case Mixed:
		case Enabled:
			g.setColour(SamplifyMainComponent::getInstance()->getLookAndFeel().findColour(SAMPLE_TILE_BG_HOVER_COLOR_ID));
			break;
		case Disabled:
			g.setColour(Colours::white);
			break;
		}
		g.fillRoundedRectangle(checkBoxRect, checkBoxCornerWidth);
		g.setColour(Colours::black);
		g.drawRoundedRectangle(checkBoxRect, checkBoxCornerWidth, 1.0f);
		g.drawText(mFile.getFileName(), height, 0, width, height, Justification::centredLeft, true);
	}

}

void DirectoryExplorerTreeViewItem::updateCheckAndChainParent()
{
	if (getNumSubItems() > 0)
	{

	}
	((DirectoryExplorerTreeViewItem*)getParentItem())->updateCheckAndChainParent();
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

void DirectoryExplorerTreeViewItem::itemClicked(const MouseEvent& e)
{
	int itemHeight = getItemHeight();
	int itemXPos = getItemPosition(false).getX();
	int xPos = e.getMouseDownPosition().getX() - itemXPos;
	if (xPos < itemHeight)
	{
		itemCheckCycled();
	}
}

void DirectoryExplorerTreeViewItem::itemCheckCycled()
{
	switch (mCheckStatus)
	{
	case Mixed:
	case Enabled:
		setCheckStatus(Disabled);
		break;
	case Disabled:
		setCheckStatus(Enabled);
		break;
	}
}

void DirectoryExplorerTreeViewItem::itemSelectionChanged(bool isNowSelected)
{
	if (isNowSelected)
	{
		SamplifyProperties::getInstance()->getSampleLibrary().updateCurrentSamples(mFile);
		if (SamplifyMainComponent::getInstance() != nullptr)
		{
			SamplifyMainComponent::getInstance()->getFilterExplorer().getTagExplorer().getTagContainer().resetTags();
		}
	}
}

void DirectoryExplorerTreeViewItem::setCheckStatus(CheckStatus newCheckStatus)
{
	if (newCheckStatus != mCheckStatus)
	{
		if (newCheckStatus == Disabled)
		{
			for (int i = 0; i < getNumSubItems(); i++)
			{
				((DirectoryExplorerTreeViewItem*)getSubItem(i))->setCheckStatus(Disabled);
			}
		}
		else if (newCheckStatus == Enabled)
		{
			for (int i = 0; i < getNumSubItems(); i++)
			{
				((DirectoryExplorerTreeViewItem*)getSubItem(i))->setCheckStatus(Enabled);
			}
		}
		updateCheckAndChainParent();
	}
}

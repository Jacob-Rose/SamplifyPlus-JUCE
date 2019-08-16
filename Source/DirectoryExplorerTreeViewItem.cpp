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
	for (int i = 0; i < files.size(); i++)
	{
		if (!mShouldUseFile)
		{
			File file(files[i]);
			if (file.isDirectory())
			{
				SamplifyProperties::getInstance()->loadSamplesFromDirectory(File(files[i]));
			}
		}
		else
		{

		}
	}
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
		SamplifyProperties::getInstance()->getSampleLibrary().updateCurrentSamples(mFile);
		if (SamplifyMainComponent::getInstance() != nullptr)
		{
			SamplifyMainComponent::getInstance()->getFilterExplorer().getTagExplorer().getTagContainer().resetTags();
		}
	}
}
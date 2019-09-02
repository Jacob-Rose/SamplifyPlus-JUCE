#include "DirectoryExplorerTreeViewItem.h"
#include "SamplifyMainComponent.h"
#include "SamplifyProperties.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

DirectoryExplorerTreeViewItem::DirectoryExplorerTreeViewItem(File file)
{
	mFile = file;
	mShouldUseFile = true;
	if (!mFile.exists())
	{
		mCheckStatus = NotLoaded;
	}
	else
	{
		mCheckStatus = Enabled;
	}
}

DirectoryExplorerTreeViewItem::DirectoryExplorerTreeViewItem(String string)
{ //this is only used for the root item
	mText = string;
	mShouldUseFile = false;
	mCheckStatus = Enabled;
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

void DirectoryExplorerTreeViewItem::setName(String name)
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
			g.setColour(c.brighter(0.5));
			g.fillRoundedRectangle(0, 0, width, height, 4.0f);
			g.setColour(c);
			g.drawRoundedRectangle(0, 0, width, height, 4.0f, 1.0f);
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
		float padding = 2.0f;
		Rectangle<float> checkBoxRect = Rectangle<float>(padding, padding, height - (padding*2), height-(padding*2));
		float checkBoxCornerWidth = 4.0f;
		switch (mCheckStatus)
		{
		case NotLoaded:
			g.setColour(Colours::lightpink);
			break;
		case Mixed:
			g.setColour(Colours::grey);
			break;
		case Enabled:
			g.setColour(SamplifyMainComponent::getInstance()->getLookAndFeel().findColour(SAMPLE_TILE_FG_DEFAULT_COLOR_ID));
			break;
		case Disabled:
			g.setColour(Colours::white);
			break;
		}
		g.fillRoundedRectangle(checkBoxRect, checkBoxCornerWidth);
		if (mCheckStatus == NotLoaded)
		{
			File crossFile = File::getCurrentWorkingDirectory().getChildFile("../../Icons/cross.svg");
			static std::unique_ptr<Drawable> cross = Drawable::createFromSVGFile(crossFile);
			cross.get()->replaceColour(Colours::black, Colours::white);
			cross.get()->drawWithin(g, checkBoxRect, RectanglePlacement::centred, 1.0f);
		}
		else if (mCheckStatus == Enabled)
		{
			File crossFile = File::getCurrentWorkingDirectory().getChildFile("../../Icons/check.svg");
			static std::unique_ptr<Drawable> check = Drawable::createFromSVGFile(crossFile);
			check.get()->replaceColour(Colours::black, Colours::white);
			check.get()->drawWithin(g, checkBoxRect, RectanglePlacement::centred, 1.0f);
		}
		else if (mCheckStatus == Mixed)
		{
			g.setColour(Colours::white);
			g.fillRoundedRectangle(checkBoxRect.getProportion(Rectangle<float>(0.25f, 0.25f, 0.5f, 0.5f)), 2.0f);
		}
		else //disabled
		{

		}
		g.setColour(Colours::black);
		g.drawRoundedRectangle(checkBoxRect, checkBoxCornerWidth, 1.0f);
		juce::String text;
		if (mShouldUseFile)
		{
			text = mFile.getFileName();
		}
		else
		{
			text = mText;
		}
		g.drawText(text, height, 0, width, height, Justification::centredLeft, true);
	}

}

void DirectoryExplorerTreeViewItem::updateChildrenItems(CheckStatus status)
{
	for (int i = 0; i < getNumSubItems(); i++)
	{
		static_cast<DirectoryExplorerTreeViewItem*>(getSubItem(i))->setCheckStatus(status);
		static_cast<DirectoryExplorerTreeViewItem*>(getSubItem(i))->updateChildrenItems(status);
	}
}

void DirectoryExplorerTreeViewItem::updateParentItems()
{
	if (getNumSubItems() > 0)
	{
		bool foundChecked = false;
		bool foundUnchecked = false;
		for (int i = 0; i < getNumSubItems(); i++)
		{
			if (static_cast<DirectoryExplorerTreeViewItem*>(getSubItem(i))->getCheckStatus() == Enabled)
			{
				foundChecked = true;
			}
			else if (static_cast<DirectoryExplorerTreeViewItem*>(getSubItem(i))->getCheckStatus() == Disabled)
			{
				foundUnchecked = true;
			}
			else if (static_cast<DirectoryExplorerTreeViewItem*>(getSubItem(i))->getCheckStatus() == Mixed)
			{
				foundChecked = true;
				foundUnchecked = true;
			}
			else
			{
				//not loaded, ignore
			}
		}
		if (foundChecked && foundUnchecked)
		{
			mCheckStatus = Mixed;
		}
		else if (foundChecked)
		{
			mCheckStatus = Enabled;
		}
		else
		{
			mCheckStatus = Disabled;
		}
		repaintItem();
	}
	if (DirectoryExplorerTreeViewItem * p = dynamic_cast<DirectoryExplorerTreeViewItem*>(getParentItem()))
	{
		p->updateParentItems();
	}
}

void DirectoryExplorerTreeViewItem::itemOpennessChanged(bool isNowOpen)
{
	if (isNowOpen)
	{
		if (getNumSubItems() == 0)
		{
			Array<File> files;
			int childDirCount = mFile.findChildFiles(files, 1, false);
			for (int i = 0; i < childDirCount; i++)
			{
				DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(files[i]);  
				addSubItem(item);
				if (mCheckStatus == Enabled || mCheckStatus == Disabled)
				{
					item->setCheckStatus(mCheckStatus);
				}
				else
				{
					item->setCheckStatus(Enabled);
				}
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

void DirectoryExplorerTreeViewItem::setCheckStatus(CheckStatus newCheckStatus)
{
	if (newCheckStatus != mCheckStatus && mCheckStatus != NotLoaded)
	{
		if (newCheckStatus == Disabled || newCheckStatus == Enabled)
		{
			mCheckStatus = newCheckStatus;
			updateChildrenItems(newCheckStatus);
			if (DirectoryExplorerTreeViewItem * p = dynamic_cast<DirectoryExplorerTreeViewItem*>(getParentItem()))
			{
				p->updateParentItems();
			}
		}
		repaintItem();
	}
}

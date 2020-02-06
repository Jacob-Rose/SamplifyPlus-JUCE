#include "DirectoryExplorerTreeViewItem.h"
#include "SamplifyMainComponent.h"
#include "SamplifyProperties.h"
#include "SamplifyLookAndFeel.h"


using namespace samplify;

DirectoryExplorerTreeViewItem::DirectoryExplorerTreeViewItem(std::shared_ptr<SampleDirectory> dir)
{
	mSampleDirectory = dir;
	mSampleDirectory->addChangeListener(this);
	mShouldUseFile = true;
}

DirectoryExplorerTreeViewItem::DirectoryExplorerTreeViewItem(String string)
{ //this is only used for the root item
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
		return mSampleDirectory->getFile().containsSubDirectories();
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

void samplify::DirectoryExplorerTreeViewItem::changeListenerCallback(ChangeBroadcaster* source)
{
	repaintItem();
}

String DirectoryExplorerTreeViewItem::getName()
{
	if (mShouldUseFile)
	{
		return mSampleDirectory->getFile().getFileName();
	}
	else
	{
		return mText;
	}
}

void DirectoryExplorerTreeViewItem::paintItem(Graphics & g, int width, int height)
{
	//check if not added yet, dont draw
	if (getOwnerView() != nullptr && mShouldUseFile)
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
		switch (mSampleDirectory->getCheckStatus())
		{
		case CheckStatus::NotLoaded:
			g.setColour(Colours::lightpink);
			break;
		case CheckStatus::Mixed:
			g.setColour(Colours::grey);
			break;
		case CheckStatus::Enabled:
			g.setColour(SamplifyMainComponent::getInstance()->getLookAndFeel().findColour(SAMPLE_TILE_FG_DEFAULT_COLOR_ID));
			break;
		case CheckStatus::Disabled:
			g.setColour(Colours::white);
			break;
		}
		g.fillRoundedRectangle(checkBoxRect, checkBoxCornerWidth);
		if (mSampleDirectory->getCheckStatus() == CheckStatus::NotLoaded)
		{
			File crossFile = File::getCurrentWorkingDirectory().getChildFile("../../Icons/cross.svg");
			static std::unique_ptr<Drawable> cross = Drawable::createFromSVGFile(crossFile);
			cross.get()->replaceColour(Colours::black, Colours::white);
			cross.get()->drawWithin(g, checkBoxRect, RectanglePlacement::centred, 1.0f);
		}
		else if (mSampleDirectory->getCheckStatus() == CheckStatus::Enabled)
		{
			File crossFile = File::getCurrentWorkingDirectory().getChildFile("../../Icons/check.svg");
			static std::unique_ptr<Drawable> check = Drawable::createFromSVGFile(crossFile);
			check.get()->replaceColour(Colours::black, Colours::white);
			check.get()->drawWithin(g, checkBoxRect, RectanglePlacement::centred, 1.0f);
		}
		else if (mSampleDirectory->getCheckStatus() == CheckStatus::Mixed)
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
			text = mSampleDirectory->getFile().getFileName();
		}
		else
		{
			text = mText;
		}
		g.drawText(text, height, 0, width, height, Justification::centredLeft, true);
	}

}

void DirectoryExplorerTreeViewItem::itemOpennessChanged(bool isNowOpen)
{
	if (isNowOpen)
	{
		if (getNumSubItems() == 0)
		{
			Array<File> files;
			int childDirCount = mSampleDirectory->getChildDirectoryCount();
			for (int i = 0; i < childDirCount; i++)
			{
				DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(mSampleDirectory->getChildDirectory(i));  
				addSubItem(item);
			}
		}
	}
}

void DirectoryExplorerTreeViewItem::itemClicked(const MouseEvent& e)
{
	if (getParentItem() != nullptr)
	{
		if (e.mods.isLeftButtonDown())
		{
			int itemHeight = getItemHeight();
			int itemXPos = getItemPosition(false).getX();
			int xPos = e.getMouseDownPosition().getX() - itemXPos;
			if (xPos < itemHeight)
			{
				itemCheckCycled();
			}
		}
		else if (e.mods.isRightButtonDown())
		{
			PopupMenu dirOptions;
			dirOptions.addItem(1, "Select Exclusively");
			int selection = dirOptions.show();
			if (selection == 1)
			{
				if (DirectoryExplorerTreeViewItem* dirTVI = dynamic_cast<DirectoryExplorerTreeViewItem*>(getParentItem()))
				{
					dirTVI->mSampleDirectory->setCheckStatus(CheckStatus::Disabled);
				}
				mSampleDirectory->setCheckStatus(CheckStatus::Enabled);
			}
		}
	}
}

void samplify::DirectoryExplorerTreeViewItem::refreshChildrenPaint()
{
	repaintItem();
	for (int i = 0; i < getNumSubItems(); i++)
	{
		((DirectoryExplorerTreeViewItem*)getSubItem(i))->refreshChildrenPaint();
	}
}

void DirectoryExplorerTreeViewItem::itemCheckCycled()
{
	mSampleDirectory->cycleCurrentCheck();
}

#include "DirectoryExplorer.h"
#include "SamplifyProperties.h"

using namespace samplify;

DirectoryExplorer::DirectoryExplorer()
{
	initializeDrawableFiles(); //first thing
	
	addAndMakeVisible(mDirectoryTree);

	refresh();
}

DirectoryExplorer::~DirectoryExplorer()
{
	mDirectoryTree.deleteRootItem();
	cleanupDrawableFiles(); //last thing
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
	std::vector<std::shared_ptr<SampleDirectory>> dirs = SamplifyProperties::getInstance()->getSampleLibrary()->getDirectories();
	for (int i = 0; i < dirs.size(); i++)
	{
		DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(dirs[i]);
		root->addSubItem(item);
	}
	root->setSelected(true, true);
}

void DirectoryExplorer::changeListenerCallback(ChangeBroadcaster* source)
{
	refresh();
}

void samplify::DirectoryExplorer::initializeDrawableFiles()
{
	if (DirectoryExplorerTreeViewItem::checkDrawable == nullptr)
	{
		File checkFile = File::getCurrentWorkingDirectory().getChildFile("../../Icons/check.svg");
		DirectoryExplorerTreeViewItem::checkDrawable = Drawable::createFromSVGFile(checkFile); //static to reuse asset
		DirectoryExplorerTreeViewItem::checkDrawable.get()->replaceColour(Colours::black, Colours::white);
	}
	if (DirectoryExplorerTreeViewItem::crossDrawable == nullptr)
	{
		File crossFile = File::getCurrentWorkingDirectory().getChildFile("../../Icons/cross.svg");
		DirectoryExplorerTreeViewItem::crossDrawable = Drawable::createFromSVGFile(crossFile); //static to reuse asset
		DirectoryExplorerTreeViewItem::crossDrawable.get()->replaceColour(Colours::black, Colours::white);
	}
}

void samplify::DirectoryExplorer::cleanupDrawableFiles()
{
	if (DirectoryExplorerTreeViewItem::checkDrawable != nullptr)
	{
		DirectoryExplorerTreeViewItem::checkDrawable.reset(nullptr);
	}
	if (DirectoryExplorerTreeViewItem::crossDrawable != nullptr)
	{
		DirectoryExplorerTreeViewItem::crossDrawable.reset(nullptr);
	}
}

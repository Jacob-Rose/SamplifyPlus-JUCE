#include "DirectoryExplorer.h"
#include "SamplifyProperties.h"
#include "Icons.h"

using namespace samplify;

DirectoryExplorer::DirectoryExplorer()
{
	initializeDrawableFiles(); //first thing
	setScrollBarsShown(true, true, true, true);
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

void DirectoryExplorer::initializeDrawableFiles()
{
	if (DirectoryExplorerTreeViewItem::checkDrawable == nullptr)
	{
		std::unique_ptr<XmlElement> svg = XmlDocument::parse(Icons::check_svg);
		DirectoryExplorerTreeViewItem::checkDrawable = Drawable::createFromSVG(*svg); //static to reuse asset
		DirectoryExplorerTreeViewItem::checkDrawable.get()->replaceColour(Colours::black, Colours::white);
		//todo make color from background and update on change
	}
	if (DirectoryExplorerTreeViewItem::crossDrawable == nullptr)
	{
		std::unique_ptr<XmlElement> svg = XmlDocument::parse(Icons::cross_svg);
		DirectoryExplorerTreeViewItem::crossDrawable = Drawable::createFromSVG(*svg); //static to reuse asset
		DirectoryExplorerTreeViewItem::crossDrawable.get()->replaceColour(Colours::black, Colours::white);
		//todo make color from background and update on change
	}
}

void DirectoryExplorer::cleanupDrawableFiles()
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

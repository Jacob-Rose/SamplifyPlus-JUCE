#include "DirectoryExplorer.h"
#include "SamplifyProperties.h"

using namespace samplify;

DirectoryExplorer::DirectoryExplorer(std::shared_ptr<SampleDirectoryManager> manager)
{
	mSampleDirectory = manager;
	addAndMakeVisible(mDirectoryTree);
	manager->addChangeListener(this);
	refresh();
}

DirectoryExplorer::~DirectoryExplorer()
{
	mDirectoryTree.deleteRootItem();
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
	for (int i = 0; i < mSampleDirectory->getCount(); i++)
	{
		DirectoryExplorerTreeViewItem* item = new DirectoryExplorerTreeViewItem(mSampleDirectory->getSampleDirectory(i));
		root->addSubItem(item);
	}
	root->setSelected(true, true);
}

void DirectoryExplorer::changeListenerCallback(ChangeBroadcaster* source)
{
	if (DirectoryLibrary * dl = dynamic_cast<DirectoryLibrary*>(source))
	{
		refresh();
	}
	else if (SampleDirectoryManager* sdm = dynamic_cast<SampleDirectoryManager*>(source))
	{
		((DirectoryExplorerTreeViewItem*)mDirectoryTree.getRootItem())->refreshChildrenPaint();
	}
}
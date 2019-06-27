/*
  ==============================================================================

    DirectoryExplorerTreeViewItem.h
    Created: 31 May 2018 1:22:28pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef DIRECTORYEXPLORERTREEVIEWITEM_H
#define DIRECTORYEXPLORERTREEVIEWITEM_H

#include "JuceHeader.h"

//==============================================================================
class DirectoryExplorerTreeViewItem    : public TreeViewItem
{
public:
	DirectoryExplorerTreeViewItem(File file);
	DirectoryExplorerTreeViewItem();
	~DirectoryExplorerTreeViewItem();

	bool mightContainSubItems() override;

	String getName();

	void paintItem(Graphics& g, int width, int height) override;

	void itemOpennessChanged(bool isNowOpen) override;

	void itemSelectionChanged(bool isNowSelected) override;

private:
	File mFile;
	bool mShouldUseFile = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectoryExplorerTreeViewItem)
};
#endif
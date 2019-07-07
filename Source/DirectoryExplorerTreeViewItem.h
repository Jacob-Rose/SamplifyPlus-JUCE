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

namespace samplify
{
	class DirectoryExplorerTreeViewItem : public TreeViewItem, public FileDragAndDropTarget
	{
	public:
		DirectoryExplorerTreeViewItem(File file);
		DirectoryExplorerTreeViewItem(String string);
		~DirectoryExplorerTreeViewItem();

		bool mightContainSubItems() override;
		bool isInterestedInFileDrag(const StringArray& files) override;
		void filesDropped(const StringArray& files, int x, int y) override;

		//todo allow external drag drop of files/folders into the directory
		//if the samples are in the root directory, just move them
		//else then add samples to sample library

		void setName(String name);
		String getName();

		void paintItem(Graphics& g, int width, int height) override;

		void itemOpennessChanged(bool isNowOpen) override;

		void itemSelectionChanged(bool isNowSelected) override;

	private:
		File mFile;
		String mText;
		bool mShouldUseFile = true;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DirectoryExplorerTreeViewItem)
	};
}
#endif
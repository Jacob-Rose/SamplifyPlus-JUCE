/*
  ==============================================================================

    DirectoryExplorerTreeView.h
    Created: 31 May 2018 1:22:15pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef DIRECTORYEXPLORERTREEVIEW_H
#define DIRECTORYEXPLORERTREEVIEW_H

#include "JuceHeader.h"

//==============================================================================
class DirectoryExplorerTreeView    : public TreeView
{
public:
    DirectoryExplorerTreeView();
    ~DirectoryExplorerTreeView();
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectoryExplorerTreeView)
};
#endif

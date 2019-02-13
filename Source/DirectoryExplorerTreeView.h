/*
  ==============================================================================

    DirectoryExplorerTreeView.h
    Created: 31 May 2018 1:22:15pm
    Author:  jacob

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DirectoryExplorerTreeView    : public TreeView
{
public:
    DirectoryExplorerTreeView();
    ~DirectoryExplorerTreeView();
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectoryExplorerTreeView)
};

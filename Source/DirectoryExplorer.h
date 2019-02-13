/*
  ==============================================================================

    DirectoryExplorer.h
    Created: 31 May 2018 1:22:06pm
    Author:  jacob

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DirectoryExplorerTreeView.h"

class DirectoryExplorer    : public Component
{
public:
    DirectoryExplorer();
    ~DirectoryExplorer();

    void paint (Graphics&) override;
    void resized() override;

private:
	std::unique_ptr<DirectoryExplorerTreeView> mDirectoryTree = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectoryExplorer)
};

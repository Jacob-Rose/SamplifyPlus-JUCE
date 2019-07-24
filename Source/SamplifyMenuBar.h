/*
  ==============================================================================

    SamplifyMenuBar.h
    Created: 6 Jul 2019 11:36:41am
    Author:  Jake Rose

	No idea how menus worked, source for help
	https://github.com/vinniefalco/SimpleDJ/blob/master/Extern/JUCE/extras/JuceDemo/Source/MainDemoWindow.cpp

  ==============================================================================
*/

#ifndef SAMPLIFYMENUBAR_H
#define SAMPLIFYMENUBAR_H

#include "JuceHeader.h"

namespace samplify
{
	class SamplifyMainMenu : public Component,
							 public MenuBarModel
	{
		enum CommandIDs
		{
			saveSampleInfo = 0x1000,
			deleteSampleInfo,
			addDirectory,
			removeDirectory = 0x3000, //this is weird
			removeAndResetDirectory = 0x1003,
			openHelpPDF,
			volumeControl
		};
		//const CommandID ids[] = { saveSampleInfo, deleteSampleInfo, addDirectory, removeDirectory, openHelpPDF, volumeControl };
	public:
		SamplifyMainMenu();
		//Menu Bar Model Functions
		StringArray getMenuBarNames() override;
		PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
		void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
		void menuBarItemsChanged(MenuBarModel* menuBarModel);
		
	private:

	};
}
#endif

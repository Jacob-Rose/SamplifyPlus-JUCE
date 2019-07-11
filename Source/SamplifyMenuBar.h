/*
  ==============================================================================

    SamplifyMenuBar.h
    Created: 6 Jul 2019 11:36:41am
    Author:  Jake Rose

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
	public:
		enum CommandIDs
		{
			saveSampleInfo = 0x2000,
			deleteSampleInfo,
			addDirectory,
			removeDirectory,
			removeAndResetDirectory,
			openHelpPDF,
			volumeControl
		};

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

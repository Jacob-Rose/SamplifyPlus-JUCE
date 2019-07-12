#include "SamplifyMenuBar.h"

using namespace samplify;

SamplifyMainMenu::SamplifyMainMenu() {}

StringArray samplify::SamplifyMainMenu::getMenuBarNames()
{
	const char* const names[] = { "Files", "Directories", "Help", nullptr };

	return StringArray(names);
}

void samplify::SamplifyMainMenu::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	switch (menuItemID)
	{
	case saveSampleInfo:
		break;
	case deleteSampleInfo:
		break;
	case addDirectory:
		break;
	case removeDirectory:
		break;
	case removeAndResetDirectory:
		break;
	case openHelpPDF:
		break;
	}
}

void samplify::SamplifyMainMenu::menuBarItemsChanged(MenuBarModel* menuBarModel) {}

PopupMenu SamplifyMainMenu::getMenuForIndex(int menuIndex, const String& menuName)
{
	PopupMenu menu;
	if (menuIndex == 0) //Files
	{
		menu.addItem(saveSampleInfo, "Save Sample Info");
		menu.addItem(deleteSampleInfo, "Delete .samp files (dialog select directory)");
	}
	if (menuIndex == 1) //dir
	{
		menu.addItem(addDirectory, "Add Directory");
		menu.addItem(removeDirectory, "Remove Directory");
		menu.addItem(removeAndResetDirectory, "Remove Directory and Delete .samp files");
	}
	if (menuIndex == 2)
	{
		menu.addItem(openHelpPDF, "View Tutorial PDF");
	}
	menu.addSeparator();
	return menu;
}

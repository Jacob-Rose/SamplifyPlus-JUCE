#include "SamplifyMenuBar.h"
#include "SamplifyProperties.h"

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
		SamplifyProperties::getInstance()->browseForDirectoryAndAdd();
		break;
	case removeDirectory:
		break;
	case removeAndResetDirectory:
		break;
	case openHelpPDF:
		break;
	}
	std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectories();
	for (int i = 0; i < dirs.size(); i++)
	{
		if (menuItemID == removeDirectory + i)
		{
			SamplifyProperties::getInstance()->removeDirectory(dirs[i]);
		}
	}
}

void samplify::SamplifyMainMenu::menuBarItemsChanged(MenuBarModel* menuBarModel) {}

PopupMenu SamplifyMainMenu::getMenuForIndex(int menuIndex, const String& menuName)
{
	PopupMenu menu;
	if (menuIndex == 0) //Files
	{
		menu.addItem(saveSampleInfo, "Save Sample Info", false, false);
		menu.addItem(deleteSampleInfo, "Delete .samp files (dialog select directory)", false, false);
	}
	if (menuIndex == 1) //dir
	{
		menu.addItem(addDirectory, "Add Directory", true, false);
		//menu.addItem(removeDirectory, "Remove Directory");
		PopupMenu removeMenu;
		std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectories();
		for (int i = 0; i < dirs.size(); i++)
		{
			removeMenu.addItem(removeDirectory + i, dirs[i].getFullPathName(), true, false);
		}
		menu.addSubMenu("Remove Dirs", removeMenu, true);
		//menu.addItem(removeAndResetDirectory, "Remove Directory and Delete .samp files");
	}
	if (menuIndex == 2)
	{
		menu.addItem(openHelpPDF, "View Tutorial PDF");
	}
	menu.addSeparator();
	return menu;
}

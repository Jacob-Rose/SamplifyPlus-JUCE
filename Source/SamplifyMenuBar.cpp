#include "SamplifyMenuBar.h"

using namespace samplify;

SamplifyMainMenu::SamplifyMainMenu(ApplicationCommandManager* m)
{
	manager = m;
}

StringArray samplify::SamplifyMainMenu::getMenuBarNames()
{
	const char* const names[] = { "Files", "Directories", "Options", "Help", nullptr };

	return StringArray(names);
}

void samplify::SamplifyMainMenu::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
}

void samplify::SamplifyMainMenu::menuBarItemsChanged(MenuBarModel* menuBarModel)
{
}

void samplify::SamplifyMainMenu::menuCommandInvoked(MenuBarModel* menuBarModel, const ApplicationCommandTarget::InvocationInfo& info)
{
}

ApplicationCommandTarget* SamplifyMainMenu::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void SamplifyMainMenu::getAllCommands(Array<CommandID>& commands)
{
	const CommandID ids[] = { saveSampleInfo, 
		deleteSampleInfo, addDirectory, removeDirectory, openHelpPDF, volumeControl };
	commands.addArray(ids, numElementsInArray(ids));
}

void SamplifyMainMenu::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	switch (commandID)
	{
	case saveSampleInfo:
		result.setInfo("Save Sample Info", "Save the sample properties files", "Files", 0);
		break;
	case deleteSampleInfo:
		result.setInfo("Delete Sample Info", "Delete all the sample properties files", "Files", 0);
		break;
	case addDirectory:
		result.setInfo("Add Directory", "Add a directory to analyse", "Directories", 0);
		break;
	case removeDirectory:
		result.setInfo("Remove Directory", "Remove A already added directory", "Directories", 0);
		break;
	case openHelpPDF:
		result.setInfo("Save Sample Info", "Save the sample properties files", "Help", 0);
		break;
	case volumeControl:
		result.setInfo("Save Sample Info", "Save the sample properties files", "Options", 0);
		break;
	}
}

bool SamplifyMainMenu::perform(const InvocationInfo& info)
{
	return false;
}

PopupMenu SamplifyMainMenu::getMenuForIndex(int menuIndex, const String& menuName)
{
	PopupMenu menu;
	if (menuIndex == 0) //Files
	{
		menu.addCommandItem(manager, saveSampleInfo);
		menu.addCommandItem(manager, deleteSampleInfo);
	}
	if (menuIndex == 1) //dir
	{
		menu.addCommandItem(manager, addDirectory);
		menu.addCommandItem(manager, removeDirectory);
	}
	if (menuIndex == 2)
	{
		menu.addCommandItem(manager, volumeControl);
	}
	if (menuIndex == 3)
	{
		menu.addCommandItem(manager, openHelpPDF);
	}
	return menu;
}

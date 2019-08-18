#include "SamplifyMenuBar.h"
#include "SamplifyProperties.h"

using namespace samplify;

SamplifyMainMenu::SamplifyMainMenu() {}

StringArray SamplifyMainMenu::getMenuBarNames()
{
	const char* const names[] = { "Directories", "Volume", nullptr };
	return StringArray(names);
}

void SamplifyMainMenu::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	if (menuItemID == addDirectory)
	{
		SamplifyProperties::getInstance()->browseForDirectoryAndAdd();
	}
	else if (menuItemID == setVolume)
	{
		Slider slider;
		slider.setRange(0, 2);
		slider.setBounds(0, 0, 200, 40);
		DialogWindow::showModalDialog("Set Gain", &slider, nullptr, Colours::slategrey, true, false);
		SamplifyProperties::getInstance()->getAudioPlayer()->setVolumeMultiply(slider.getValue());
	}
	else if (menuItemID == removeSampFiles)
	{
		File file = SamplifyProperties::browseForDirectory();
		if (file.exists())
		{
			bool notLoaded = true;
			std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectoryLibrary().getDirectories();
			for (int i = 0; i < dirs.size(); i++)
			{
				if (file == dirs[i] || file.isAChildOf(dirs[i]))
				{
					notLoaded = false;
					break;
				}
			}
			if (notLoaded)
			{
				SamplifyProperties::getInstance()->getDirectoryLibrary().removeDirectory(file);
			}
			DeleteSamplifyFilesThread deleteThread(file);
			deleteThread.runThread();
		}
	}
	else
	{
		std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectoryLibrary().getDirectories();
		for (int i = 0; i < dirs.size(); i++)
		{
			if (menuItemID == removeDirectory + i)
			{
				SamplifyProperties::getInstance()->getDirectoryLibrary().removeDirectory(dirs[i]);
			}
		}
	}
}

PopupMenu SamplifyMainMenu::getMenuForIndex(int menuIndex, const String& menuName)
{
	PopupMenu menu;
	if (menuIndex == 0) //dir
	{
		menu.addItem(addDirectory, "Add Directory", true, false);
		menu.addItem(removeSampFiles, "Select Directory and Remove Samples");
		//menu.addItem(removeDirectory, "Remove Directory");
		PopupMenu removeMenu;
		std::vector<File> dirs = SamplifyProperties::getInstance()->getDirectoryLibrary().getDirectories();
		for (int i = 0; i < dirs.size(); i++)
		{
			removeMenu.addItem(removeDirectory + i, dirs[i].getFullPathName(), true, false);
		}
		menu.addSubMenu("Remove Dirs", removeMenu, true);
		//menu.addItem(removeAndResetDirectory, "Remove Directory and Delete .samp files");
	}
	else if (menuIndex == 1)
	{
		menu.addItem(setVolume, "Set Gain", true, false);
	}
	menu.addSeparator();
	return menu;
}

void SamplifyMainMenu::DeleteSamplifyFilesThread::run()
{
	DirectoryIterator iterator(mDirectory, true, "*.samplify");
	while (iterator.next())
	{
		iterator.getFile().deleteFile();
	}
}

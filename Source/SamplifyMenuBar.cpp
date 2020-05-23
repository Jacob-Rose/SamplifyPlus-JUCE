#include "SamplifyMenuBar.h"
#include "SamplifyProperties.h"

using namespace samplify;

SamplifyMainMenu::SamplifyMainMenu() {}

StringArray SamplifyMainMenu::getMenuBarNames()
{
	StringArray names = { "File", "View", "Info", nullptr };
	return names;
}

void SamplifyMainMenu::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	if (menuItemID == addDirectory)
	{
		File dir = SamplifyProperties::browseForDirectory();
		if (dir.exists())
		{
			SamplifyProperties::getInstance()->getSampleLibrary()->addDirectory(dir);
		}
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
		//todo clean this and update app when thread finishes
		/*
		File file = SamplifyProperties::browseForDirectory();
		if (file.exists())
		{
			bool notLoaded = true;
			std::vector<std::shared_ptr<SampleDirectory>> dirs = SamplifyProperties::getInstance()->getSampleLibrary()->getDirectories();
			for (int i = 0; i < dirs.size(); i++)
			{
				if (file == dirs[i]->getFile() || file.isAChildOf(dirs[i]->getFile()))
				{
					notLoaded = false;
					break;
				}
			}
			if (notLoaded)
			{
				SamplifyProperties::getInstance()->getSampleLibrary()->removeDirectory(file);
			}
			DeleteSamplifyFilesThread deleteThread(file);
			deleteThread.runThread();
		}
		*/
	}
}

PopupMenu SamplifyMainMenu::getMenuForIndex(int menuIndex, const String& menuName)
{
	PopupMenu menu;
	if (menuIndex == 0) //File
	{
		menu.addItem(addDirectory, "Add Directory", true, false);
		//menu.addItem(removeSampFiles, "Select Directory and Remove Samples");
		//menu.addItem(removeDirectory, "Remove Directory");
		PopupMenu removeMenu;
		std::vector<std::shared_ptr<SampleDirectory>> dirs = SamplifyProperties::getInstance()->getSampleLibrary()->getDirectories();
		for (int i = 0; i < dirs.size(); i++)
		{
			removeMenu.addItem(removeDirectory + i, dirs[i]->getFile().getFullPathName(), true, false);
		}
		menu.addSubMenu("Remove Directory:", removeMenu, true);
		//menu.addItem(removeAndResetDirectory, "Remove Directory and Delete .samp files");
	}
	else if (menuIndex == 1) //View
	{
		//todo check if enabled or disabled
		//menu.addItem(togglePlayerWindow, "");
	}
	else if (menuIndex == 2) //Info
	{
		menu.addItem(visitWebsite, "Visit Website", true, false);
	}
	//menu.addSeparator();
	return menu;
}
/*
void SamplifyMainMenu::DeleteSamplifyFilesThread::run()
{
	DirectoryIterator iterator(mDirectory, true, "*.samplify");
	while (iterator.next())
	{
		iterator.getFile().deleteFile();
	}
}
*/
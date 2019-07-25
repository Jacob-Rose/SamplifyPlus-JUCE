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
	class SamplifyMainMenu : public Component, public MenuBarModel
	{
	public:
		enum CommandIDs
		{
			addDirectory = 0x299,
			removeSampFiles,
			setVolume,
			removeDirectory = 0x3000, //outlier number required for removeDirectory sub-menu
		};

		class DeleteSamplifyFilesThread : public ThreadWithProgressWindow
		{
		public:
			DeleteSamplifyFilesThread(File dir) : ThreadWithProgressWindow("deleting .samplify", true, false),
				mDirectory(dir) {}
			void run() override;
		private:
			File mDirectory;
		};
		SamplifyMainMenu();
		//Menu Bar Model Functions
		StringArray getMenuBarNames() override;
		PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
		void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
	};
}
#endif

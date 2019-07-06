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
		StringArray getMenuBarNames();
		PopupMenu getMenuForIndex(int menuIndex);
	private:

	};
}
#endif

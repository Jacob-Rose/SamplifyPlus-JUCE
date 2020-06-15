/*
  ==============================================================================

    InfoWindow.cpp
    Created: 14 Jun 2020 8:48:43pm
    Author:  jacob

  ==============================================================================
*/

#include "InfoWindow.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;

InfoWindow::InfoWindow() : DocumentWindow("Information", AppValues::getInstance().MAIN_BACKGROUND_COLOR, allButtons, true)
{
    setSize(200, 400);
}

void InfoWindow::paint(Graphics& g)
{
    g.fillAll(getBackgroundColour());
}

void InfoWindow::closeButtonPressed()
{
    exitModalState(0);
}

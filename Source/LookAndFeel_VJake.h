/*
  ==============================================================================

    LookAndFeel_V5.h
    Created: 11 Jun 2020 2:57:37pm
    Author:  jacob

  ==============================================================================
*/

#ifndef LOOKANDFEELVJAKE_H
#define LOOKANDFEELVJAKE_H
#include "JuceHeader.h"

//way to override menu and anything else that comes up
class LookAndFeel_VJake : public LookAndFeel_V4
{
    //modified to override the menu the way I want
    void drawMenuBarBackground(Graphics&, int width, int height, bool isMouseOverBar, MenuBarComponent&) override;
};
#endif

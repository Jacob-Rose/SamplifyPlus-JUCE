/*
  ==============================================================================

    LookAndFeel_V5.cpp
    Created: 11 Jun 2020 2:57:37pm
    Author:  jacob

  ==============================================================================
*/

#include "LookAndFeel_VJake.h"

void LookAndFeel_VJake::drawMenuBarBackground(Graphics& g, int width, int height, bool isMouseOverBar, MenuBarComponent& menuBar )
{
    //copy and pasted with no alpha for menu and small tweaks
    auto colour = menuBar.findColour(TextButton::buttonColourId);

    Rectangle<int> r(width, height);
    g.setColour(colour);
    //g.setColour(colour.contrasting(0.15f));
    g.fillRect(r.removeFromTop(1));
    g.fillRect(r.removeFromBottom(1));

    //g.setGradientFill(ColourGradient::vertical(colour, 0, colour.darker(0.2f), (float)height));
    g.fillRect(r);
}

void LookAndFeel_VJake::drawTreeviewPlusMinusBox(Graphics& g, const Rectangle<float>& area, Colour backgroundColour, bool isOpen, bool isMouseOver)
{
    Path p;
    p.addTriangle(0.0f, 0.0f, 1.0f, isOpen ? 0.0f : 0.5f, isOpen ? 0.5f : 0.0f, 1.0f);

    g.setColour(backgroundColour.withAlpha(isMouseOver ? 0.5f : 0.3f));
    g.fillPath(p, p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 4), true));
}

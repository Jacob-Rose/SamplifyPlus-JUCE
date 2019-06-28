/*
  ==============================================================================

    SamplifyColorPallete.h
    Created: 27 Jun 2019 8:23:08pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SAMPLIFYCOLORPALLETE_H
#define SAMPLIFYCOLORPALLETE_H

#include "JuceHeader.h"

namespace samplify
{
	const Colour MAIN_BASE_COLOR = Colours::slategrey;
	const Colour MAIN_ACCENT_COLOR = Colours::orangered;
	const Colour MAIN_TEXT_COLOR = Colours::white;

	const int TAG_TEXT_PADDING = 2;
	const int TAG_TEXT_FONT_SIZE = 14;

	const Colour SAMPLE_TILE_COLOR_BG_DEFAULT = MAIN_BASE_COLOR;
	const Colour SAMPLE_TILE_COLOR_BG_HOVER = MAIN_BASE_COLOR.darker(0.3f);
	const Colour SAMPLE_TILE_COLOR_FG_DEFAULT = MAIN_ACCENT_COLOR;
	const Colour SAMPLE_TILE_COLOR_FG_HOVER = MAIN_ACCENT_COLOR;

	const int SAMPLE_TILE_MIN_WIDTH = 150;
	const float SAMPLE_TILE_ASPECT_RATIO = 9.0f / 16.0f;

	const int SAMPLE_TAG_FONT_SIZE = 12;
	const int SAMPLE_TAG_TEXT_PADDING = 2;
}
#endif

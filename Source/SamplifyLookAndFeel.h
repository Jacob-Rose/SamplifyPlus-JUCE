/*
  ==============================================================================

    SamplifyColorPallete.h
    Created: 27 Jun 2019 8:23:08pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SAMPLIFYLOOKANDFEEL_H
#define SAMPLIFYLOOKANDFEEL_H

#include "JuceHeader.h"
#include "Fonts.h"

namespace samplify
{
	const int MAIN_BASE_COLOR_ID = 40;
	const Colour MAIN_BASE_COLOR = Colours::white;

	const int MAIN_ACCENT_COLOR_ID = 41;
	const Colour MAIN_ACCENT_COLOR = Colours::orangered;

	const int SAMPLE_TILE_BG_DEFAULT_COLOR_ID = 44;
	const Colour SAMPLE_TILE_BG_DEFAULT_COLOR = MAIN_BASE_COLOR;

	const int SAMPLE_TILE_BG_HOVER_COLOR_ID = 45;
	const Colour SAMPLE_TILE_BG_HOVER_COLOR = MAIN_BASE_COLOR.darker();

	const int SAMPLE_TILE_FG_DEFAULT_COLOR_ID = 46;
	const Colour SAMPLE_TILE_FG_DEFAULT_COLOR = MAIN_ACCENT_COLOR;

	const int SAMPLE_TILE_FG_HOVER_COLOR_ID = 47;
	const Colour SAMPLE_TILE_FG_HOVER_COLOR = MAIN_ACCENT_COLOR.darker();

	const int SAMPLE_TILE_OUTLINE_DEFAULT_COLOR_ID = 48;
	const Colour SAMPLE_TILE_OUTLINE_DEFAULT_COLOR = Colours::red;

	const int SAMPLE_TILE_OUTLINE_HOVER_COLOR_ID = 48;
	const Colour SAMPLE_TILE_OUTLINE_HOVER_COLOR = Colours::orange;

	const int SAMPLE_TAG_FONT_SIZE = 12;
	const float SAMPLE_TAG_TEXT_PADDING = 2.0f;

	const float SAMPLE_TILE_MIN_WIDTH = 180.0f;
	const float SAMPLE_TILE_ASPECT_RATIO = 0.666;
	const float SAMPLE_TILE_CONTAINER_ITEM_PADDING = 2.0f;
	const float SAMPLE_TILE_CORNER_RADIUS = 8.0f;
	const float SAMPLE_TILE_OUTLINE_THICKNESS = 2.0f;

	//Thumbnail Properties
	const float AUDIO_THUMBNAIL_LINE_GAP_WIDTH = 0.5f; //ratio based, not pixels
	const float AUDIO_THUMBNAIL_LINE_FILL_WIDTH = 2.0f;
	const int AUDIO_THUMBNAIL_LINE_COUNT = 60;

	const Font SAMPLE_TILE_TITLE_FONT = Font(Typeface::createSystemTypefaceFor(Fonts::AbelRegular_ttf, Fonts::AbelRegular_ttfSize)).withHeight(22.0f);
}
#endif

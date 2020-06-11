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
	static Colour MAIN_BACKGROUND_COLOR = Colours::white;
	static Colour MAIN_FOREGROUND_COLOR = Colours::blueviolet;

	static int SAMPLE_TAG_FONT_SIZE = 12;
	static float SAMPLE_TAG_TEXT_PADDING = 2.0f;

	static float SAMPLE_TILE_MIN_WIDTH = 150.0f;
	static float SAMPLE_TILE_ASPECT_RATIO = 0.666f;
	static float SAMPLE_TILE_CONTAINER_ITEM_PADDING = 2.0f;
	static float SAMPLE_TILE_CORNER_RADIUS = 8.0f;
	static float SAMPLE_TILE_OUTLINE_THICKNESS = 2.0f;

	//Thumbnail Properties
	static float AUDIO_THUMBNAIL_LINE_GAP_WIDTH = 0.5f; //ratio based, not pixels
	static float AUDIO_THUMBNAIL_LINE_FILL_WIDTH = 2.0f;
	static int AUDIO_THUMBNAIL_LINE_COUNT = 60;

	const Font SAMPLE_TILE_TITLE_FONT = Font(Typeface::createSystemTypefaceFor(Fonts::AbelRegular_ttf, Fonts::AbelRegular_ttfSize)).withHeight(22.0f);
}
#endif

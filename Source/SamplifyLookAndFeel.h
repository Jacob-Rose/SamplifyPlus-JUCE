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

namespace samplify
{
	const int MAIN_BASE_COLOR_ID = 40;
	const int MAIN_ACCENT_COLOR_ID = 41;

	const int SAMPLE_TILE_BG_DEFAULT_COLOR_ID = 44;
	const int SAMPLE_TILE_BG_HOVER_COLOR_ID = 45;
	const int SAMPLE_TILE_FG_DEFAULT_COLOR_ID = 46;
	const int SAMPLE_TILE_FG_HOVER_COLOR_ID = 47;

	const int SAMPLE_TAG_FONT_SIZE = 12;
	const float SAMPLE_TAG_TEXT_PADDING = 2.0f;

	const float SAMPLE_TILE_MIN_WIDTH = 150.0f;
	const float SAMPLE_TILE_ASPECT_RATIO = 0.666;
	const float SAMPLE_TILE_CONTAINER_ITEM_PADDING = 2.0f;

	const float AUDIO_THUMBNAIL_LINE_GAP_WIDTH = 1.0f;
	const float AUDIO_THUMBNAIL_LINE_FILL_WIDTH = 2.0f;
	const int AUDIO_THUMBNAIL_LINE_COUNT = 40;
}
#endif

/*
  ==============================================================================

    AudioPlayerComponent.h
    Created: 16 Aug 2019 12:32:06pm
    Author:  jacob

  ==============================================================================
*/

#ifndef AUDIOPLAYERCOMP_H
#define AUDIOPLAYERCOMP_H

#include <JuceHeader.h>
#include "AudioPlayer.h"

namespace samplify
{
	class AudioPlayerComponent : public Component
	{
	public:
		AudioPlayerComponent();
		~AudioPlayerComponent();
	private:
		ShapeButton mPlayButton;
		ComboBox mCueSelector;
	};
}
#endif

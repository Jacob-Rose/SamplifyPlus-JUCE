/*
  ==============================================================================

    InfoWindow.h
    Created: 14 Jun 2020 8:48:43pm
    Author:  jacob

  ==============================================================================
*/

#ifndef INFOWINDOW_H
#define INFOWINDOW_H
#include "JuceHeader.h"

namespace samplify
{
	class InfoWindow : public DocumentWindow
	{
	public:
		StringArray attributions = {
		"Info Icon by bqlqn through FlatIcon",
		"Check Icon by Pixel Perfect through FlatIcon",
		"Minus Icon by Becris through FlatIcon",
		};
		InfoWindow();
		void paint(Graphics& g) override;

		void closeButtonPressed() override;
	private:

	};
}
#endif

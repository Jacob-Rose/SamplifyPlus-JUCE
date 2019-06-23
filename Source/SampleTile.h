/*
  ==============================================================================

    SampleTile.h
    Created: 31 May 2018 1:20:01pm
    Author:  Jacob Rose

  ==============================================================================
*/

#ifndef SAMPLETILE_H
#define SAMPLETILE_H

#include "JuceHeader.h"
#include "SampleReference.h"
#include "SamplifyProperties.h"

#define SAMPVIEW_WIDTH 200
#define SAMPVIEW_ASPECTRATIO (9.0f/16.0f)
#define SAMPVIEW_HEIGHT SAMPVIEW_WIDTH * SAMPVIEW_ASPECTRATIO

#define C_BACKGROUND_HOVER Colours::lightgrey
#define C_BACKGROUND_DEFAULT Colours::white
#define C_FOREGROUND_HOVER Colours::forestgreen
#define C_FOREGROUND_DEFAULT Colours::forestgreen

class SampleTile    : public Component, public DragAndDropTarget
{
public:
	//===========================================================================
    SampleTile(SampleReference*);
    ~SampleTile();

    void paint (Graphics&) override;
    void resized() override;
	//===========================================================================
	void mouseDown(const MouseEvent &e) override;
	void mouseMove(const MouseEvent &e) override;

	void playSample();
	bool isMouseWithinPlayButton(const MouseEvent &e);

	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
	void itemDropped(const SourceDetails& dragSourceDetails) override;

	void setSampleReference(SampleReference*);

	SampleReference* getSampleReference();
	bool operator==(SampleReference* ref);

private:
	SampleReference* mSampleReference = nullptr;
	//Not actual button used as the button activates something from
	//Samplereference, seemed easier to generate button then to nest a new object
	//it works
	bool isMouseOverButton = false;
	bool isButtonPressed = false;
	Rectangle<int> buttonBounds;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleTile)
};
#endif

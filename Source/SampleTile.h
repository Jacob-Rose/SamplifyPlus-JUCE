/*
  ==============================================================================

    SampleTile.h
    Created: 31 May 2018 1:20:01pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SAMPLETILE_H
#define SAMPLETILE_H

#include "JuceHeader.h"

#include "SampleReference.h"
#include "TagContainer.h"
#include "SamplifyProperties.h"

namespace samplify
{
	class SampleTile : public Component, public DragAndDropTarget
	{
	public:
		//===========================================================================
		SampleTile(SampleReference*);
		~SampleTile();

		void paint(Graphics&) override;
		void resized() override;
		//===========================================================================
		void mouseDown(const MouseEvent& e) override;
		void mouseMove(const MouseEvent& e) override;

		void playSample();
		void playSample(float t);

		bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
		void itemDropped(const SourceDetails& dragSourceDetails) override;

		void setSampleReference(SampleReference*);

		SampleReference* getSampleReference();
		//=Operator Overrides===========================================
		bool operator==(SampleReference* ref);

	private:
		SampleReference* mSampleReference = nullptr;
		TagContainer mTagContainer;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleTile)
	};
}

#endif

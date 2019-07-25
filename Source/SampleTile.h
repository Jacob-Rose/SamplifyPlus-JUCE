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

#include "Sample.h"
#include "TagContainer.h"
#include "SamplifyProperties.h"

namespace samplify
{
	class SampleTile : public Component, public DragAndDropTarget, public MouseListener, public ChangeListener
	{
	public:
		//===========================================================================
		SampleTile(Sample::Reference);
		~SampleTile();

		void paint(Graphics&) override;
		void resized() override;
		//===========================================================================
		void mouseDown(const MouseEvent& e) override;
		void mouseUp(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;
		void mouseMove(const MouseEvent& e) override;

		bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
		void itemDropped(const SourceDetails& dragSourceDetails) override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		void playSample();
		void playSample(float t);

		void setSample(Sample::Reference);
		Sample::Reference getSample();

		Rectangle<float> getTitleRect();
		Rectangle<float> getTypeRect();
		Rectangle<float> getTimeRect();
		Rectangle<float> getThumbnailRect();
		Rectangle<float> getTagRect();
		Rectangle<float> getParentDirRect();

	private:
		Sample::Reference mSample = nullptr;
		TagContainer mTagContainer;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleTile)
	};
}

#endif

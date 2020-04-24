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
	class SampleTile : public Component, 
		public DragAndDropTarget, 
		public MouseListener, 
		public ChangeListener
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
		void mouseExit(const MouseEvent& e) override;

		bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
		void itemDropped(const SourceDetails& dragSourceDetails) override;
		void changeListenerCallback(ChangeBroadcaster* source) override;
		void playSample();
		void playSample(float t);

		void setSample(Sample::Reference);
		Sample::Reference getSample();


	private:
		Sample::Reference mSample = nullptr;
		TagContainer mTagContainer;

		/*This updates all the rects, called when sampleTile resized (thus on start too)*/
		void updateRects();
		Rectangle<float> m_TitleRect;
		Rectangle<float> m_TypeRect;
		Rectangle<float> m_TimeRect;
		Rectangle<float> m_ThumbnailRect;
		Rectangle<float> m_TagRect;
		Rectangle<float> m_InfoIconRect;


		const float INFO_ICON_PADDING = 4.0f;

		//Rectangle<float> m_FavoriteButtonRect;
		//Rectangle<float> m_SaveForLaterRect;


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleTile)
	};
}

#endif

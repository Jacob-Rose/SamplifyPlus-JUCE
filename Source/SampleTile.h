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
		public ChangeListener
	{
	public:

		enum ColourIds
		{
			foregroundHoverColorID = 720,
		    foregroundDefaultColorID,
		    backgroundDefaultColorID,
			backgroundHoverColorID
		};

		enum class RightClickOptions
		{
			openExplorer = 1,
			renameSample,
			deleteSample,
			addTriggerKeyAtStart,
			addTriggerKeyAtCue
		};
		
		//===========================================================================
		SampleTile(Sample::Reference);
		~SampleTile();

		void paint(Graphics&) override;
		void resized() override;
		//===========================================================================
		void mouseUp(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;
		void mouseExit(const MouseEvent& e) override;

		bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
		void itemDropped(const SourceDetails& dragSourceDetails) override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		void setSample(Sample::Reference);
		Sample::Reference getSample();


	private:
		Sample::Reference mSample = nullptr;
		TagContainer mTagContainer;

		/*This updates all the rects, called when sampleTile resized (thus on start too)*/
		void updateRects();
		Rectangle<int> m_TitleRect;
		Rectangle<int> m_TypeRect;
		Rectangle<int> m_TimeRect;
		Rectangle<int> m_ThumbnailRect;
		Rectangle<int> m_TagRect;
		Rectangle<int> m_InfoIconRect;

		const int INFO_ICON_PADDING = 4;

		//Rectangle<float> m_FavoriteButtonRect;
		//Rectangle<float> m_SaveForLaterRect;


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleTile)
	};
}

#endif

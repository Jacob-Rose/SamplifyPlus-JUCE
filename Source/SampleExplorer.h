/*
  ==============================================================================

    SampleExplorer.h
    Created: 31 May 2018 12:43:08pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SAMPLEEXPLORER_H
#define SAMPLEEXPLORER_H

#include "JuceHeader.h"

#include "SampleContainer.h"
#include "SamplifyProperties.h"

namespace samplify
{
	class SampleExplorer : public Component, public TextEditor::Listener, public ChangeListener, public ComboBox::Listener
	{
	public:
		//============================================================
		class SampleViewport : public Viewport
		{
		public:
			SampleViewport(SampleContainer* container);
			void visibleAreaChanged(const Rectangle<int>& newVisibleArea) override;
		private:
			SampleContainer* mSampleContainer = nullptr;
		};

		//============================================================
		SampleExplorer();
		~SampleExplorer();

		void paint(Graphics&) override;
		void resized() override;

		void textEditorTextChanged(TextEditor&) override;
		void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

	private:
		//============================================================
		ComboBox mFilter;
		SampleViewport mViewport;
		TextEditor mSearchBar;
		SampleContainer mSampleContainer;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleExplorer)
	};
}

#endif

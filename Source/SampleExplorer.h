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

		class SampleSearchbar : public TextEditor, public Button::Listener
		{
		public:
			SampleSearchbar()
			{
				addAndMakeVisible(mEraseSearchButton);
				mEraseSearchButton.setButtonText("Clear");
				mEraseSearchButton.addListener(this);
			}
			void resized() override
			{
				mEraseSearchButton.setBoundsRelative(0.8f, 0.2f, 0.1f, 0.6f);
				TextEditor::resized();
			}

			void buttonClicked(Button* button)
			{
				setText("");
			}
		private:
			TextButton mEraseSearchButton;
		};

		//============================================================
		SampleExplorer();
		~SampleExplorer();

		void paint(Graphics&) override;
		void resized() override;

		void textEditorTextChanged(TextEditor&) override;
		void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		TextEditor& getSearchBar() { return mSearchBar; }
	private:
		//============================================================
		ComboBox mFilter;
		SampleViewport mViewport;
		SampleSearchbar mSearchBar;
		SampleContainer mSampleContainer;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleExplorer)
	};
}

#endif

/*
  ==============================================================================

    SampleContainer.h
    Created: 31 May 2018 1:20:21pm
    Author:  Jake Rose
	Contains SampleTiles and properly adjust depending on input search terms

  ==============================================================================
*/
#ifndef SAMPLECONTAINER_H
#define SAMPLECONTAINER_H

#include "JuceHeader.h"

#include "SampleReference.h"
#include "SampleTile.h"

namespace samplify
{
	class SampleContainer : public Component
	{
	public:
		//========================================================
		SampleContainer();
		~SampleContainer();

		void paint(Graphics&) override;
		void resized() override;

		void updateItems();
		void extendItems();
		void clearItems();

		void setSampleItems(std::vector<Sample::SampleReference> mSampleReferences);
		//======================================================
		int calculateAllRowsHeight();
		int calculateRowCount();
		int calculateColumnCount();
	private:
		//=============================================================================
		std::vector<SampleTile*> mUsedSampleTiles;
		std::vector <Sample::SampleReference> mCurrentSampleReferences;
		int mMaxItems = 20;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleContainer)
	};
}
#endif
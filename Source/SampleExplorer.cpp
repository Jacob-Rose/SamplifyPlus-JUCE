#include "SampleExplorer.h"
#include "SampleLibrary.h"
#include "SamplifyProperties.h"
#include "SamplifyMainComponent.h"

using namespace samplify;

SampleExplorer::SampleExplorer() : mViewport(&mSampleContainer)
{
    addAndMakeVisible(mViewport);
	addAndMakeVisible(mFilter);
	addAndMakeVisible(mVolume);
	mVolume.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	mVolume.setRange(0.0f, 2.0f);
	//mVolume.setRotaryParameters(0, (MathConstants<float>().pi * 2), true);
	mVolume.setValue(1.0f);
	mVolume.addListener(this);
	mFilter.addItem("A-Z", 1);
	mFilter.addItem("Z-A", 2);
	mFilter.addItem("Newest", 3);
	mFilter.addItem("Oldest", 4);
	mFilter.addItem("Random", 5);
	mViewport.addAndMakeVisible(mSampleContainer);
	addAndMakeVisible(mSearchBar);
	mViewport.setViewedComponent(&mSampleContainer);
	mViewport.setScrollBarsShown(true, false, true, false);
	mSearchBar.addListener(this);
	mFilter.addListener(this);
	//mSampleContainer.setSampleItems(SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples());
	SamplifyProperties::getInstance()->getSampleLibrary()->addChangeListener(this);
}

SampleExplorer::~SampleExplorer()
{
}

void SampleExplorer::paint (Graphics& g)
{
    
}

void SampleExplorer::resized()
{
    mSearchBar.setBounds(0, 0, getWidth() - 120, 30);
	mFilter.setBounds(getWidth() - 120, 0, 120, 30);
	mViewport.setBounds(0,30,getWidth(), getHeight()-30);
	mSampleContainer.setBounds(mViewport.getBounds().withRight(mViewport.getWidth() - mViewport.getScrollBarThickness()));
	mVolume.setBoundsRelative(0.8f, 0.8f, 0.2f, 0.2f);
}

void SampleExplorer::textEditorTextChanged(TextEditor& e)
{
	SamplifyProperties::getInstance()->getSampleLibrary()->updateCurrentSamples(e.getText());
}

void SampleExplorer::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
}

void SampleExplorer::changeListenerCallback(ChangeBroadcaster* source)
{
	mSampleContainer.setSampleItems(SamplifyProperties::getInstance()->getSampleLibrary()->getCurrentSamples());
}

void samplify::SampleExplorer::sliderValueChanged(Slider* slider)
{
	float gain = slider->getValue() * 0.5f;
	SamplifyProperties::getInstance()->getAudioPlayer()->setVolumeMultiply(gain);
}

SampleExplorer::SampleViewport::SampleViewport(SampleContainer* container)
{
	mSampleContainer = container;
}

void SampleExplorer::SampleViewport::visibleAreaChanged(const Rectangle<int>& newVisibleArea)
{
	int height = mSampleContainer->getHeight();
	int bottomBounds = newVisibleArea.getBottomLeft().y;
	int topBounds = newVisibleArea.getTopLeft().y;
	if (height == bottomBounds)
	{
		mSampleContainer->extendItems();
		
		setViewPosition(0, topBounds);
	}
}
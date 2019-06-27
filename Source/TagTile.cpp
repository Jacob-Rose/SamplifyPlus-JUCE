#include "TagTile.h"
#include "SamplifyProperties.h"

//==============================================================================
TagTile::TagTile(juce::String tag)
{
	mTag = tag;
}

TagTile::~TagTile()
{
}

void TagTile::paint (Graphics& g)
{
	//todo replace
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (SamplifyProperties::getInstance()->getTagColor(mTag));
    g.setFont (14.0f);
    g.drawText (mTag, getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void TagTile::resized()
{
	//repainting is automatic
}

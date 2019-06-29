#include "TagTile.h"
#include "SamplifyProperties.h"
#include "SamplifyColorPallete.h"

using namespace samplify;
TagTile::TagTile(juce::String tag)
{
	mTag = tag;
}

TagTile::~TagTile()
{
}

void samplify::TagTile::setTag(juce::String tag)
{
	mTag = tag;
	repaint();
}

void TagTile::paint (Graphics& g)
{
	Colour mainColor = SamplifyProperties::getInstance()->getTagColor(mTag);
    g.setColour (mainColor);
    g.fillRoundedRectangle (getLocalBounds().toFloat(), 1.0f);   // draw an outline around the component
	g.setColour(mainColor.darker());
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 1.0f, 1.0f);
	float oldFontSize = g.getCurrentFont().getHeight();
	if (mainColor.getPerceivedBrightness() > 0.5f)
	{
		g.setColour(Colours::black);
	}
	else
	{
		g.setColour(Colours::white);
	}
    g.setFont (SAMPLE_TAG_FONT_SIZE);
    g.drawText (mTag, getLocalBounds(), Justification::centred, true); 
	g.setFont(oldFontSize);
}

void TagTile::resized()
{
	//repainting is automatic
}

void TagTile::mouseDown(const MouseEvent& e)
{
}

void TagTile::mouseMove(const MouseEvent& e)
{
}

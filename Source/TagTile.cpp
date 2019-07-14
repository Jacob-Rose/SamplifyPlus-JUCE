#include "TagTile.h"
#include "SamplifyProperties.h"
#include "SamplifyLookAndFeel.h"

using namespace samplify;
TagTile::TagTile(juce::String tag, Font& font)
{
	mTag = tag;
	mFont = &font;
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
	if (mTag != "")
	{
		Colour mainColor = SamplifyProperties::getInstance()->getTagColor(mTag);
		g.setColour(mainColor);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 1.0f);   // draw an outline around the component
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
		g.setFont(*mFont);
		g.drawText(mTag, getLocalBounds(), Justification::centred, true);
		g.setFont(oldFontSize);
	}
}

void TagTile::resized()
{
	//repainting is automatic
}


void TagTile::mouseUp(const MouseEvent& e)
{
	if (e.mouseWasClicked())
	{
		
	}
}

void samplify::TagTile::mouseDrag(const MouseEvent& e)
{
	startDragging("Tags", this, juce::Image().null, true);
}

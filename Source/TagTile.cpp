#include "TagTile.h"
#include "SamplifyProperties.h"
#include "SampleTile.h"
#include "SamplifyLookAndFeel.h"
#include "SamplifyMainComponent.h"

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
		Colour mainColor = SamplifyProperties::getInstance()->getTagLibrary().getTagColor(mTag);
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


void TagTile::mouseDown(const MouseEvent& e)
{
	
}

void TagTile::mouseUp(const MouseEvent& e)
{
	if (!isDragAndDropActive())
	{
		if (e.mods.isLeftButtonDown())
		{
			//todo set sample container filter
			String text = SamplifyMainComponent::getInstance()->getSampleExplorer().getSearchBar().getText();
			SamplifyMainComponent::getInstance()->getSampleExplorer().getSearchBar().setText("#" + mTag);
		}
		else if (e.mods.isRightButtonDown())
		{
			PopupMenu menu;
			//TagContainer -> sampleTile?
			if (SampleTile * parent = dynamic_cast<SampleTile*>(getParentComponent()->getParentComponent()))
			{
				menu.addItem(1, "Edit Tag", false, false);
				menu.addItem(2, "Untag", true, false);
				int selection = menu.show();
				if (selection == 1)
				{

				}
				else if (selection == 2)
				{
					parent->getSample().removeTag(mTag);
					parent->repaint();
				}
			}
			else
			{
				menu.addItem(1, "Edit Tag", false, false);
				menu.addItem(2, "Delete Tag (+ Refs)", true, false);
				int selection = menu.show();
				if (selection == 1)
				{

				}
				else if (selection == 2)
				{
					//todo add confirm screen
					SamplifyProperties::getInstance()->getTagLibrary().deleteTag(mTag);
					if (SamplifyMainComponent::getInstance() != nullptr)
					{
						SamplifyMainComponent::getInstance()->getFilterExplorer().getTagExplorer().getTagContainer().removeNewTag(mTag);
						SamplifyMainComponent::getInstance()->getFilterExplorer().getTagExplorer().getTagContainer().resetTags();
					}
					
				}
			}

		}
	}
}

void TagTile::mouseDrag(const MouseEvent& e)
{
	startDragging("Tags", this, juce::Image(), true);
}

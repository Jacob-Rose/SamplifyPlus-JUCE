/*
  ==============================================================================

    SamplifyLookAndFeel.cpp
    Created: 12 Jun 2020 5:42:06pm
    Author:  jacob

  ==============================================================================
*/

#include "SamplifyLookAndFeel.h"
#include "Icons.h"

using namespace samplify;

std::unique_ptr<AppValues> AppValues::instance = nullptr;

void AppValues::initInstance()
{
    instance = std::make_unique<AppValues>();
    instance->loadDrawables();
}

void AppValues::cleanupInstance()
{
    instance->cleanupDrawables();
    instance.reset();
}
AppValues& AppValues::getInstance()
{ 
    return *instance; 
}

Drawable* AppValues::getDrawable(String id)
{
    std::map<String, std::unique_ptr<Drawable>>::iterator it= mDrawables.find(id);
    if (it != mDrawables.end())
    {
        return (it->second).get();
    }
    return nullptr;
}

void AppValues::loadDrawables()
{
	std::unique_ptr<XmlElement> correctSVG = XmlDocument::parse(Icons::correct_svg);
	mDrawables["correct"] = Drawable::createFromSVG(*correctSVG);
	mDrawables["correct"]->replaceColour(Colours::black, Colours::white);

	std::unique_ptr<XmlElement> minusSVG = XmlDocument::parse(Icons::minus_svg);
	mDrawables["minus"] = Drawable::createFromSVG(*minusSVG);
	mDrawables["minus"]->replaceColour(Colours::black, Colours::white);

	std::unique_ptr<XmlElement> infoSVG = XmlDocument::parse(Icons::info_svg);
	mDrawables["info"] = Drawable::createFromSVG(*infoSVG);
	mDrawables["info"]->replaceColour(Colours::black, Colours::white);

    std::unique_ptr<XmlElement> crossSVG = XmlDocument::parse(Icons::close_svg);
    mDrawables["close"] = Drawable::createFromSVG(*infoSVG);
    mDrawables["close"]->replaceColour(Colours::black, Colours::white);
}

void AppValues::cleanupDrawables()
{
   
}

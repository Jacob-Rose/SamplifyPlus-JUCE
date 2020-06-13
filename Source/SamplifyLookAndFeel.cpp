/*
  ==============================================================================

    SamplifyLookAndFeel.cpp
    Created: 12 Jun 2020 5:42:06pm
    Author:  jacob

  ==============================================================================
*/

#include "SamplifyLookAndFeel.h"
using namespace samplify;

std::unique_ptr<AppValues> AppValues::instance = nullptr;

void samplify::AppValues::initInstance()
{
    instance = std::make_unique<AppValues>();
}

void samplify::AppValues::cleanupInstance()
{
    instance.reset();
}
AppValues& samplify::AppValues::getInstance()
{ 
    return *instance; 
}
/*
  ==============================================================================

    SortingMethod.h
    Created: 11 Jun 2020 2:08:20pm
    Author:  Jake Rose

  ==============================================================================
*/

#ifndef SORTINGMETHODS_H
#define SORTINGMETHODS_H
const enum class SortingMethod
{
	Newest,
	Oldest,
	Recent,
	Popular,
	Random
};

const std::map<SortingMethod, juce::String> sortingNames = {
	{SortingMethod::Newest, "Newest"},
	{SortingMethod::Oldest, "Oldest"},
	{SortingMethod::Recent, "Recent"},
	{SortingMethod::Popular, "Popular"},
	{SortingMethod::Random, "Randomize"} };
#endif
/*
  ==============================================================================

    SamplifyCore.h
    Created: 6 Sep 2019 1:04:31pm
    Author:  jacob

  ==============================================================================
*/

#ifndef SAMPLIFYCORE_H
#define SAMPLIFYCORE_H
#include "JuceHeader.h"
#include "Sample.h"
#include <future>
#include <vector>
namespace samplify
{

	

	class TagLibrary
	{
	public:
		void addTag(juce::String tag, Colour color);
		void addTag(juce::String tag);
		void renameTag(juce::String currentTagName, juce::String desiredName);
		void deleteTag(juce::String tag);
		//StringArray getAllTags();

		Colour getTagColor(juce::String text);
	private:
		std::map<juce::String, Colour> mSampleTagColors;
		JUCE_LEAK_DETECTOR(TagLibrary);
	};

	class SamplifyCore : public ChangeListener, public ChangeBroadcaster
	{
	public:
		static std::shared_ptr<SamplifyCore> getInstance() { return msInstance; }
		static void createInstance();
		void init();
		void cleanup();

		void addDirectory(const File& newDir);
		void removeDirectory(const File& dirToRemove);

		Sample::List getAllSamples();
		Sample::List getSearchedSamples(juce::String query);
		//todo make custom promise and future objects
		std::promise<Sample::List> getSearchedSamplesAsync(juce::String query);

		static File browseForDirectory();
	private:
		SamplifyCore();
		~SamplifyCore();
		bool mIsInit = false;
		
		ApplicationProperties mProperties;
		static std::shared_ptr<SamplifyCore> msInstance;
	};
}
#endif
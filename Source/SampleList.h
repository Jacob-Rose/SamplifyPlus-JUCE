/*
  ==============================================================================

    SampleList.h
    Created: 10 Sep 2019 2:57:19pm
    Author:  jacob

  ==============================================================================
*/

#ifndef SAMPLELIST_H
#define SAMPLELIST_H
#include "Sample.h"
#include <vector>
namespace samplify
{
	enum SortMethod
	{
		None = -1,
		Alphabetical,
		ReverseAlphabetical,
		Newest,
		Oldest,
		Random
	};

	class SampleList
	{
	public:
		SampleList(const std::vector<Sample::Reference>& list);
		SampleList();
		int size() const;
		virtual void addSample(const Sample::Reference& sample);
		void addSamples(const SampleList& list);
		void addSamples(const std::vector<Sample::Reference>& samples);
		void removeSample(Sample::Reference sample);
		void removeSample(int index);
		void removeSamples(std::vector<Sample::Reference> samples);
		void removeSamples(const SampleList& list);
		void clearSamples();

		Sample::Reference operator[](int index) const;
		friend SampleList operator+(const SampleList& lhs, const SampleList& rhs);
	protected:
		std::vector<Sample::Reference> mSamples;

		JUCE_LEAK_DETECTOR(List)
	};

	inline SampleList operator+(const SampleList& lhs, const SampleList& rhs)
	{
		SampleList combined;
		combined.addSamples(lhs);
		combined.addSamples(rhs);
		return combined;
	}
}
#endif

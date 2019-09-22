#include "SampleLibrary.h"
#include "SamplifyMainComponent.h"

using namespace samplify;

SampleLibrary::SampleLibrary(std::shared_ptr<SampleDirectoryManager> manager)
{
}

SampleLibrary::~SampleLibrary()
{
}

void SampleLibrary::updateCurrentSamples(String query)
{
	mCurrentQuery = query;
	updateCurrentSamples(query);
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* source)
{
	refreshCurrentSamples();
}

Sample::List SampleLibrary::getCurrentSamples()
{
	return mCurrentSamples;
}
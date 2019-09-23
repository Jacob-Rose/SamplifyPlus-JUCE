#include "SampleLibrary.h"
#include "SamplifyMainComponent.h"

using namespace samplify;

SampleLibrary::SampleLibrary(std::shared_ptr<SampleDirectoryManager> manager)
{
	mDirectoryManager = manager;
}

SampleLibrary::~SampleLibrary()
{
}

void SampleLibrary::updateCurrentSamples(String query)
{
	mCurrentQuery = query;
	std::future<Sample::List> future = mDirectoryManager->getAllSamplesAsync(query);
	future.wait();
	if (future.valid())
		mCurrentSamples = future.get();
	sendChangeMessage();
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* source)
{
	refreshCurrentSamples();
}

Sample::List SampleLibrary::getCurrentSamples()
{
	return mCurrentSamples;
}
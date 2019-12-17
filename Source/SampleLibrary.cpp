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
	if (updateSampleFuture != nullptr)
	{
		updateSampleFuture->_Abandon();
		updateSampleFuture = nullptr;
	}
	updateSampleFuture = std::make_unique<std::future<Sample::List>>(mDirectoryManager->getAllSamplesAsync(query));
	updateSampleFuture->wait();
	if (updateSampleFuture->valid())
		mCurrentSamples = updateSampleFuture->get();
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
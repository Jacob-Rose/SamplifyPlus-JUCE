#include "TagExplorer.h"
#include "SamplifyProperties.h"

void TagExplorer::updateTags(std::string newSearch)
{
	mContainedTags = SamplifyProperties::getInstance()->getSampleLibrary()->getAllTags();
	for (int i = 0; i < mContainedTags.size(); i++)
	{
		if (mContainedTags[i].contains(newSearch));
	}
}
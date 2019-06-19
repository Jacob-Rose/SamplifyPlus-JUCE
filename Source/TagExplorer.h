#include "JuceHeader.h"

class TagExplorer : public Component
{
public:
	void updateTags(std::string newSearch);
private:
	TextButton mNewButtonTag;
	StringArray mContainedTags; //all tags for current files in directory with text from search bar contained in it
	StringArray mNotContainedTags; //all tags for lower half, only ones with the text in search bar contained in it
	FlexBox mTagFlexBox;
};
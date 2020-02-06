/*
  ==============================================================================

    DirectoryLibrary.h
    Created: 16 Aug 2019 6:20:03pm
    Author:  jacob

  ==============================================================================
*/

#ifndef DIRECTORYLIBRARY_H
#define DIRECTORYLIBRARY_H
#include "JuceHeader.h"
namespace samplify
{
	/*
	class DirectoryLibrary : public ChangeBroadcaster
	{
	public:
		void addDirectory(File dir);
		void removeDirectory(File dir);
		void setDirectories(std::vector<File> directories);
		std::vector<File> getDirectories() const { return mDirectories; }
		std::vector<File> getLastDirectories() const { return mLastDirectories; }
		File getRelativeDirectoryForFile(const File& sampleFile) const {
			for (int i = 0; i < mDirectories.size(); i++)
			{
				if (sampleFile.isAChildOf(mDirectories[i]))
				{
					return mDirectories[i];
				}
			}
			return File();
		}
		void clearDirectories();
	private:
		std::vector<File> mDirectories;
		std::vector<File> mLastDirectories; //to determine change
		JUCE_LEAK_DETECTOR(DirectoryLibrary)
	};
	*/
}
#endif

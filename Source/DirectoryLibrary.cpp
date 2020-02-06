#include "DirectoryLibrary.h"
using namespace samplify;
/*
void DirectoryLibrary::setDirectories(std::vector<File> directories)
{
	mLastDirectories = mDirectories;
	mDirectories = directories;
	sendChangeMessage();
}

void DirectoryLibrary::removeDirectory(File dir)
{
	mLastDirectories = mDirectories;
	for (int i = 0; i < mDirectories.size(); i++)
	{
		if (mDirectories[i] == dir)
		{
			mDirectories.erase(mDirectories.begin() + i);
			break;
		}
	}
	sendChangeMessage();
}

void DirectoryLibrary::addDirectory(File dir)
{
	mLastDirectories = mDirectories;
	mDirectories.push_back(dir);
	sendChangeMessage();
}

void DirectoryLibrary::clearDirectories()
{
	mLastDirectories = mDirectories;
	for (int i = 0; i < mDirectories.size(); i++)
	{
		//the function modifies mDirectories
		removeDirectory(mDirectories[0]);
	}
	sendChangeMessage();
}
*/
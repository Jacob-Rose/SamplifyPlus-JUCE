#include "SampleTile.h"

#include "SamplifyLookAndFeel.h"
#include "TagTile.h"
#include "SamplifyMainComponent.h"

#include <iomanip>
#include <sstream>

using namespace samplify;

SampleTile::SampleTile(Sample::Reference sample) : mTagContainer(false)
{
	setRepaintsOnMouseActivity(true);
	setSize(AppValues::getInstance().SAMPLE_TILE_MIN_WIDTH, AppValues::getInstance().SAMPLE_TILE_MIN_WIDTH * AppValues::getInstance().SAMPLE_TILE_ASPECT_RATIO);
	setSample(sample);
    mTagContainer.addMouseListener(this, false);
	addAndMakeVisible(mTagContainer);
}

SampleTile::~SampleTile()
{
}
void SampleTile::paint (Graphics& g)
{
	if (!mSample.isNull())
	{
		//setup colors to use
		Colour backgroundColor;
		Colour foregroundColor;
		Colour outlineColor;
		Colour titleColor;
		if (isMouseOver(true))
		{
			backgroundColor = getLookAndFeel().findColour(backgroundHoverColorID);
			foregroundColor = getLookAndFeel().findColour(foregroundHoverColorID);
			outlineColor = getLookAndFeel().findColour(foregroundHoverColorID);
		}
		else
		{
			backgroundColor = getLookAndFeel().findColour(backgroundDefaultColorID);
			foregroundColor = getLookAndFeel().findColour(foregroundDefaultColorID);
			outlineColor = getLookAndFeel().findColour(foregroundDefaultColorID);
		}

		if (backgroundColor.getPerceivedBrightness() > 0.5f)
		{
			titleColor = Colours::slategrey;
		}
		else
		{
			titleColor = Colours::whitesmoke;
		}

		//Draw BG
		g.setColour(backgroundColor);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), AppValues::getInstance().SAMPLE_TILE_CORNER_RADIUS);
		if (mSample.getInfoText() != "")
		{
			//Draw info icon
			g.setColour(foregroundColor);
			g.fillEllipse(m_InfoIconRect.reduced(INFO_ICON_PADDING).toFloat());
			g.setColour(outlineColor);
			g.drawEllipse(m_InfoIconRect.reduced(INFO_ICON_PADDING).toFloat(), AppValues::getInstance().SAMPLE_TILE_OUTLINE_THICKNESS);

			//Draw Title
			g.setFont(SAMPLE_TILE_TITLE_FONT);
			g.setColour(titleColor);
			g.drawText(mSample.getFile().getFileName(), m_TitleRect.withTrimmedLeft(m_InfoIconRect.getWidth()), Justification::centredLeft);
		}
		else
		{
			//Draw Title
			g.setFont(SAMPLE_TILE_TITLE_FONT);
			g.setColour(titleColor);
			g.drawText(mSample.getFile().getFileName(), m_TitleRect.withTrimmedLeft(2.0f), Justification::centredLeft);
		}

		
		g.setColour(titleColor);
		//Draw Time
		g.setFont(16.0f);
		std::stringstream secondsStr;
		std::stringstream minutesStr;
		int minutes = ((int)mSample.getLength()) / 60;
		secondsStr << std::fixed << std::setprecision(1) << (mSample.getLength() - (60.0*minutes));
		minutesStr << std::fixed << minutes;
		g.drawText(String(secondsStr.str()) + " sec", m_TimeRect, Justification::bottom);
		g.drawText(String(minutesStr.str()) + " min", m_TimeRect, Justification::top);
		

		//Draw Thumbnail
		g.setColour(foregroundColor);
		std::shared_ptr<SampleAudioThumbnail> thumbnail = mSample.getThumbnail();
		if (thumbnail->isFullyLoaded())
		{
			if (thumbnail->getNumChannels() != 0)
			{
				thumbnail->drawChannel(g, m_ThumbnailRect.toNearestInt(), 0.0, thumbnail->getTotalLength(), 0, 1.0f);
			}
		}

		//Draw Audio Line if playing
		std::shared_ptr<AudioPlayer> auxPlayer = SamplifyProperties::getInstance()->getAudioPlayer();
		if (auxPlayer->getSampleReference() == mSample)
		{
			float startT = auxPlayer->getStartCueRelative();
			float currentT = auxPlayer->getRelativeTime();
			float startX = m_ThumbnailRect.getTopLeft().x + ((m_ThumbnailRect.getTopRight().x - m_ThumbnailRect.getTopLeft().x) * startT);
			float currentX = m_ThumbnailRect.getTopLeft().x + ((m_ThumbnailRect.getTopRight().x - m_ThumbnailRect.getTopLeft().x) * currentT);
			float y1 = m_ThumbnailRect.getTopLeft().y;
			float y2 = m_ThumbnailRect.getBottomLeft().y;
			g.setColour(Colours::black);
			g.drawLine(startX, y1, startX, y2, 1.0f);
			if (auxPlayer->getState() == AudioPlayer::TransportState::Playing)
			{
				g.setColour(Colours::red);
				g.drawLine(currentX, y1, currentX, y2, 1.0f);
				repaint();
			}
		}
		//set tags
		mTagContainer.setTags(mSample.getTags());

		g.setColour(outlineColor);
		g.drawRoundedRectangle(getLocalBounds().reduced(1).toFloat(), AppValues::getInstance().SAMPLE_TILE_CORNER_RADIUS, AppValues::getInstance().SAMPLE_TILE_OUTLINE_THICKNESS);
	}
	else
	{
		//reset all info if no sample currently active
		mTagContainer.setTags(StringArray());
	}
}

void SampleTile::resized()
{
	updateRects();
}

bool SampleTile::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
	if (dragSourceDetails.description == "Tags")
	{
		return true;
	}
	else
	{
		return false;
	}
}


void SampleTile::mouseUp(const MouseEvent& e)
{
	if (!mSample.isNull())
	{
		if (e.mods.isLeftButtonDown())
		{
			SamplifyProperties::getInstance()->getAudioPlayer()->loadFile(mSample);
			if (m_ThumbnailRect.contains(e.getMouseDownPosition()))
			{
				SamplifyProperties::getInstance()->getAudioPlayer()->playSample();
			}
		}
		else if (e.mods.isRightButtonDown())
		{
			/*
			if (m_ThumbnailRect.contains(e.getMouseDownPosition().toFloat()))
			{
				float rectWidth = m_ThumbnailRect.getWidth();
				float mouseDownX = e.getMouseDownX();
				playSample(mouseDownX / rectWidth);
			}
			else if (m_TitleRect.contains(e.getMouseDownPosition().toFloat()) && e.mods.isLeftButtonDown())
			{
				PopupMenu menu;
				StringArray parentDirs = mSample.getRelativeParentFolders();
				for (int i = 0; i < parentDirs.size(); i++)
				{
					menu.addItem(i + 1, parentDirs[i]);
				}
				int choice = menu.show();
			}

			else
			{
						*/
				PopupMenu menu;
				menu.addItem((int)RightClickOptions::openExplorer, "Open in Explorer", true, false); //QEDITOR IS THE PLACE TO BREAK A SAMPLE
				menu.addSeparator();
				menu.addItem((int)RightClickOptions::renameSample, "Rename Sample", false, false);
				menu.addItem((int)RightClickOptions::deleteSample, "Delete (Move to Trash) ", false, false);
				menu.addItem((int)RightClickOptions::deleteSample, "Delete (Permanant)", false, false);
				menu.addSeparator();
				menu.addItem((int)RightClickOptions::addTriggerKeyAtStart, "Add Trigger at Start", false, false);
				menu.addItem((int)RightClickOptions::addTriggerKeyAtCue, "Add Trigger at Cue", false, false);
				int selection = menu.show();
				if (selection == (int)RightClickOptions::openExplorer)
				{
					mSample.getFile().revealToUser();
				}
				else if (selection == (int)RightClickOptions::renameSample)
				{
					//rename sample (do not activate until the list updated with the new name)
					FileChooser newFile("rename file", mSample.getFile());
					if (newFile.browseForFileToSave(true))
					{
						mSample.getFile().moveFileTo(newFile.getResult());
						//todo update list
					}
				}
				else if (selection == (int)RightClickOptions::deleteSample)
				{
					//delete sample (do not activate until the list updated with deleted item)
					if (!mSample.getFile().moveToTrash()) //if failed
					{
						NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error in Throwing Away", "Failed to move item to trash, check if it is full!");
					}
					//todo update list 
				}
				else if (selection == (int)RightClickOptions::addTriggerKeyAtStart)
				{
					//todo
				}
				else if (selection == (int)RightClickOptions::addTriggerKeyAtCue)
				{
					//todo
				}
			}
		//}
	}
}

void SampleTile::mouseDrag(const MouseEvent& e)
{
	if (!mSample.isNull())
	{
		StringArray files = StringArray();
		files.add(mSample.getFile().getFullPathName());
		DragAndDropContainer::performExternalDragDropOfFiles(files, false);
	}
}

void SampleTile::mouseExit(const MouseEvent& e)
{
	repaint();
}


void SampleTile::itemDropped(const SourceDetails & dragSourceDetails)
{
	if (!mSample.isNull())
	{
		if (TagTile * tagComp = dynamic_cast<TagTile*>(dragSourceDetails.sourceComponent.get()))
		{
			mSample.addTag(tagComp->getTag());
			mTagContainer.setTags(mSample.getTags());
		}
	}
}

void SampleTile::changeListenerCallback(ChangeBroadcaster* source)
{
	if (!mSample.isNull())
	{
		std::shared_ptr<AudioPlayer> aux = SamplifyProperties::getInstance()->getAudioPlayer();
		if (aux->getSampleReference() == mSample)
		{
			if (!(aux->getState() == AudioPlayer::TransportState::Starting ||
				aux->getState() == AudioPlayer::TransportState::Stopped || 
				aux->getState() == AudioPlayer::TransportState::Stopping))
			{
				aux->removeChangeListener(this);
			}
		}
	}
	repaint();
}

void SampleTile::setSample(Sample::Reference sample)
{
	if (!sample.isNull())
	{
		bool alreadyThis = false;
		if (!mSample.isNull())
		{
			if (mSample == sample)
			{
				alreadyThis = true;
			}
			else
			{
				mSample.removeChangeListener(this);
			}
		}
		if (!alreadyThis)
		{
			sample.generateThumbnailAndCache();
			sample.addChangeListener(this);
		}
	}
	mSample = sample;
	repaint();
}

Sample::Reference SampleTile::getSample()
{
	return mSample;
}


void SampleTile::updateRects()
{
	//Core Rects
	m_TitleRect = Rectangle<int>(0, 0, getWidth(), SAMPLE_TILE_TITLE_FONT.getHeight() + 4.0f);
	m_TypeRect = Rectangle<int>(0, getHeight() - (getWidth() / 5), getWidth() / 5, getWidth() / 5);
	m_TimeRect = Rectangle<int>(getWidth() / 5, getHeight() - (getWidth() / 5), getWidth() / 5, getWidth() / 5);

	//Derivative Rects
	int startY = m_TitleRect.getHeight();
	m_ThumbnailRect = Rectangle<int>(0, startY, getWidth(), getHeight() - (startY + (getWidth() / 5)));

	int offset = (m_TitleRect.getHeight() + m_ThumbnailRect.getHeight());
	m_TagRect = Rectangle<int>(getWidth() / 2, offset, getWidth() / 2, getHeight() - offset);
	mTagContainer.setBounds(m_TagRect.toNearestInt());

	m_InfoIconRect = Rectangle<int>(0, 0, m_TitleRect.getHeight(), m_TitleRect.getHeight()); //square in top right

}

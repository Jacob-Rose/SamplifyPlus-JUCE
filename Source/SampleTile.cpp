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
	addAndMakeVisible(m_InfoIcon);
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

		Rectangle<int> titleRect;
		if (mSample.getInfoText() != "" || mSample.getColor().getAlpha() != 0.0f)
		{
			//Draw info icon
			if (mSample.getColor().getFloatAlpha() > 0.0f)
			{
				g.setColour(mSample.getColor());
				g.fillEllipse(m_InfoIcon.getBounds().reduced(INFO_ICON_PADDING).toFloat());
				g.setColour(mSample.getColor().darker());
				g.drawEllipse(m_InfoIcon.getBounds().reduced(INFO_ICON_PADDING).toFloat(), AppValues::getInstance().SAMPLE_TILE_OUTLINE_THICKNESS);
			}

			titleRect = m_TitleRect.withTrimmedLeft(m_InfoIcon.getWidth());
		}
		else 
		{
			titleRect = m_TitleRect.withTrimmedLeft(2.0f);
		}
		g.setFont(SAMPLE_TILE_TITLE_FONT);
		g.setColour(titleColor);
		g.drawText(mSample.getFile().getFileName(), titleRect, Justification::centredLeft);

		//Draw Time
		g.setColour(titleColor);
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

	m_InfoIcon.setBounds(0, 0, m_TitleRect.getHeight(), m_TitleRect.getHeight()); //square in top right
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
			
			if (m_ThumbnailRect.contains(e.getMouseDownPosition()) && AppValues::getInstance().RIGHTCLICKPLAYFROMPOINT)
			{
				float rectWidth = m_ThumbnailRect.getWidth();
				float mouseDownX = e.getMouseDownX();
				SamplifyProperties::getInstance()->getAudioPlayer()->loadFile(mSample);
				SamplifyProperties::getInstance()->getAudioPlayer()->playSample(mouseDownX / rectWidth);
			}
			/*
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
			*/
			else
			{	
				PopupMenu menu;
				menu.addItem((int)RightClickOptions::openExplorer, "Open in Explorer", true, false); //QEDITOR IS THE PLACE TO BREAK A SAMPLE
				menu.addSeparator();
				menu.addItem((int)RightClickOptions::renameSample, "Rename", true, false);
				menu.addItem((int)RightClickOptions::deleteSample, "Move To Trash", true, false);
				//menu.addSeparator();
				//menu.addItem((int)RightClickOptions::addTriggerKeyAtStart, "Add Trigger at Start", false, false);
				//menu.addItem((int)RightClickOptions::addTriggerKeyAtCue, "Add Trigger at Cue", false, false);
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
						if (mSample.getFile().moveFileTo(newFile.getResult()))
						{
							SamplifyProperties::getInstance()->getSampleLibrary()->refreshCurrentSamples();
						}
					}
				}
				else if (selection == (int)RightClickOptions::deleteSample)
				{
					int shouldDelete = NativeMessageBox::showYesNoBox(AlertWindow::AlertIconType::WarningIcon, "Delete Sample?", "Are you sure you want to delete this sample?");
					if (shouldDelete == 1)
					{
						if (mSample.getFile().moveToTrash())
						{
							SamplifyProperties::getInstance()->getSampleLibrary()->refreshCurrentSamples();
						}
						else
						{
							NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error in Throwing Away", "Failed to move item to trash, check if it is full!");
						}
					}
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
		}
	}
}

void SampleTile::mouseDrag(const MouseEvent& e)
{
	if (!mSample.isNull())
	{
		StringArray files = StringArray();
		files.add(mSample.getFile().getFullPathName());
		DragAndDropContainer::performExternalDragDropOfFiles(files, false);
		SamplifyProperties::getInstance()->getAudioPlayer()->stop();
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
		m_InfoIcon.setTooltip(mSample.getInfoText());
	}
	resized();
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
			m_InfoIcon.setTooltip(sample.getInfoText());
			sample.addChangeListener(this);
		}
	}
	else
	{
		m_InfoIcon.setTooltip("");
	}
	mSample = sample;
	repaint();
}

Sample::Reference SampleTile::getSample()
{
	return mSample;
}

SampleTile::InfoIcon::InfoIcon()
{

}

String SampleTile::InfoIcon::getTooltip()
{
	return mTooltip;
}

void SampleTile::InfoIcon::setTooltip(String newTooltip)
{
	mTooltip = newTooltip;
	repaint();
}

void SampleTile::InfoIcon::paint(Graphics& g)
{
	if (mTooltip != "")
	{
		AppValues::getInstance().getDrawable("info")->drawWithin(g, getBounds().reduced(4.0f).toFloat(), RectanglePlacement::centred, 1.0f);
	}
}

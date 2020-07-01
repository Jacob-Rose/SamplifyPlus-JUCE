/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

	Modified by: Jake Rose

  ==============================================================================
*/

#include "JuceHeader.h"

#include "LookAndFeel_VJake.h"

#include "SamplifyMainComponent.h"
#include "SamplifyMenuBar.h"
#include "SamplifyProperties.h"
#include "SamplifyLookAndFeel.h"
#include "Sample.h"

namespace samplify
{
	class SamplifyPlusApplication : public JUCEApplication
	{
	public:
		//==============================================================================
		SamplifyPlusApplication() {	}

		const String getApplicationName() override { return ProjectInfo::projectName; }
		const String getApplicationVersion() override { return ProjectInfo::versionString; }
		bool moreThanOneInstanceAllowed() override { return false; }

		//==============================================================================
		void initialise(const String& commandLine) override
		{
			Time timeToKill = Time(2020, 8, 1, 0, 0); //month is between 0-11
			Time cTime = Time::getCurrentTime();
			if (cTime.toMilliseconds() > timeToKill.toMilliseconds())
			{
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "App Beta Expired", "The Application has passed its usage date. The application will now close. \nThank you for your participation. \nThis is one of my few ways to ensure security and encourage more feedback from participants.");
				quit();
			}

			//I KNOW THIS IS INEFFICIENT, BUT LIKE HONESTLY I CREATE IT QUICKLY AND DELETE SO FUCK YOU
			std::shared_ptr<AudioPlayer> audioPlayer = std::make_shared<AudioPlayer>();
			SampleDirectory::mWildcard = audioPlayer->getFormatManager()->getWildcardForAllFormats();
			AppValues::initInstance();
			SamplifyProperties::initInstance();
			mainWindow.reset(new MainWindow(getApplicationName()));
			//}
		}

		void shutdown() override
		{
			mainWindow = nullptr; // (deletes our window)
			SamplifyProperties::cleanupInstance();
			AppValues::cleanupInstance();
		}

		//==============================================================================
		void systemRequestedQuit() override
		{
			quit();
		}

		void anotherInstanceStarted(const String& commandLine) override
		{
			quit();
		}
		//The main application that creates the SamplifyMainComponent
		class MainWindow : public DocumentWindow
		{
		public:
			MainWindow(String name) : DocumentWindow(name,
				Desktop::getInstance()
				.getDefaultLookAndFeel()
				.findColour(ResizableWindow::backgroundColourId),
				DocumentWindow::allButtons)
			{
				setUsingNativeTitleBar(true);
				SamplifyMainComponent* smc = new SamplifyMainComponent();
				setContentOwned(smc, true);
				setResizable(true, true);
				centreWithSize(getWidth(), getHeight());
				setVisible(true);

				SamplifyMainComponent::setupLookAndFeel(mLookAndFeel);
				setLookAndFeel(&mLookAndFeel);
#if JUCE_MAC || DOXYGEN
                MenuBarModel::setMacMainMenu(&mMainMenuModel);
#endif
#if JUCE_WINDOWS || JUCE_WIN32
				setMenuBar(&mMainMenuModel);
				mMainMenuModel.setLookAndFeel(&mLookAndFeel);
#endif
					
			}
			~MainWindow()
			{
#if JUCE_MAC || DOXYGEN
                                MenuBarModel::setMacMainMenu(nullptr);
#endif
#if JUCE_WINDOWS || JUCE_WIN32
                                setMenuBar(nullptr);
                                mMainMenuModel.setLookAndFeel(nullptr);
#endif
				mMainMenuModel.setLookAndFeel(nullptr);
				getContentComponent()->setLookAndFeel(nullptr);
				setLookAndFeel(nullptr);
				clearContentComponent();
			}

			void closeButtonPressed() override
			{
				JUCEApplication::getInstance()->systemRequestedQuit(); //close app
			}

			/* Note: Be careful if you override any DocumentWindow methods - the base
			   class uses a lot of them, so by overriding you might break its functionality.
			   It's best to do all your work in your content component instead, but if
			   you really have to override any DocumentWindow methods, make sure your
			   subclass also calls the superclass's method.
			*/

		private:
			LookAndFeel_VJake mLookAndFeel;
			SamplifyMainMenu mMainMenuModel;
			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
		};

	private:
		std::unique_ptr<MainWindow> mainWindow;
	};

}


//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (samplify::SamplifyPlusApplication)

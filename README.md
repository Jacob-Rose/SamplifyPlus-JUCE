# SamplifyPlus-JUCE

My first attempt with using the JUCE library. 

A port of my Samplify Project created in WPF using C#. I wanted more control over when each sample box draws and when each samples waveform is calculated. 

**PLEASE NOTE:**

I made this project after my first year at Champlain College. The architecture is not as clear as I want it to be.


**NOTE IF USE:**

Project creates string to sample library path in registry, just an FYI.
Project creates a folder in directory to save all file waveforms to. Just delete folder, all created files are in there.


Goal of project was to create a new sample browser for music producers.

Tha main mechanic that I wanted from a sample browser was a waveform view and a tagging system. The waveform view is working great, with the ability to custom draw the waveform in any way desired, but the loading of this many waveforms slows down the system greatly, especially with large sample libraries.

This project is also before I commented anything well if at all, bad habits die hard.

I needed to hard code in the sample library location and never added the ability to change it once its set. Saves to system registry,

I have included all the JUCE library files I used, I do not own these files and all rights for those files fall under their specified license.

Old project, need to work on more :ok_hand:

## Samplify Plus
### JUCE Library

My first attempt with using the JUCE library. 

A port of my Samplify Project created in WPF using C#. I wanted more control over when each sample box draws and when each samples waveform is calculated. 

There is a memory leak on program quit, need to fix at some point.

## Features
![SamplifyImage1](https://i.imgur.com/GoQHlHo.png)
- Sample Folder Selection
- Directory View of Folders
- Waveform View of Files
- Drag and Drop ability



**NOTE IF USE:**

Project creates string to sample library path in registry, just an FYI.
Project currently creates a copy of each file with a .samp extension. This will hopefully be changed to a single folder that can be deleted, but currently there is an included .bat file to delete all sample files. Put this file in your sample directory and run.


**Goal of project was to create a new sample browser for music producers.* *

Tha main mechanic that I wanted from a sample browser was a waveform view and a tagging system. The waveform view is working great, with the ability to custom draw the waveform in any way desired, but the loading of this many waveforms slows down the system greatly, especially with large sample libraries.



I needed to hard code in the sample library location and never added the ability to change it once its set. Saves to system registry.

Old project, need to work on more :ok_hand:

**PLEASE NOTE:**

I made this project after my first year at Champlain College. The architecture is not as clear as I want it to be.
This project is also before I commented anything well if at all, bad habits die hard. Will hopefully add some at some point.



I have included all the JUCE library files I used, I do not own these files and all rights for those files fall under their specified license.

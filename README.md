## Samplify Plus
![SamplifyImage1](https://i.imgur.com/TEacSr6.png)
[^1]:
*concept image*

A File Manager based around music production and samples.

Music production has hundreds of great tools to allow creators to make new and amazing things, but searching for the right sounding sample when creators have libraries of thousands of files is very difficult in modern softwares like Ableton Live and FL Studio.

File organization is not the only solution for these projects, but properly organizing and labeling samples.
Tags allow users to label each sample with certain traits that they can define. Label a sample with the tags *kick*, *short*, *trigger* and search for multiple tags at once to find the sample for the situation.

Optimized file loading prevents crashes, only loads supported files, and after a first launch it takes only seconds to start up.

Easy drag and drop from Samplify into your preferred DAW, similar to dragging it in from any OS File Explorer.

[Samplify Site](https://www.samplify.app)

### JUCE Library

My first attempt with using the JUCE library.

The library offers many tools that make it easier than ever to create an audio application.

Juce will allow an easy port to a VST, simplier cryptography and copy protection as well as online authorization. Also allows cross platform support that is required as music production device market share is split between Windows and Mac.

## Features
![SamplifyImage1](https://i.imgur.com/GoQHlHo.png)
> The version with images published is very out of date, massive rebuild has been completed and more features are being added.
- Directory View of Folders
- Waveform View of Files
- Drag and Drop ability
- Tag system
- File sticky notes, easy ways to save a file for later and any ideas had with the file

![SamplifyImage1](https://i.imgur.com/LbgovDd.png)

### Future Plans

- TBA (right now the issue is more of protecting my IP, so features will be announced on release


## Learning Outcomes

- Learned how to read waveforms and draw custom waveforms based off their data
- Learned how to manage memory effectively to reduce UI hanging
- Learned to use JUCE library
- Gained skill in C++ and standard library features included in it


__Goal of project was to create a new sample browser for music producers.__

Tha main mechanic that I wanted from a sample browser was a waveform view and a tagging system. The waveform view is working great, with the ability to custom draw the waveform in any way desired, but the loading of this many waveforms slows down the system greatly, especially with large sample libraries.



Current passion project, working on as I can. Hopefully beta release with online authorization soon! :ok_hand:


### Legal

I have included all the JUCE library files I used, I do not own these files and all rights for those files fall under their specified license.

I have included a FontAwesome module for JUCE, I do not own these files and all rights for those files fall under their specified license.

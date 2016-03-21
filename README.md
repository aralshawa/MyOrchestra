# MyOrchestra
A [Waterloo WearHacks](http://waterloo.wearhacks.com) 36-hour hackathon project repository.

*Project:* MyOrchestra is an orchestral music experience. Become a conductor and direct your own orchestra using a Myo armband and intuitive gesture controls.

Details are available on the [DevPost project page](http://devpost.com/software/myorchestra-yxl37m).

## Sub-Projects

1. **MyOrchestra-Hub** - Mac OS X user application using the [Myo](https://www.myo.com) API
2. **server** - The [Twilio](https://www.twilio.com) enabled server-side source code

## Inspiration
Make an intuitive and simple music mixing platform that lets the audience and artist interact in new ways. 

## Overview of Functionality 
This system allows the user to conduct their own virtual orchestra composed of their own "orchestral" sections. Though using spacial gestures, properties of each section can be individually manipulated to quickly and uniquely prototype/perform compositions.

Moreover, it allows the audience to contribute to the performance along with the artist using crowd sourced audio. The audience can dial into the show and leave sound bytes for the artist to use. The artist can mix the sounds in real time during the show.  

## Technical Overview
We used the accelerometer and gyroscope positioning system of the Myo to setup, select, and manipulate tracks. We used the Twilio platform to source audio from the audience. The artist can access the crowd sourced audio through HTTP hooks to our web server and use them alongside pre-recorded audio tracks.

## Challenges
- Dealing with a multiple Myo setup
- Determining the spacial boundaries for audio control
- Sourcing the audio from the crowd via the Twilio platform
- Drinking Solent during the hackathon
- Integrating the various systems together
- Selecting and debugging various hardware platforms (e.x. MbientLab MetaWare, Thalamic Labs Myo, Leap Motion) during the design stage

## Accomplishments
- Manipulating different parts of the audio file via gestures
- Calibrating and configuring the spacial regions for operation
- Navigating the Twilio documentation
- Using the Twilio Platform to record and provide user audio clips

## What was Learned
- Using the Myo SDK
- Myo hardware bugs

## Future Work
- Make the crowd sourcing system real-time
- Putting all the controls on one Myo
- Application to add/configure tracks and effects
- Achieving more audio channels and effects
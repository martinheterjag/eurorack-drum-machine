/*
  ==============================================================================

    DrumMachine.cpp
    Created: 29 Feb 2020 5:39:07pm
    Author:  me

  ==============================================================================
*/

#include "DrumMachine.h"

#include <iostream>
#include <pigpio.h>

#include "../JuceLibraryCode/JuceHeader.h"

const char* KICK = "/home/pi/sampleplayer/samples/Kick.wav";
const char* SNARE = "/home/pi/sampleplayer/samples/Snare.wav";
const char* CLAP = "/home/pi/sampleplayer/samples/Clap.wav";
const char* CLOSED_HAT = "/home/pi/sampleplayer/samples/HatOpen.wav";
const char* OPEN_HAT = "/home/pi/sampleplayer/samples/HatClosed.wav";
const char* CYMBAL = "/home/pi/sampleplayer/samples/Ride.wav";

const int IO_LED = 26;
const int IO_KICK = 19;
const int IO_SNARE = 6;
const int IO_CLAP = 5;
const int IO_CLOSED_HAT = 22;
const int IO_OPEN_HAT = 25;
const int IO_CYMBAL = 27;

DrumMachine::DrumMachine() 
    : kick_player_(new AudioSourcePlayer()),
      snare_player_(new AudioSourcePlayer()),
      clap_player_(new AudioSourcePlayer()),
      closed_hat_player_(new AudioSourcePlayer()),
      open_hat_player_(new AudioSourcePlayer()),
      cymbal_player_(new AudioSourcePlayer()) {
  std::cout << "+++ INIT AUDIO +++" << std::endl;
  // AudioFormatManager fmgr_;
  std::cout << "init format manager" << std::endl;
  fmgr_.registerBasicFormats();

  std::cout << "init all sounds" << std::endl;
  // init all drum sounds
  File kick(KICK);
  kick_ = new AudioFormatReaderSource(fmgr_.createReaderFor(kick), true);
  kick_player_->setSource (kick_);

  File snare(SNARE);
  snare_ = new AudioFormatReaderSource(fmgr_.createReaderFor(snare), true);
  snare_player_->setSource (snare_);

  File clap(CLAP);
  clap_ = new AudioFormatReaderSource(fmgr_.createReaderFor(clap), true);
  clap_player_->setSource (clap_);

  File c_hat(CLOSED_HAT);
  closed_hat_ = new AudioFormatReaderSource(fmgr_.createReaderFor(c_hat), true);
  closed_hat_player_->setSource (closed_hat_);

  File o_hat(OPEN_HAT);
  open_hat_ = new AudioFormatReaderSource(fmgr_.createReaderFor(o_hat), true);
  open_hat_player_->setSource (open_hat_);

  File cymbal(CYMBAL);
  cymbal_ = new AudioFormatReaderSource(fmgr_.createReaderFor(cymbal), true);
  cymbal_player_->setSource (cymbal_);

  std::cout << "init audio device" << std::endl;
  devmgr_.initialiseWithDefaultDevices (0, 2);
  device_ = devmgr_.getCurrentAudioDevice();


  std::cout << "+++ INIT GPIO +++" << std::endl;
  if (gpioInitialise() < 0) {
    fprintf(stderr, "pigpio initialisation failed\n");
    exit(1);
  }

  gpioSetMode(IO_LED, PI_OUTPUT);
  gpioSetMode(IO_KICK, PI_INPUT);
  gpioSetMode(IO_SNARE, PI_INPUT);
  gpioSetMode(IO_CLAP, PI_INPUT);
  gpioSetMode(IO_CLOSED_HAT, PI_INPUT);
  gpioSetMode(IO_OPEN_HAT, PI_INPUT);
  gpioSetMode(IO_CYMBAL, PI_INPUT);

  if (device_) {
    gpioSetAlertFuncEx(IO_KICK, playKick, this);
    gpioSetAlertFuncEx(IO_SNARE, playSnare, this);
    gpioSetAlertFuncEx(IO_CLAP, playClap, this);
    gpioSetAlertFuncEx(IO_CLOSED_HAT, playClosedHat, this);
    gpioSetAlertFuncEx(IO_OPEN_HAT, playOpenHat, this);
    gpioSetAlertFuncEx(IO_CYMBAL, playCymbal, this);
  }

  
  std::cout << "Init done -> LED on" << std::endl;

  while(true) {
    // wait for events
    Thread::sleep(10);
    gpioWrite(IO_LED, gpioRead(IO_KICK) |
                      gpioRead(IO_SNARE) |
                      gpioRead(IO_CLAP) |
                      gpioRead(IO_CLOSED_HAT) |
                      gpioRead(IO_OPEN_HAT) |
                      gpioRead(IO_CYMBAL));
  }
}

DrumMachine::~DrumMachine() {
  gpioWrite(IO_LED, 0);
  gpioTerminate();
}

void playKick(int gpio, int level, uint32_t tick, void *userdata) {
  if(level == 0) {
    return;
  }
  DrumMachine* dm = static_cast<DrumMachine*>(userdata);
  dm->kick_->setNextReadPosition(0);
  dm->kick_->prepareToPlay(dm->device_->getDefaultBufferSize(),
                       dm->device_->getCurrentSampleRate());
  dm->devmgr_.addAudioCallback (dm->kick_player_);
}

void playSnare(int gpio, int level, uint32_t tick, void *userdata) {
  if(level == 0) {
    return;
  }
  DrumMachine* dm = static_cast<DrumMachine*>(userdata);
  dm->snare_->setNextReadPosition(0);
  dm->snare_->prepareToPlay(dm->device_->getDefaultBufferSize(),
                        dm->device_->getCurrentSampleRate());
  dm->devmgr_.addAudioCallback (dm->snare_player_);
}

void playClap(int gpio, int level, uint32_t tick, void *userdata) {
  if(level == 0) {
    return;
  }
  DrumMachine* dm = static_cast<DrumMachine*>(userdata);
  dm->clap_->setNextReadPosition(0);
  dm->clap_->prepareToPlay(dm->device_->getDefaultBufferSize(),
                       dm->device_->getCurrentSampleRate());
  dm->devmgr_.addAudioCallback (dm->clap_player_);
}

void playClosedHat(int gpio, int level, uint32_t tick, void *userdata) {
  if(level == 0) {
    return;
  }
  DrumMachine* dm = static_cast<DrumMachine*>(userdata);
  // TODO: Make open hat quiet when playing the closed hat
  // Did not work: open_hat_->setNextReadPosition(open_hat_->getTotalLength());
  dm->closed_hat_->setNextReadPosition(0);
  dm->closed_hat_->prepareToPlay(dm->device_->getDefaultBufferSize(),
                             dm->device_->getCurrentSampleRate());
  dm->devmgr_.addAudioCallback (dm->closed_hat_player_);
}

void playOpenHat(int gpio, int level, uint32_t tick, void *userdata) {
  if(level == 0) {
    return;
  }
  DrumMachine* dm = static_cast<DrumMachine*>(userdata);
  dm->open_hat_->setNextReadPosition(0);
  dm->open_hat_->prepareToPlay(dm->device_->getDefaultBufferSize(),
                           dm->device_->getCurrentSampleRate());
  dm->devmgr_.addAudioCallback (dm->open_hat_player_);
}

void playCymbal(int gpio, int level, uint32_t tick, void *userdata) {
  if(level == 0) {
    return;
  }
  DrumMachine* dm = static_cast<DrumMachine*>(userdata);
  dm->cymbal_->setNextReadPosition(0);
  dm->cymbal_->prepareToPlay(dm->device_->getDefaultBufferSize(),
                         dm->device_->getCurrentSampleRate());
  dm->devmgr_.addAudioCallback (dm->cymbal_player_);
}

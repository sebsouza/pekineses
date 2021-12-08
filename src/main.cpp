#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <SamdAudioSD.h>

//-----------------------------------Global Variables-----------------------------------
#define NUM_AUDIO_CHANNELS 1  // Set to either 1, 2, or 4 for simultaneous playback
#define AUDIO_BUFFER_SIZE 512 // 512 works fine for 22.05kh, 32khz, and 44.1khz (not recommeneded to use any other size)
#define YOUR_SD_CS 28         // SD chip select pin
#define SHUTDOWN_PIN 7        // shutdown pin for Class D amplifiers such as the PAM8302 (defaults to pin 7 in library if no explicitly set in setup)
#define DAC_PIN A0            // audio output pin on Arduino board (defaults to pin A0 in library if no explicitly set in setup)
#define DIGITAL_POT 5         // digital potentiometer pin to control amplifier hardware gain (defaults to pin 5 in library if no explicitly set in setup)

#define PIR_SensorPin 14

#define NUM_AUDIOS 2

bool sensorPIR = false;
bool playing = false;
int audioId = 0;
String baseAudioFile = "audio";
String audioFile = "";

SamdAudioSD AudioPlayer;
// const char *filename = "audio0.wav"; // your wav file, be cautious of the 8.3 filename format and place in root of your sd card
uint32_t sampleRate = 44100; // indicate sample rate here (use audacity to convert your wav)

void setup()
{
  pinMode(PIR_SensorPin, INPUT);

  Serial.begin(9600);

  Serial.println("Initializing SD card...");

  if (!SD.begin(YOUR_SD_CS))
  { // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return; // don't do anything more if not
  }
  Serial.println(" done.");

  AudioPlayer.selectShutdownPin(SHUTDOWN_PIN);                          // used to set shutdown pin (SD) on amplifier if desired and set pinmode to output, if set .play() will power the amplifier up and down as needed
  AudioPlayer.selectDACPin(DAC_PIN);                                    // used to set audio output pin on Arduino (defaults to pin A0)
  AudioPlayer.selectDigitalPotPin(DIGITAL_POT);                         // used to set the digital potentiometer pin on the arduino in order to control amplifier volume (note, if using other SPI devices all pins will have to be set high first)
  SPI.setClockDivider(12);                                              // brings the SPI clock back to default, needed when using a digital potentiometer due to poor SPI usage in the MCP4XXX library
  AudioPlayer.begin(sampleRate, NUM_AUDIO_CHANNELS, AUDIO_BUFFER_SIZE); // required inputs: sample rate, number of audio channels possible, size of audio buffer for processing
}

void loop()
{
  sensorPIR = digitalRead(PIR_SensorPin);

  if (sensorPIR && !AudioPlayer.isPlaying(0))
  // Movement detected and NOT playing
  {
    audioFile = baseAudioFile + (String)audioId + ".wav";
    audioId < NUM_AUDIOS - 1 ? audioId++ : audioId = 0;

    AudioPlayer.play(audioFile.c_str());

    Serial.print("Playing on ");
    Serial.println(audioFile.c_str());
  }
}
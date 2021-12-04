#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>

#define SD_ChipSelectPin 28
#define PIR_SensorPin 14

#define AUDIO_QUANTITY 1

bool playing = false;
int audioId = 0;
String baseAudioFile = "audio";
String audioFile = "";
String voidAudio = "void.wav";

void setup()
{
  Serial.println("Initializing...");
  pinMode(PIR_SensorPin, INPUT);

  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin))
  { // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return; // don't do anything more if not
  }
  // SPI.setClockDivider(4);
  // AudioZero.begin(44100);
}

void loop()
{
  yield();
  // Serial.println(digitalRead(PIR_SensorPin));

  if (digitalRead(PIR_SensorPin))
  {
    audioFile = baseAudioFile + (String)audioId + ".wav";
    audioId < AUDIO_QUANTITY - 1 ? audioId++ : audioId = 0;

    File myFile = SD.open(audioFile.c_str());

    AudioZero.begin(44100);
    // Serial.print(" Playing silence...");
    // AudioZero.play(voidFile);
    Serial.print(" Playing file ");
    Serial.println(audioFile.c_str());
    AudioZero.play(myFile);
    AudioZero.end();
    // Serial.print(" Playing silence...");
    // AudioZero.play(voidFile);
    // Serial.print(" End of playing");

    // playing = false;
  }
}
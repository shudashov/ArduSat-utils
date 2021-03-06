/*
********************************************************************

Copyright 2014, Jean-François Omhover (jf.omhover@gmail.com) and Nanosatisfi, Inc. (www.nanosatisfi.com)

********************************************************************
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
********************************************************************

Description : prompts for a filename, gets to the SD to read that file
and decode it according to the format generated by SAT_DataLib

see library at https://github.com/jfomhover/ArduSat-utils/tree/master/libraries/SAT_DataLib

********************************************************************
*/

#include <SD.h>
#include <SAT_DataLib.h>
#include "promptSD.h"

// NOTE : configuration of SD card / file
#define CS_PIN      4    // 4 for Arduino Ethernet
#define BUFFER_LENGTH  64

byte * buffer;   // buffer for parsing the file (maximum size of chunks)
int bufferLen = 0;

// use the syntax beloc to declare the datatypes you'll need
DataDecoder decoder;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  buffer = NULL;
  bufferLen = BUFFER_LENGTH;
  while(buffer == NULL) {
    buffer = (byte*)malloc(bufferLen);
    if (buffer == NULL)
      bufferLen = bufferLen / 2;
    if (bufferLen < 16) {
      Serial.println("not enough memory");
      while(1);
    }
  };
  Serial.print("allocated ");
  Serial.println(bufferLen);
}


void loop() {
  // put your main code here, to run repeatedly: 

  // prompting for a filename
  char * t_filename = promptFile();
  Serial.print("opening ");
  Serial.println(t_filename);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(t_filename);

  if (dataFile.isDirectory()) {
    Serial.print("!!! can't open directories - "); Serial.println(t_filename);
    dataFile.close();
    return;
  }    
  // if the file is available, write to it:
  if (dataFile) {
    decoder.setSeparation(';');
    decoder.parseFile(dataFile, buffer, bufferLen);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.print("!!! error opening "); Serial.println(t_filename);
  } 
}


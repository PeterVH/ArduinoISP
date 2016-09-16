/*
  Melody
 
 Plays a melody 
 
 circuit:
 * 8-ohm speaker on digital pin 1_3
 
 by Tom Igoe 
 modified for 8051 by Peter Van Hoyweghen
 

This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/Tone
 
 */

#include <stdio.h>
#include "hw.h"
#include "tone.h"
#include "delay.h"

void ClockIrqHandler (void) __interrupt(1);

#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3, 0/* div by zero? */, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };

void main(void) 
{
  while (1) {
    int thisNote;
 
    // iterate over the notes of the melody:
    for (thisNote = 0; thisNote < 8; thisNote++) {
      int pauseBetweenNotes ;
      // to calculate the note duration, take one second 
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000/noteDurations[thisNote];
      tone(melody[thisNote],noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      pauseBetweenNotes = noteDuration * 130 / 100;
      delay_ms(pauseBetweenNotes);
      // stop the tone playing:
      // noTone(8);
    }
    delay_ms(2000);
  }
}


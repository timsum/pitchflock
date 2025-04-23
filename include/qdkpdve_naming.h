//
//  qdkpdve_naming.h
//  pitchflock
//
//  Created by Tim Summers on 6/12/23.
//

#ifndef QDKPDVE_NAMING_H
#define QDKPDVE_NAMING_H

#define CANONICSTARTPOINT 11

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "qdkpdve.h"

extern char* noteStrings[];
extern char* basePatternConventionalNames[];
extern char* basePatternConventionalNamesShort[];
extern char* modeNamesMatrix[];
extern char* patternDistortionsMatrix[];
extern char* modeNumbersMatrix[];
extern char* voicingNamesMatrix[];
extern char* extensionNamesMatrix[];

int canonicalLydianStartPtForCircleIndex(int index);
int canonicalMajorStartPtForCircleIndex(int index);
int conventionalDegreeDisplacementForKPDVE(int patnum);

int conventionalDegreeForKPDVE(int kpdve);
int conventionalTonicForKPDVEpattern(int kpdve);

int circleIndexForBitLocation(int aByte);

int stringIndexForFifthsLocationInKey(int loc, int key);

int nameIndexForKPDVE(int kpdve);

const char* nameStringForKPDVE(int kpdve);
const char* nameStringAtIndex(int index);
double noteRatioAtIndex(int index);
const char* lydBaseForKPDVE(int kpdve);
const char* majBaseForKPDVE(int kpdve);
const char* rootStringForKPDVE(int kpdve);
const char* conventionalTonicStringForKPDVE(int kpdve);
const char* conventionalDegreeStringForKPDVE(int kpdve);

double freqRatioForKPDVE(int kpdve);

// fill in the string buffer with the chord notes for a given KPDVE
void chordNotesStringForKPDVE(int kpdve, char*stringBuf, size_t bufSize);

// these will give a mode starting at the root of the chord (d)
void modeNotesStringForKPDVE(int kpdve, char*stringBuf, size_t bufSize); // in the order of the voicing
void modeNotesScaleStringForKPDVE(int kpdve, char* stringBuf, size_t bufSize); // in the order of the scale
void modeNotesFifthsStringForKPDVE(int kpdve, char* stringBuf, size_t bufSize); // in the order of fifths

// Conventional names for consistency in the pattern naming. 
const char* modeNameForKPDVE(int kpdve);
const char* patternDistortionForKPDVE(int kpdve);
const char* scaleNameForKPDVE(int kpdve);

// utility for printing binary values
const char* kpdve_as_string(int kpdve);


#endif // QDKPDVE_NAMING_H

//
//  qdkpdve_naming.c
//  pitchflock
//
//  Created by Tim Summers on 6/12/23.
//

#include "qdkpdve_naming.h"

// Three rounds of modular rotation -- could be a dynamic function in the future...
char* noteStrings[] = {
    "FB", "CB", "GB", "DB", "AB", "EB", "BB",
    "Fb", "Cb", "Gb", "Db", "Ab", "Eb", "Bb",
    "F ", "C ", "G ", "D ", "A ", "E ", "B ",
    "F#", "C#", "G#", "D#", "A#", "E#", "B#",
    "Fx", "Cx", "Gx", "Dx", "Ax", "Ex", "Bx"
};

// Three rounds of corresponding ratios in pure 3/2, octave reduced -- could also be a dynamic function in the future... correspondingly.
// basically, the above is a notation for this...
double noteRatios[] = { 1.1692330293868376, 1.7538495440802564, 1.3153871580601924, 1.9730807370902885, 1.4798105528177163, 1.1098579146132872, 1.6647868719199308,
    1.2485901539399482, 1.8728852309099222, 1.4046639231824416, 1.0534979423868314, 1.5802469135802468, 1.1851851851851851, 1.7777777777777777,
    1.3333333333333333, 1.0, 1.5, 1.125, 1.6875, 1.265625, 1.8984375, 1.423828125,
    1.06787109375, 1.601806640625, 1.20135498046875, 1.802032470703125, 1.3515243530273438, 1.0136432647705078,
    1.5204648971557617, 1.1403486728668213, 1.710523009300232, 1.282892256975174, 1.924338385462761, 1.4432537890970707, 1.082440341822803};

// Conventional names of patterns 1 bit removed from Lydian (0)
char* basePatternConventionalNames[] = {
    "Tonic Major      ",
    "Dominant Major   ",
    "Melodic Minor    ",
    "Harmonic Minor   ",
    "Harmonic Major   ",
    "Parallel Minor   ",
    "Subdominant Major"
};

// Short names of patterns 1 bit removed from Lydian (0)
char* basePatternConventionalNamesShort[] = {
    "M (tonic)",
    "M (dom)  ",
    "m/mel    ",
    "m/harm   ",
    "M/harm   ",
    "m/para   ",
    "M (subd) "
};

// Conventional mode names of patterns starting from different degrees of Lydian scale
char* modeNamesMatrix[] = {
    "Lydian    ",
    "Ionian    ",
    "Mixolydian",
    "Dorian    ",
    "Aeolian   ",
    "Phrygian  ",
    "Locrian   "
};

// Alterations of the Lydian scale in the base patterns
char* patternDistortionsMatrix[] = {
    "", // Major
    "#1", // Dominant
    "#5", // Melodic
    "#2", // Harmonic
    "b3", // Harmonic Major
    "b7", // Parallel Minor
    "b4" // subdominant Major
};

// Chord degrees within a pattern (could later be dynamically capitalized for Chord quality)
char* modeNumbersMatrix[] = {
    "IV ",
    "I  ",
    "V  ",
    "II ",
    "VI ",
    "III",
    "VII"
};

// Voicing names for the 0-2 (shuffle) voicing scheme
char* voicingNamesMatrix[] = {
    "5ths",
    "3rds",
    "2nds",
};

// Extension names, just for the textual representation
char* extensionNamesMatrix[] = {
    "One",
    "Two",
    "Three",
    "Four",
    "Five",
    "Six",
    "Seven"
};

/**
 * @brief Canonical start point for the Lydian scale (degree 0) at a given key.
 * 
 * @param index The circle index.
 * @return int The canonical start point.
 */
int canonicalLydianStartPtForCircleIndex(int index)
{
    return CANONICSTARTPOINT + index + 3;
}

/**
 * @brief Canonical start point for the Major scale (degree 1) at a given key.
 * 
 * @param index The circle index.
 * @return int The canonical start point.
 */
int canonicalMajorStartPtForCircleIndex(int index)
{
    return CANONICSTARTPOINT + (index + 4) % 12;
}

/**
 * @brief Canonical start point for a given pattern.
 * 
 * Examples: C Major, G Major, D Minor, A Harmonic, etc.
 * 
 * @param patnum The pattern number.
 * @return int The degree displacement.
 */
int conventionalDegreeDisplacementForKPDVE(int patnum)
{
    return (patnum < 4 || patnum > 5) ? patnum : 0;
}

/**
 * @brief Get the conventional role of a chord in its pattern.
 * 
 * @param kpdve The KPDVE value.
 * @return int The conventional degree.
 */
int conventionalDegreeForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int patnum = result[1];
    int degnum = result[2];
    degnum += (conventionalDegreeDisplacementForKPDVE(patnum) * 6) % 7;
    return degnum % 7;
}

/**
 * @brief Get the conventional tonic of a pattern.
 * 
 * @param kpdve The KPDVE value.
 * @return int The conventional tonic degree.
 */
int conventionalTonicForKPDVEpattern(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int patnum = result[1];
    int degnum = 1;
    degnum += conventionalDegreeDisplacementForKPDVE(patnum);
    //printf(" * displacement: %d * ", conventionalDegreeDisplacementForKPDVE(patnum));
    return degnum % 7;
}

/**
 * @brief Extract location of a bit in a byte.
 * 
 * Examples:
 * - 0b00000001 = 0
 * - 0b00000010 = 1
 * - 0b00000100 = 2
 * 
 * @param aByte The byte value.
 * @return int The bit location.
 */
int circleIndexForBitLocation(int aByte)
{
    int v = aByte;
    int counter = 0;

    while (v > 0)
    {
         counter++;
         v >>= 1;
    }
    return counter - 1;
}

/**
 * @brief Get the location of the name of a note in a given key.
 * 
 * @param loc The location in the circle of fifths.
 * @param key The key index.
 * @return int The string index.
 */
int stringIndexForFifthsLocationInKey(int loc, int key)
{
    int flatsideKey = (key > 7) ? -12 : 0;
    return canonicalLydianStartPtForCircleIndex(key) + flatsideKey + loc;
}

/**
 * @brief Gets the location of a note in a given key for use in the note name array.
 * 
 * @param kpdve The KPDVE value.
 * @return int The name index.
 */
int nameIndexForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int key = result[0];
    int pat = result[1];
    int deg = result[2];
    int voice_leap = result[3]; // (int)pow(2, (3 - kpdve[3]) % 3); // IMPORTANT ! IN 0-2 (SHUFFLE) VOICING SCHEME, THIS MUST BE TAKEN CARE OF...
    int ext = result[4];

    int flag = (deg + (voice_leap * ext)) % 7;
    int flat_displace = 0;

    if (pat > 0)
    {
        if ((flag == (pat - 1)) && (pat < 4))
        {
            flat_displace = 7;
        }
        else if ((flag == pat) && (pat >= 4))
        {
            flat_displace = -7;
        }
    }
    flat_displace += flag;
    return stringIndexForFifthsLocationInKey(flat_displace, key);
}

/**
 * @brief Returns the name of the note at the end of the KPDVE encoding (the last extension).
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The note name.
 */
const char* nameStringForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    return noteStrings[nameIndexForKPDVE(kpdve)];
}

/**
 * @brief Returns the frequency ratio (from a C with a value of 1.0)  of the note at the end of the KPDVE encoding (the last extension).
 *
 * @param kpdve The KPDVE value.
 * @return double A calculated ratio from 1.0 up to (not including)  2.0
 */
double freqRatioForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    return noteRatios[nameIndexForKPDVE(kpdve)];
}

// FIXME: This function is completely redundant, but might be further implemented (for array access safety, especially)
/**
 * @brief Returns the name of a note at a given index.
 * 
 * @param index The index in the note array.
 * @return const char* The note name.
 */
const char* nameStringAtIndex(int index)
{
    return noteStrings[index];
}

// FIXME: This function is completely redundant, but might be further implemented (for array access safety, especially)
/**
 * @brief Returns the name of a note at a given index.
 *
 * @param index The index in the note array.
 * @return double The note name.
 */
double noteRatioAtIndex(int index)
{
    double result = noteRatios[index];
    return result;
}

/**
 * @brief Finds the base note for the fundamental lydian orientation.
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The base note name.
 */
const char* lydBaseForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int baseLoc[] = { result[0], 0, 0, 0, 0 };
    int enc_baseLoc = KPDVEtoBinaryEncoding(baseLoc);

    return nameStringForKPDVE(enc_baseLoc);
}

/**
 * @brief Finds the name of the tonic of the major scale for the KPDVE.
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The major tonic name.
 */
const char* majBaseForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int baseLoc[] = { result[0], 0, 1, 0, 0 };
    int enc_baseLoc = KPDVEtoBinaryEncoding(baseLoc);

    return nameStringForKPDVE(enc_baseLoc);
}

/**
 * @brief Finds the name of the root of a chord for the KPDVE. (The first note in the chord, where E=0)
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The root note name.
 */
const char* rootStringForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int rootLoc[] = { result[0], result[1], result[2], 0, 0 };
    int enc_rootLoc = KPDVEtoBinaryEncoding(rootLoc);
    
    return nameStringForKPDVE(enc_rootLoc);
}

/**
 * @brief Finds the name of the conventional tonic of a pattern in a key.
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The conventional tonic name.
 */
const char* conventionalTonicStringForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int patLoc[] = { result[0], result[1], 0, 0, 0 };
    int enc_patLoc = KPDVEtoBinaryEncoding(patLoc);
    
    int dVal = conventionalTonicForKPDVEpattern(enc_patLoc); // get the convention for the pattern...
    int tonicLoc[] = { result[0], result[1], dVal, 0, 0 }; // use it as an example...
    int enc_tonicLoc = KPDVEtoBinaryEncoding(tonicLoc);

    
    return nameStringForKPDVE(enc_tonicLoc);
}

/**
 * @brief Finds the Roman numeral name of a chord in a pattern.
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The Roman numeral name.
 */
const char* conventionalDegreeStringForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int dVal = conventionalDegreeForKPDVE(kpdve); // get the convention for the pattern...
    return modeNumbersMatrix[dVal];
}

/**
 * @brief Builds a string of chord notes for a given KPDVE.
 * 
 * @param kpdve The KPDVE value.
 * @param stringBuf The buffer to store the resulting string.
 * @param bufSize The size of the provided buffer.
 */
void chordNotesStringForKPDVE(int kpdve, char* stringBuf, size_t bufSize) {
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);

    // set the extensions to 0, prepare to iterate through
    int noteLoc[] = { result[0], result[1], result[2], result[3], 0 };
    // set a variable for encoding the note location to get the name string
    int enc_noteLoc;

    // Start with an empty string
    stringBuf[0] = '\0';

    for (int i = 0; i <= result[4]; i++) {
        noteLoc[4] = i;
        enc_noteLoc = KPDVEtoBinaryEncoding(noteLoc);

        // Get the name of the note
        const char* noteName = nameStringForKPDVE(enc_noteLoc);

        // Append the note name to the buffer
        if (strlen(stringBuf) + strlen(noteName) + 2 < bufSize) { // +2 for space and null terminator
            strcat(stringBuf, noteName);
            strcat(stringBuf, " ");
        } else {
            // Buffer is full, truncate the string
            strncat(stringBuf, "...", bufSize - strlen(stringBuf) - 1);
            break;
        }
    }
}

/**
 * @brief Returns the mode notes for a given KPDVE as a string.
 * 
 * @param kpdve The KPDVE value.
 * @param stringBuf The buffer to store the resulting string.
 * @param bufSize The size of the provided buffer.
 */
void modeNotesStringForKPDVE(int kpdve, char* stringBuf, size_t bufSize)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int noteLoc[] = { result[0], result[1], result[2], result[3], 0 };
    int enc_noteLoc;

    const char* noteName;   
    // Start with an empty string
    stringBuf[0] = '\0';

    for (int i = 0; i <= 6; i++)
    {
        noteLoc[4] = i;
        enc_noteLoc = KPDVEtoBinaryEncoding(noteLoc);

        noteName = nameStringForKPDVE(enc_noteLoc);
        if (strlen(stringBuf) + strlen(noteName) + 2 < bufSize) { // +2 for space and null terminator
            strcat(stringBuf, noteName);
            strcat(stringBuf, " ");
        } else {
            // Buffer is full, truncate the string
            strncat(stringBuf, "...", bufSize - strlen(stringBuf) - 1);
            break;
        }
    }
}

/**
 * @brief Returns the mode notes for a given KPDVE as a string in stepwise order starting at tonic.
 * 
 * @param kpdve The KPDVE value.
 * @param stringBuf The buffer to store the resulting string.
 * @param bufSize The size of the provided buffer.
 */
void modeNotesScaleStringForKPDVE(int kpdve, char* stringBuf, size_t bufSize)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    // set the voicing to 2 so that the notes are in scale order
    int noteLoc[] = { result[0], result[1], result[2], 2, 0 };
    int enc_noteLoc;

    const char* noteName;
    // Start with an empty string
    stringBuf[0] = '\0';

    for (int i = 0; i <= 6; i++)
    {
        noteLoc[4] = i;
        enc_noteLoc = KPDVEtoBinaryEncoding(noteLoc);

        noteName = nameStringForKPDVE(enc_noteLoc);
        if (strlen(stringBuf) + strlen(noteName) + 2 < bufSize) { // +2 for space and null terminator
            strcat(stringBuf, noteName);
            strcat(stringBuf, " ");
        } else {
            // Buffer is full, truncate the string
            strncat(stringBuf, "...", bufSize - strlen(stringBuf) - 1);
            break;
        }
    }
}

/**
 * @brief Returns the mode notes for a given KPDVE as a string in stepwise order starting at chord root.
 * 
 * @param kpdve The KPDVE value.
 * @param stringBuf The buffer to store the resulting string.
 * @param bufSize The size of the provided buffer.
 */
void modeNotesFifthsStringForKPDVE(int kpdve, char* stringBuf, size_t bufSize)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    int noteLoc[] = { result[0], result[1], 1, 1, 0 };
    int enc_noteLoc;

    const char* noteName;

    // Start with an empty string
     stringBuf[0] = '\0';

    for (int i = 0; i <= 6; i++)
    {
        noteLoc[4] = i;
        enc_noteLoc = KPDVEtoBinaryEncoding(noteLoc);

        noteName = nameStringForKPDVE(enc_noteLoc);
        if (strlen(stringBuf) + strlen(noteName) + 2 < bufSize) { // +2 for space and null terminator
            strcat(stringBuf, noteName);
            strcat(stringBuf, " ");
        } else {
            // Buffer is full, truncate the string
            strncat(stringBuf, "...", bufSize - strlen(stringBuf) - 1);
            break;
        }
    }
}

/**
 * @brief Returns the name of the mode of a given KPDVE, including the pattern distortion.
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The mode name.
 */
const char* modeNameForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);

    return modeNamesMatrix[result[2]]; // Return a pointer to a string literal
}

/**
 * @brief Returns the pattern distortion for a given KPDVE.
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The pattern distortion.
 */
const char* patternDistortionForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);

    return patternDistortionsMatrix[result[1]]; // Return a pointer to a string literal
}

/**
 * @brief Returns the name of the scale of a given KPDVE.
 * 
 * Examples: "Major", "Melodic Minor", "Harmonic Major", etc.
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The scale name.
 */
const char* scaleNameForKPDVE(int kpdve)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);

    return basePatternConventionalNames[result[1]];
}

/**
 * @brief Returns the KPDVE values as a string.
 * 
 * @param kpdve The KPDVE value.
 * @return const char* The string representation of KPDVE.
 */
const char* kpdve_as_string(int kpdve) {
    static char kpdveString[17]; // Static buffer to hold the string representation
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    snprintf(kpdveString, sizeof(kpdveString), "[%2d.%2d.%2d.%2d.%2d]", result[0], result[1], result[2], result[3], result[4]);
    return kpdveString;
}





//
//  qdkpdve.h
//  qdkpdve
//
//  Created by Tim Summers on 6/12/23.
//

#ifndef qdkpdve_h
#define qdkpdve_h

#include <stdio.h>
#include <stdlib.h>

// Define constants for chromaCount and primeDivision
#define CHROMA_COUNT 12
#define PRIME_DIVISION 7

extern int chromaCount;
extern int primeDivision;

extern int FM7LydianKPDVE[];
extern int FM7LydianChroma;

// Function declarations

int kpdve_chromatic_byte(int kpdve, int chromatic);
void kpdve_chromatic_byte_to_kpdve(int encodedByte, int result[]);
//
int KPDVEtoBinaryEncoding(int aKPDVE[]);
void binaryEncodingToKPDVE(int encodedByte, int result[]);
int getKPDVEparameterValue(int kpdve, int index);
//
int chroma_chord_from_kpdve(int kpdve);
int chroma_scale_from_kpdve(int kpdve);
int chroma_root_from_kpdve(int kpdve);
int chroma_ext_from_kpdve(int kpdve);

int circle_chord_from_kpdve(int kpdve);
int circle_scale_from_kpdve(int kpdve);
int circle_root_from_kpdve(int kpdve);
int circle_ext_from_kpdve(int kpdve);

int chroma_circle_hash(int val);
int circle_to_chroma(int val);
int chroma_to_circle(int val);
int bit_bunch(int breadth);
int key_filt(int breadth);
int mod_rot(int val, int rot, int mod);
int loop_mod(int x, int mod);
int v_shift(int v);
int ve_val(int v, int e);
int ve_chord_val(int v, int e);
int dve_val(int d, int v, int e);
int dve_chord_val(int d, int v, int e);
int p_filt(int p_val, int breadth);
int apply_p_filt(int val, int p);
int pdve_val(int p, int d, int v, int e);
int pdve_chord_val(int p, int d, int v, int e);

int kpdve_val(int kpdve);
int kpdve_chord_val(int kpdve);

#endif /* qdkpdve_h */

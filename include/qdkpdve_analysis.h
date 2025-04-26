//
//  qdkpdve_analysis.h
//  pitchflock
//
//  Created by Tim Summers on 6/12/23.
//
//
//

#ifndef qdkpdve_analysis_h
#define qdkpdve_analysis_h

#include <stdio.h>
#include "qdkpdve.h"

struct ve_value
 {
     int bin_val; // seven bits, with bits to be adjusted rightward (lowest possible value)
     int v;
     int e;
 };

struct  dve_value
 { // seven bits, as above, but rotated for d
     int bin_val;
     int d;
     struct ve_value ve_val;
 };

struct kpdve_value
 {
     int bin_val;
     int k;
     int p;
     struct dve_value dve_val;
 };

 // a utility function to find the efficiencency of a ve_value
int largest_bit(int aUInt);

// shuffles bits like a deck of cards
int shuffle_bits(int ve_val, int breadth);

// unshuffles bits by shuffling them further... :)
int unshuffle_bits(int ve_val, int breadth);

// manufacture VE and DVE structs
struct ve_value make_ve(int input_val);
struct dve_value make_dve(int input_val);
struct kpdve_value make_kpdve(int input_val);

struct ve_value minimize_ve_value(struct ve_value ve_val);
struct dve_value minimize_dve_value(struct dve_value dve_val);

#endif /* qdkpdve_analysis_h */

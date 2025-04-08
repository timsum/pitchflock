//
//  qdkpdve_statemaker.h
//  pitchflock
//
//  Created by Tim Summers on 6/12/23.
//


#ifndef qdkpdve_statemaker_h
#define qdkpdve_statemaker_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "harmony_state.h"
#include "qdkpdve.h"
#include "qdkpdve_analysis.h"
#include "qdkpdve_harmonycrystal.h"

#endif /* qdkpdve_statemaker_h */


// KPDVE ENCODING for C
int KPDVEtoBinaryEncoding(int aKPDVE[]);
void binaryEncodingToKPDVE(int encodedByte, int result[]);
// //////////////////////////////////

int undo_kp_for_input_val(int input_val, int k, int p);
float modDistance(int val1, int val2, int mod);
double KPD_distance(int kpdve_1, int kpdve_2);

harmony_state harmony_state_default(void);
harmony_state harmony_state_from_kpdve(int a_kpdve);
harmony_state harmony_state_from_binary(int chroma_val);
harmony_state harmony_state_from_binary_w_context(int chroma_val, int contextkpdve);
harmony_state harmony_state_from_min_encoding(int kpdve_bin_encoding);

// THESE need pointers
void set_kp_list(harmony_state *a_state);
void set_min_index(harmony_state *current_state, int context);
void choose_kpdve_from_context(harmony_state *current_state, int context);

// THESE ARE THE ESSENTIAL ADJUSTMENTS
void adjust_harmony_state_from_min_encoding(harmony_state *a_state, int kpdve_bin_encoding);
void adjust_harmony_state_from_chroma(harmony_state *a_state, int chroma_val);
void adjust_harmony_state_from_chroma_and_context(harmony_state *a_state, int chroma_val, int context);
void adjust_harmony_state_from_chroma_lr_and_context(harmony_state *a_state, int chroma_val, int context);
int reverse_12_bits( int num);
void adjust_harmony_state_from_kpdve(harmony_state *a_state, int a_kpdve);

void encode_and_validate_state(harmony_state *a_state);




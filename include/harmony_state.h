//
//  harmony_state.h
//  pitchflock
//
//  Created by Tim Summers on 6/12/23.
//

#ifndef harmony_state_h
#define harmony_state_h

#include <stdio.h>

/**
 * @struct harmony_state
 * @brief Represents the state of a harmonic structure.
 *
 * This struct contains information about the encoded state, chromatic notes,
 * and KPDVE (Key, Pattern, Degree, Voicing, Extension) values. It also includes
 * lists for possible KPDVE encodings and their associated distances.
 */
struct harmony_state {
    int encoded_state; /**< Encoded binary representation of the state (32 bits). */
    int chromatic_notes; /**< Chromatic notes represented as a 12-bit integer (right to left, as in Hebrew). */
    int kpdve; /**< Encoded KPDVE value (3 bits for each of the first 4 values, 4 bits for the sign). */
    int dve; /**< Encoded Degree-Voicing-Extension (DVE) value. */
    int ve; /**< Encoded Voicing-Extension (VE) value. */
    int kpdve_list_length; /**< Number of valid KPDVE encodings in the list. */
    int kpdve_min_index; /**< Index of the minimum distance KPDVE encoding. */
    int kpdve_list[84]; /**< List of possible KPDVE encodings (maximum size: 84). */
    int dve_list[84]; /**< List of associated DVE values for each KPDVE encoding. */
    int ve_list[84]; /**< List of associated VE values for each KPDVE encoding. */
};
typedef struct harmony_state harmony_state;


#endif /* harmony_state_h */

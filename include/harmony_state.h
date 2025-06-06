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
 * This struct contains information about the encoded state (32-bit, completed state), 
 * chromatic notes (12-bit, encoded right to left as in Hebrew),
 * and KPDVE (Key, Pattern, Degree, Voicing, Extension) values (16-bit). 
 * 
 * the min_index is the closest to the previous analyzed state in KPD (12x7x7) space.
 * 
 * It also includes
 * lists for possible KPDVE encodings and their associated distances.
 */
struct harmony_state {
    int encoded_state; /**< Encoded binary representation of the state (32 bits). x---KKKKPPPDDDVVVEEEb-a-g-fe-d-c */
    int chromatic_notes; /**< Chromatic notes represented as a 12-bit integer (right to left, as in Hebrew). b-a-g-fe-d-c */
    int kpdve; /**< Encoded KPDVE value (4 bits for key; 3 bits for each of the next 4 values, 3 spare, 1 (leftmost) for invalid state). KKKKPPPDDDVVVEEE*/
    int dve; /**< Encoded Degree-Voicing-Extension (DVE) value. reduces chord to seven possible notes, independent of key */
    int ve; /**< Encoded Voicing-Extension (VE) value. reduces chord to least possible value, which defines a 'root' */
    int kpdve_list_length; /**< Number of valid KPDVE encodings in the list. (up to 84 for memory safety)*/
    int kpdve_min_index; /**< Index of the minimum distance KPDVE encoding. */
    int kpdve_list[84]; /**< List of possible KPDVE encodings (maximum size: 84). */
    int dve_list[84]; /**< List of associated DVE values for each KPDVE encoding. */
    int ve_list[84]; /**< List of associated VE values for each KPDVE encoding. */
};
typedef struct harmony_state harmony_state;


#endif /* harmony_state_h */

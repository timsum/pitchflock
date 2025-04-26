//
//  qdkpdve_statemaker.c
//  pitchflock
//
//  Created by Tim Summers on 6/12/23.
//

/**
 * @file qdkpdve_statemaker.c
 * @brief Implementation of functions for managing and analyzing harmony states in the pitchflock system.
 *
 * This file contains functions for generating, adjusting, and validating harmony states,
 * as well as calculating distances and encoding/decoding KPDVE values.
 * It provides the core logic for harmony state manipulation and analysis.
 */

#include "../include/qdkpdve_statemaker.h"
#include "../include/harmony_state.h"
#include "../include/qdkpdve.h"
#include "../include/qdkpdve_analysis.h"
#include "../include/qdkpdve_harmonycrystal.h"

// This is 12 * 7 --> if the system were to be expanded to other twin primes, this would become dynamic.
static int MAXKPDVELIST = 84;

// also specific to the 7/5 division
static int kpdve_mods[] = {12, 7, 7, 7, 7};

// Thesea are basically hyperparameters for the KPDVE distance function.
// they are used to weight the distance function, so that some parameters are more important than others.
// they would require some more investigation to determine the best values for different circumstances.

// biases toward axes (used for P, mainly... to tend away from entropic patterns -- NOT IMPLEMENTED
static float kpdve_axis_biases[] = {1.0, 1.0, 1.0, 1.0, 1.0};
// dilates the axis so distances are greater for key and pattern (harder to change) than for degree.
static float kpdve_axis_scale[] = {1.02, 1.01, 1.0, 1.0, 1.0};

/**
 * @brief Reduces an input value by applying modular rotation and compression.
 *
 * @param input_val The input value to process.
 * @param k The key value for modular rotation.
 * @param p The pattern value for compression.
 * @return The compressed value after applying modular rotation and pattern filtering.
 */
int undo_kp_for_input_val(int input_val, int k, int p)
{
    int reduced = mod_rot(input_val, -k, 12);
    int compressed = apply_p_filt(reduced, p);

    return compressed;
}

/**
 * @brief Generates a list of valid KPDVE encodings for the given harmony state.
 *
 * This function iterates through all possible Key-Pattern (KP) combinations
 * and calculates the corresponding Degree-Voiding-Extension (DVE) values.
 * It populates the `kpdve_list`, `dve_list`, and `ve_list` fields of the harmony state.
 *
 * @param a_state Pointer to the harmony state to process.
 */
void set_kp_list(harmony_state *a_state)
{
    int k, p;
    int match_count = 0;
    int dve_input;
    struct dve_value test_dve;
    int kpdve_temp[5];

    int circle_notes = chroma_to_circle(a_state->chromatic_notes);
    
    // this is new
    harmonycrystal the_crystal = default_harmonycrystal();
    
    // this is changed from i < 84
    for (int i = 0; i < the_crystal.crystalsize; i++)
    //for (int i = 0; i < 84; i++)
    {
        // this was moved out of the loop because now there is no array for the crystal... dynamically calculated with k and p
        k = (int)(i / 7);
        p = i % 7;

        if ((kp_for_harmonycrystal(the_crystal, k, p) & circle_notes) == circle_notes)
        //if ((harmony_crystal[i] & circle_notes) == circle_notes)
        {
            kpdve_temp[0] = k;
            kpdve_temp[1] = p;

            // undo the KP -- this yields a 7-bit combination of notes. The lydian base is moved to zero position, 
            // and pattern distortion is removed.
            dve_input = undo_kp_for_input_val(circle_notes, k, p);

            // now is ready for lowest dve
            test_dve = make_dve(dve_input);

            // run the dve through the minimizer to find its most efficient kpdve representation
            // #fixme: this needs more thorough documentation, so that the algorithm is clear.
            // in principle, it is the minimum shuffle. But I am using only fifths and thirds. 
            // -- AND the shuffling should be more thoroughly traced.
            test_dve = minimize_dve_value(test_dve);

            // extract the kpdve values from the minimized dve
            kpdve_temp[2] = test_dve.d;
            kpdve_temp[3] = test_dve.ve_val.v;
            kpdve_temp[4] = test_dve.ve_val.e;

            // record the most efficient values for the kp match
            a_state->dve_list[match_count] = test_dve.bin_val;
            a_state->ve_list[match_count] = test_dve.ve_val.bin_val;
            a_state->kpdve_list[match_count] = KPDVEtoBinaryEncoding(kpdve_temp);

            match_count++;
        }
    }
    a_state->kpdve_list_length = match_count;
}

//////////////////////////////

/**
 * @brief Calculates the modular distance between two values.
 *
 * @param val1 The first value.
 * @param val2 The second value.
 * @param mod The modulus for the calculation.
 * @return The modular distance between the two values.
 */
float modDistance(int val1, int val2, int mod)
{
    float a = val1;
    float b = val2;
    float diff = fabs(b - a);
    float wrapped_diff = mod - diff;
    return (diff < wrapped_diff) ? diff : wrapped_diff;
}

/**
 * @brief Calculates a biased modular distance between two values.
 *
 * This is not currently used. It may be used to bias the distance toward lower p values...
 * but this would mean that the p value would have to be re-centered to 0 which is rather fiddly.
 * 
 * @param val1 The first value.
 * @param val2 The second value.
 * @param mod The modulus for the calculation.
 * @param bias The bias factor to apply.
 * @return The biased modular distance between the two values.
 */
float biasedModDistance(int val1, int val2, int mod, float bias)
{
    float mod_dist = modDistance(val1, val2, mod);
    // Bias calculation: distance is weighted by proximity to 0
    float bias_factor_a = 1.0 / (fabs((float)val1) + 1);  // More weight near 0
    float bias_factor_b = 1.0 / (fabs((float)val2) + 1);
    float bias_factor = (bias_factor_a + bias_factor_b) / 2.0;

    // Adjust distance by bias factor
    return mod_dist * (1.0 + bias * bias_factor);
}

/**
 * @brief Calculates the distance between two KPDVE encodings.
 *
 * @param kpdve_1 The first KPDVE encoding.
 * @param kpdve_2 The second KPDVE encoding.
 * @return The distance between the two KPDVE encodings.
 */
double KPD_distance(int kpdve_1, int kpdve_2)
{
    float innerSum = 0;
    float dist = 0;

    int temp1[5];
    binaryEncodingToKPDVE(kpdve_1, temp1);
    
    int temp2[5];
    binaryEncodingToKPDVE(kpdve_2, temp2);
    
    for (int i = 0; i < 3; i++)
    {
        dist = modDistance(temp1[i], temp2[i], kpdve_mods[i]);
//        dist = biasedModDistance(temp1[i], temp2[i], kpdve_mods[i], kpdve_axis_biases[i]);
        dist *= kpdve_axis_scale[i]; // scales distance for param priorities
//        innerSum += dist * dist;
        innerSum += dist;
    }
    return innerSum;  
//    return sqrt(innerSum);
}

/**
 * @brief Sets the minimum index in the harmony state based on the context.
 *
 * This function compares the KPDVE list in the harmony state with the context
 * and selects the index with the minimum distance.
 *
 * @param current_state Pointer to the current harmony state.
 * @param context The context KPDVE value to compare against.
 */
void set_min_index(harmony_state *current_state, int context)
{
    double min_dist = 100.0f;
    double temp_dist = 0.0f;
    int min_index = 0;
    
    int kpdve_temp[5];
    int context_temp[5];
    //
    // COMPARE KPD VECTOR WITH VECTOR OF CONTEXT...
    //
    binaryEncodingToKPDVE(context, context_temp);

    for (int i = 0; i < current_state->kpdve_list_length; i++)
    {
        binaryEncodingToKPDVE(current_state->kpdve_list[i], kpdve_temp);
        
        // STAY IN SAME KP IF AT ALL POSSIBLE
        if ((kpdve_temp[0] == context_temp[0]) && (kpdve_temp[1] == context_temp[1]))
        {
            min_index = i;
            break;
        }
        temp_dist = KPD_distance(current_state->kpdve_list[i], context);
        
        // the question here is whether the context should also be anchored by an origin
        // -- a secondary context that is cumulative, and not just previous...
        // probably should live outside, especially since the 'context' here can be from any source
        
        // it can be a permanent feature (or option) to find this distance as a function of
        // 1) distance from the previous analysis
        // 2) distance from another harmony state.
        // this is already possible with the 'context', but where will it be called?
        
        if (temp_dist < min_dist)
        {
            min_dist = temp_dist;
            min_index = i;
        }
    }

    //    copy the lowest to the harmony_state, making kpdve array and kpdve struct
    current_state->kpdve_min_index = min_index;
}

/**
 * @brief Creates a harmony state from a binary chroma value (direction Hebrew).
 *
 * @param chroma_val The binary chroma value to process.
 * @return The generated harmony state.
 */
harmony_state harmony_state_from_binary(int chroma_val)
{
    harmony_state a_state;

    a_state.chromatic_notes = chroma_val & 0xFFF;
    set_kp_list(&a_state);
    
    
    a_state.encoded_state = kpdve_chromatic_byte(a_state.kpdve, a_state.chromatic_notes);

    return a_state;
}

// the context is usually just the previous analysis, but can also be a different context -- especially one
// with a deeper memory (buffered or cumulative)
/**
 * @brief Adjusts the harmony state context when choosing a KPDVE from the kpdve list after analyzing chroma.
 *
 * This function updates the chromatic notes of the harmony state and
 * recalculates the KPDVE list. It then selects the most appropriate KPDVE
 * encoding based on the provided context.
 *
 * @param a_state Pointer to the harmony state to adjust.
 * @param chroma_val The chroma value to analyze (12-bit integer).
 * @param context The context KPDVE value to guide the adjustment.
 */
void choose_kpdve_from_context(harmony_state *current_state, int context)
// REGULAR
{
    set_min_index(current_state, context);
    current_state->kpdve = current_state->kpdve_list[current_state->kpdve_min_index];
    current_state->dve = current_state->dve_list[current_state->kpdve_min_index];
    current_state->ve = current_state->ve_list[current_state->kpdve_min_index];

    encode_and_validate_state(current_state);
}


/**
 * @brief Creates a harmony state from a binary chroma value and a context KPDVE.
 *
 * @param chroma_val The binary chroma value to process (12 bits, left to right as in Hebrew).
 * @param contextkpdve The context KPDVE value to guide the adjustment.
 * @return The generated harmony state.
 */
harmony_state harmony_state_from_binary_w_context(int chroma_val, int contextkpdve)
{
    harmony_state a_state = harmony_state_from_binary(chroma_val & 0xFFF);
    choose_kpdve_from_context(&a_state, contextkpdve);
        
    return a_state;
}

/**
 * @brief Creates a harmony state from a KPDVE value.
 *
 * @param a_kpdve The KPDVE value to process.
 * @return The generated harmony state.
 */
harmony_state harmony_state_from_kpdve(int a_kpdve)
{

    harmony_state a_state;

    a_state.kpdve = a_kpdve;

    a_state.chromatic_notes = circle_to_chroma(kpdve_chord_val(a_kpdve));
    set_kp_list(&a_state);
    set_min_index(&a_state, a_state.kpdve);
    
    a_state.dve = a_state.dve_list[a_state.kpdve_min_index];
    a_state.ve = a_state.ve_list[a_state.kpdve_min_index];
    
    a_state.encoded_state = kpdve_chromatic_byte(a_state.kpdve, a_state.chromatic_notes);

    return a_state;
}

/**
 * @brief Creates a default harmony state with a specific KPDVE value -- an F major triad, as base of lydian mode.
 *
 * This function initializes a harmony state with a default KPDVE value
 * and sets the chromatic notes to represent a major triad.
 *
 * @return The default harmony state.
 */
harmony_state harmony_state_default(void) {
    return harmony_state_from_kpdve(34); // Default KPDVE value [0.0.0.4.2] -- F major triad
}

/**
 * @brief Creates a harmony state from a minimum encoding value (X~~~k___p__d__v__e__B-A-G-FE-D-C)
 *
 * This function extracts the KPDVE and chromatic notes from the provided
 * minimum encoding value and creates a corresponding harmony state.
 *
 * @param kpdve_bin_encoding The minimum encoding value (binary).
 * @return The generated harmony state.
 */
harmony_state harmony_state_from_min_encoding(int kpdve_bin_encoding){
    harmony_state new_state = harmony_state_from_kpdve(kpdve_bin_encoding >> 12);
    new_state.chromatic_notes = kpdve_bin_encoding & 0xFFF;
    
    return new_state;
}

void adjust_harmony_state_from_min_encoding(harmony_state *a_state, int kpdve_bin_encoding){
    adjust_harmony_state_from_kpdve(a_state, kpdve_bin_encoding >> 12);
    a_state->chromatic_notes = kpdve_bin_encoding & 0xFFF;
    
    encode_and_validate_state(a_state);
}

/**
 * @brief Adjusts an existing harmony state based on chroma input
 *
 * This function updates the chromatic notes of the harmony state and
 * recalculates the KPDVE list. It then selects the most appropriate KPDVE
 * encoding based on the current state.
 *
 * @param a_state Pointer to the harmony state to adjust.
 * @param chroma_val The chroma value to analyze (12-bit integer, left to right as in Hebrew).
 */
void adjust_harmony_state_from_chroma(harmony_state *a_state, int chroma_val)
{
    a_state->chromatic_notes = chroma_val & 0xFFF;
    set_kp_list(a_state);
    // the kpdve is still from the previous, and provides context for analysis.

    choose_kpdve_from_context(a_state, a_state->kpdve);
}

/**
 * @brief Adjusts a harmony state based on chroma AND context.
 *
 * This function updates the chromatic notes of the harmony state and
 * recalculates the KPDVE list. It then selects the most appropriate KPDVE
 * encoding based on the provided context. Creates a hybrid markov chain.
 *
 * @param a_state Pointer to the harmony state to adjust.
 * @param chroma_val The chroma value to analyze (12-bit integer).
 * @param context The context KPDVE value to guide the adjustment.
 */
void adjust_harmony_state_from_chroma_and_context(harmony_state *a_state, int chroma_val, int context)
{
    a_state->chromatic_notes = chroma_val & 0xFFF;
    set_kp_list(a_state);
    // the kpdve is still from the previous, and provides context for analysis.

    choose_kpdve_from_context(a_state, context);
}

/**
 * @brief Adjusts a harmony state based on chroma (LEFT TO RIGHT AS IN ENGLISH!) and context.
 *
 * This function updates the chromatic notes of the harmony state and
 * recalculates the KPDVE list. It then selects the most appropriate KPDVE
 * encoding based on the provided context. . Creates a hybrid markov chain.
 *
 * @param a_state Pointer to the harmony state to adjust.
 * @param chroma_val The chroma value to analyze (12-bit integer).
 * @param context The context KPDVE value to guide the adjustment.
 */
void adjust_harmony_state_from_chroma_lr_and_context(harmony_state *a_state, int chroma_val, int context)
{
    a_state->chromatic_notes = reverse_12_bits(chroma_val & 0xFFF);
    set_kp_list(a_state);
    // the kpdve is still from the previous, and provides context for analysis.

    choose_kpdve_from_context(a_state, context);
}

/**
 * @brief Reverses the bits of a 12-bit integer.
 *
 * @param num The 12-bit integer to reverse.
 * @return The reversed 12-bit integer.
 */
int reverse_12_bits( int num) {
    int reversed = 0;
    for (int i = 0; i < 12; i++) {
        reversed <<= 1;
        reversed |= (num & 1);
        num >>= 1;
    }
    return reversed;
}

/**
 * @brief Adjusts a harmony state based on a KPDVE value.
 *
 * This function updates the KPDVE value of the harmony state and
 * recalculates the chromatic notes. It also sets the KPDVE list and
 * selects the minimum index based on the new KPDVE value.
 *
 * @param a_state Pointer to the harmony state to adjust.
 * @param a_kpdve The KPDVE value to set.
 */ 
void adjust_harmony_state_from_kpdve(harmony_state *a_state, int a_kpdve){
    a_state->kpdve = a_kpdve;
    a_state->chromatic_notes = circle_to_chroma(kpdve_chord_val(a_kpdve));
    
    set_kp_list(a_state);
    set_min_index(a_state, a_state->kpdve);
    
    a_state->dve = a_state->dve_list[a_state->kpdve_min_index];
    a_state->ve = a_state->ve_list[a_state->kpdve_min_index];
    
    encode_and_validate_state(a_state);
}

/**
 * @brief Encodes and validates the harmony state.
 *
 * This function encodes the harmony state into a binary format and validates
 * its integrity by checking the number of chromatic notes and the KPDVE list length.
 *
 * @param a_state Pointer to the harmony state to encode and validate.
 */
void encode_and_validate_state(harmony_state *a_state) {
    a_state-> encoded_state = kpdve_chromatic_byte(a_state->kpdve, a_state->chromatic_notes);
    bool isInvalid = false;
    int validity_bit = (1 << 31);
    //
    int tester = a_state->chromatic_notes;
    // check that there are not more than 7
    int matches = 0;
    while (tester > 0) {
        if ((1 & tester) > 0) {
            matches++;
        }
        if (matches > 7) {
            isInvalid = true;
        }
        tester >>= 1;
    }
    // check that there are any notes at all.
    if (!(a_state->kpdve_list_length > 0)) {
        isInvalid = true;
    }
    if (isInvalid) {
        a_state -> encoded_state |= validity_bit;
    }
//    else {
//        // make a record that something happened, and what it was.
//        a_state->num_analyses += 1;
//        int recorder = a_state->chromatic_notes;
//        int bin = 0;
//        while (recorder > 0) {
//            if ((1 & recorder) > 0) {
//                a_state->cumulative_chroma_analyses[bin]++;
//            }
//            recorder >>= 1;
//            bin++;
//        }
//    }
}

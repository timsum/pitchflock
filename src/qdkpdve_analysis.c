//
//  qdkpdve_analysis.c
//  pitchflock
//
//  Created by Tim Summers on 6/12/23.
//



#include "../include/qdkpdve_analysis.h"
#include "../include/qdkpdve.h"
#include <stdio.h> // For printf


// for constructing/comparing ve_values in a single loop.
 // finding minimally entropic representations of any number (up to 7 binary digits)
 // in ve context and in dve context


/**
 * @brief Returns the position of the largest bit set to 1 in the given unsigned integer.
 *
 * If the input is zero, it returns -1.
 *
 * @param aUInt The unsigned integer to analyze.
 * @return The position of the largest bit set to 1, or -1 if the input is zero.
 */
int largest_bit(int aUInt)
{
    int v = aUInt;
    int counter = 0;

    while (v > 0)
    {
        counter++;
        v >>= 1;
    }
    // The largest bit position is 0-indexed, so we subtract 1.
    // If the input is zero, we return -1.
    return counter - 1;
}

/**
 * @brief Shuffles the bits of a given value based on the specified breadth.
 *
 * It interleaves the lower and upper bits of the input value by breadth.
 * 
 * The shuffling allows for the ordered discovery of a minimal value. 
 * 
 * The 'shuffling' is equivalent to a deck of cards cleanly shuffled.
 * #FIXME: THIS NEEDS FURTHER CLARIFICATION. ESPECIALLY IF THE SYSTEM SHOULD BE EXTENDED TO ALL POSSIBLE TWIN PRIMES
 * in principle, the breadth should be an even number, to allow for a clean shuffle.
 *
 * @param ve_val The value to shuffle.
 * @param breadth The breadth to use for shuffling. (not yet implemented)
 * @note The breadth parameter is not currently used in the implementation.
 * @note The function assumes that the input value is a 7-bit integer, fit into an 8-bit byte
 * @return The shuffled value.
 */
int shuffle_bits(int ve_val, int breadth)
{
    int result = 0;
    // do it the obvious way...
    for (int i = 0; i < 4; i++)
    {
        result |= (ve_val & 1 << i) << i;
        result |= (ve_val & 128 >> i) >> i;
    }
    return result;
}

/**
 * @brief Reverses the bit shuffling performed by shuffle_bits.
 *
 * It applies the shuffle_bits function twice to achieve the unshuffling.
 *
 * @param ve_val The value to unshuffle.
 * @param breadth The breadth to use for unshuffling.
 * @return The unshuffled value.
 */
int unshuffle_bits(int ve_val, int breadth)
{
    return shuffle_bits(shuffle_bits(ve_val, breadth), breadth);
}

/**
 * @brief Creates a ve_value struct from the given input value.
 *
 * It calculates the binary value, v, and e (largest bit position) for the struct.
 * allows for the discovery of root position by finding the least possible value (closest regular ordering)
 *
 * @param input_val The input value to convert.
 * @return The constructed ve_value struct.
 */
struct ve_value make_ve(int input_val)
{
    struct ve_value ve;

    ve.bin_val = input_val;
    ve.v = 0;
    ve.e = largest_bit(input_val);

    return ve;
}

/**
 * @brief Creates a dve_value struct from the given input value.
 *
 * It initializes the binary value, d, and ve_value for the struct.
 *
 * @param input_val The input value to convert.
 * @return The constructed dve_value struct.
 */
struct dve_value make_dve(int input_val)
{
    struct dve_value dve;

    dve.bin_val = input_val;
    dve.d = 0;
    dve.ve_val = make_ve(input_val);

    return dve;
}

/**
 * @brief Creates a kpdve_value struct from the given input value.
 *
 * It initializes the binary value, k, p, and dve_value for the struct.
 *
 * @param input_val The input value to convert.
 * @return The constructed kpdve_value struct.
 */
struct kpdve_value make_kpdve(int input_val)
{
    struct kpdve_value kpdve;
    kpdve.bin_val = input_val;
    kpdve.k = 0;
    kpdve.p = 0;
    kpdve.dve_val = make_dve(0);

    return kpdve;
}

/**
 * @brief Minimizes a ve_value by finding the representation with the smallest binary value.
 *
 * It iterates through possible unshuffled values to determine the minimal representation.
 *
 * @param ve_val The ve_value to minimize.
 * @return The minimized ve_value.
 */
struct ve_value minimize_ve_value(struct ve_value ve_val)
{
    struct ve_value min_ve = ve_val;
    
    // extract the binary value from ve_val
    int test_val = ve_val.bin_val;
    for (int i = 0; i < 3; i++)
    {
        // if the test value is less than the current minimum, update the minimum
        if (ve_val.bin_val >= test_val)
        {
            min_ve.bin_val = test_val;
            // make the v value the next power of two
            min_ve.v = 1 << i;
            min_ve.e = largest_bit(test_val);
        }
        // 
        test_val = unshuffle_bits(test_val, 8);
    }
    return min_ve;
}

/**
 * @brief Minimizes a dve_value by finding the representation with the smallest ve_value.
 *
 * It iterates through rotations of the binary value to determine the minimal representation.
 * This minimization allows for the discovery of the best (but not the only) taxonomic representation of a group of notes.
 *
 * @param dve_val The dve_value to minimize.
 * @return The minimized dve_value.
 */
struct dve_value minimize_dve_value(struct dve_value dve_val)
{
    struct dve_value min_dve = dve_val; // make a copy to return
    
    int slide_val = dve_val.bin_val;
    struct ve_value test_ve;
    int low_ve = 0b10000000;

    for (int i = 0; i < 7; i++)
    {
        // find the d values...
        if ((slide_val & 1) == 1)
        {
            test_ve = make_ve(slide_val);
            test_ve = minimize_ve_value(test_ve);
            if (low_ve >= test_ve.bin_val)
            {
                min_dve.ve_val = test_ve;
                min_dve.d = i;
                low_ve = test_ve.bin_val;
            }
        }
        slide_val = mod_rot(slide_val, -1, 7);
    }
    return min_dve;
}

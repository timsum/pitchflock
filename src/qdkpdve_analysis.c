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
 * #FIXME: THIS NEEDS FURTHER CLARIFICATION. ESPECIALLY IF THE SYSTEM SHOULD BE EXTENDED TO MORE TWIN PRIMES
 * in principle, the breadth should be an even number, to allow for a clean shuffle. It is quite possible that it might best be be a power of 2 for future use.
 * Powers of two have the smallest number of shuffles to return -- the number of shuffles in that case seems to be equal to the power of 2.
 * other even numbers have a different number of shuffles (always larger?) based on prime factors of n-1.
 *
 * @param ve_val The value to shuffle (a so-called out-shuffle).
 * @param breadth The breadth to use for shuffling. In this scheme, it is always 8.
 * 
 * start:             [7,6,5,4,3,2,1,0]
 * After 1st shuffle: [7,3,6,2,5,1,4,0]
 * After 2nd shuffle: [7,5,3,1,6,4,2,0]
 * After 3rd shuffle: [7,6,5,4,3,2,1,0]
 * 
 * 7 is always zero, but keeps the split even.
 * 
 * @note The breadth parameter splits the bits like a deck of cards, and is made to be an even number if it comes in as an odd number.
 * @note The function is used with an input value is a 7-bit integer, fit into an 8-bit byte
 * 
 * @return The shuffled value.
 */
int shuffle_bits(int ve_val, int breadth)
{
    int result = 0;
    
    // The breadth should be an even number for a clean shuffle.
    int shifts = ((breadth & 1) ? breadth : breadth + 1) / 2; // if breadth is odd, make it even to splity clean in half with an extra bit.

    // root is always first; last bit is always last.

    for (int i = 0; i < shifts; i++)
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
 * this function assumes that the breadth is 8. 
 * #FIXME For this to be a proper unshuffle, it would have to adjust number of function calls based on the breadth. To start with, it would have to be the power of 2 which
 * corresponds to the breadth (right now, it is 2^3 = 8). And the number of shuffles whould have to that power minus 1...
 * 
 * @note This is the reciprocal of the shuffle_bits function because a cycle of 3 shuffles returns the original value. To do two shuffles is to move in the other direction.
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
            min_ve.v = 1 << i; // can be 1 (fifths), 2 (scale up), or 4 (thirds)
            min_ve.e = largest_bit(test_val); // how far the value must reach to contain all bits
        }
        // unshuffling the bits yields scale from fifths, thirds from scale, and fifths from thirds // shuffle_bits would put the 
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

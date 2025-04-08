//
//  qdkpdve.c
//  qdkpdve
//
//  Created by Tim Summers on 6/12/23.
//

#include "../include/qdkpdve.h" // Ensure correct relative path

// Function definitions
// ////////////////////////////////// Minimum encoding

/**
 * @brief Number of chromatic notes in an octave.
 */
int chromaCount = 12;

/**
 * @brief Number of divisions in the prime scale.
 */
int primeDivision = 7;

/**
 * @brief KPDVE representation of FM7 Lydian chord.
 */
int FM7LydianKPDVE[] = { 0, 0, 0, 4, 3 };

/**
 * @brief Chromatic representation of FM7 Lydian chord.
 */
int FM7LydianChroma = 0b001000110001;

/**
 * @brief Encodes a KPDVE and chromatic value into a single byte.
 *
 * @param kpdve The KPDVE value to encode.
 * @param chromatic The chromatic value to encode.
 * @return The encoded byte.
 */
int kpdve_chromatic_byte(int kpdve, int chromatic)
{
    return (kpdve << 12) | chromatic;
}

/**
 * @brief Decodes an encoded byte into KPDVE values.
 *
 * @param encodedByte The encoded byte to decode.
 * @param result Array to store the decoded KPDVE values.
 */
void kpdve_chromatic_byte_to_kpdve(int encodedByte, int result[])
{
    binaryEncodingToKPDVE(encodedByte >> 12, result);
}

/**
 * @brief Converts a KPDVE array to a binary encoding.
 *
 * @param aKPDVE The KPDVE array to encode.
 * @return The binary encoding of the KPDVE array.
 */
int KPDVEtoBinaryEncoding(int aKPDVE[])
{
    int encodedByte = 0;

    for (int i = 0; i < 5; i++)
    {
        encodedByte <<= 3;
        encodedByte |= (int)aKPDVE[i];
    }
    return encodedByte;
}

/**
 * @brief Converts a binary encoding to KPDVE values.
 * 
 * Important because kpdve values are maily used in binary encoding, but 
 * individual parameters are often used in extrapolations and analysis.
 *
 * @param encodedByte The encoded byte to decode.
 * @param result Array to store the decoded KPDVE values.
 */
void binaryEncodingToKPDVE(int encodedByte, int result[])
{
    int chompedByte = encodedByte;

    for (int i = 0; i < 4; i++)
    {
        result[4 - i] = (int)(chompedByte & (int)0x0007);
        chompedByte >>= 3;
    }
    result[0] = chompedByte;
}

/**
 * @brief Gets the KPDVE parameter value at a specific index from an encoded KPDVE.
 *
 * @param kpdve The KPDVE value.
 * @param index The index of the parameter to retrieve (0-4).
 * @return The KPDVE parameter value at the specified index.
 */
int getKPDVEparameterValue(int kpdve, int index)
{
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    return result[index];
}

/**
 * @brief Gets a chroma chord from an encoded KPDVE value.
 *
 * @param kpdve The KPDVE value.
 * 
 * @return Chroma-ordered chord value, encoded as in a 12-bit integer
 *         (right to left, as in Hebrew).
 */
int chroma_chord_from_kpdve(int kpdve) {
    return circle_to_chroma(kpdve_chord_val(kpdve));
}

/**
 * @brief Gets a chroma scale from an encoded KPDVE value.
 *
 * @param kpdve The KPDVE value.
 * 
 * @return Chroma-ordered scale for the KPDVE, encoded as 7 bits in a 12-bit integer
 *         (right to left, as in Hebrew).
 */
int chroma_scale_from_kpdve(int kpdve) {
    return circle_to_chroma(circle_scale_from_kpdve(kpdve));
}

/**
 * @brief Gets a chroma root from an encoded KPDVE value.
 *
 * @param kpdve The KPDVE value.
 * 
 * @return Chroma-ordered root for the KPDVE, encoded as 1 bit in a 12-bit integer
 *         (right to left, as in Hebrew).s
 */
int chroma_root_from_kpdve(int kpdve) {
    return circle_to_chroma(circle_root_from_kpdve(kpdve));
}

/**
 * @brief Gets a chroma extension from an encoded KPDVE value.
 *
 * @param kpdve The KPDVE value.
 * 
 * @return Chroma-ordered extension for the KPDVE, encoded as 1 bit in a 12-bit integer
 *         (right to left, as in Hebrew).
 */
int chroma_ext_from_kpdve(int kpdve) {
    return circle_to_chroma(circle_ext_from_kpdve(kpdve));
}

/**
 * @brief Gets a circle ordered chord for an encoded KPDVE value.
 *
 * @param kpdve The KPDVE value.
 * 
 * @return Circle-ordered voicing for the KPDVE, encoded as 1 bit in a 12-bit integer
 *         (right to left, as in Hebrew).
 */
int circle_chord_from_kpdve(int kpdve) {
    return kpdve_chord_val(kpdve);
}

/**
 * @brief Gets a circle ordered scale for an encoded KPDVE value.
 * * 
 * @param kpdve The KPDVE value.
 * 
 * @return Circle-ordered scale for the KPDVE, encoded as 7 bits in a 12-bit integer
 *         (right to left, as in Hebrew).
 */
int circle_scale_from_kpdve(int kpdve) {
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    int scale_kpdve[] = { result[0], result[1], result[2], result[3], 6 };
    int encoded_kpdve = KPDVEtoBinaryEncoding(scale_kpdve);
    
    return kpdve_chord_val(encoded_kpdve);
}

/**
 * @brief Gets a circle ordered root for an encoded KPDVE value.
 *
 * @param kpdve The KPDVE value.
 * 
 * @return Circle-ordered root for the KPDVE, encoded as 1 bit in a 12-bit integer
 *         (right to left, as in Hebrew).
 */ 
int circle_root_from_kpdve(int kpdve) {
    
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    int root_kpdve[] = { result[0], result[1], result[2], result[3], 0 };
    int encoded_kpdve = KPDVEtoBinaryEncoding(root_kpdve);
    
    return kpdve_chord_val(encoded_kpdve);
}

/**
 * @brief Gets a circle ordered extension for an encoded KPDVE value.
 *
 * @param kpdve The KPDVE value.
 * 
 * @return Circle-ordered extension for the KPDVE, encoded as 1 bit in a 12-bit integer
 *         (right to left, as in Hebrew).
 */
int circle_ext_from_kpdve(int kpdve) {
    return kpdve_val(kpdve);
}

/**
 * @brief Exchanges chroma order for circle order and vice versa.
 *
 * @param a 12-bit integer representing
 * 
 * @return inverts entropy of chroma and circle values, so that
 */
int chroma_circle_hash(int val) {
    return (val & 0x555) | mod_rot(val & 0xAAA, 6, chromaCount);
}

/**
 * @brief Converts a chroma value to a circle value.
 *
 * @param val The chroma value to convert.
 * 
 * @return The corresponding circle value.
 */
int circle_to_chroma(int val) {
    return chroma_circle_hash(mod_rot(val, -1, chromaCount));
}

/**
 * @brief Converts a chroma value to a circle value.
 *
 * @param val The chroma value to convert.
 * 
 * @return The corresponding circle value.
 */
int chroma_to_circle(int val) {
    return mod_rot(chroma_circle_hash(val), 1, chromaCount);
}

/**
 * @brief Gets a bunch of consecutive 1s in a byte
 *
 * @param breadth The breadth value.
 * 
 * @return The bit bunch value.
 */
int bit_bunch(int breadth) {
    return (1 << breadth) - 1;
}

/**
 * @brief Filters for creating patterns out of lydian (non-entropic) mode sources
 *
 * @param breadth The breadth value.
 * 
 * e.g. 4 = 0b10001
 * 
 * @return The filter value.
 */
int key_filt(int breadth) {
    return (1 << breadth) + 1;
}

/// shifts value, rot to the left, modulo mod
/**
 * @brief Rotates a value within a byte to the left by a specified number of bits.
 * 
 * Typicall used for modulation of circle values.
 *
 * @param val The value to rotate.
 * @param rot The number of bits to rotate to the LEFT
 * @param mod The modulus for the rotation (typically 12 for chroma, 7 for scales)
 * 
 * @return The rotated value.
 */

int mod_rot(int val, int rot, int mod) {
    int rot_small = loop_mod(rot, mod);
    return bit_bunch(mod) & ((val << rot_small) | (val >> (mod - rot_small)));
}

/**
 * @brief Loops a value within a specified modulus.
 *
 * @param x The value to loop.
 * @param mod The modulus for the loop.
 * 
 * @return The looped value.
 */
int loop_mod(int x, int mod) {
    if (mod == 0) return x;
    int r = x % mod;
    return r < 0 ? r + mod : r;
}

// this was once part of a fancier idea.
int v_shift(int v) {
    return v;
}

/**
 * @brief Gets the value of a voicing for a given extension. returns a single bit in a set of bits of size primeDivision
 *
 * These values are used to find the minimum value of a voicing (collected in the function below).
 * 
 * @param v The voicing value.
 * @param e The extension value.
 * 
 * @return The voicing value.
 */
int ve_val(int v, int e) {
    if (v == 0) {
        return 1;
    } else {
        return mod_rot(1, v_shift(v) * e, primeDivision);
    }
}

/**
 * @brief collects the voicing bits for a given extension.
 *
 * These values are used to find the minimum value of a voicing.
 * 
 * @param v The voicing value.
 * @param e The extension value.
 * 
 * @return The collected values of the voicing in a kpd-independent setting.
 */
int ve_chord_val(int v, int e) {
    int val = 0;
    int e_sign = (e < 0) ? -1 : 1;
    for (int i = 0; i <= abs(e); i++) {
        val |= ve_val(v, e_sign * i);
    }
    return val;
}

/**
 * @brief Gets the value of a voicing for a given extension and degree.
 *
 * These values are used to begin to find the minimum value of a voicing (collected in the function below).
 * 
 * @param d The degree value.
 * @param v The voicing value.
 * @param e The extension value.
 * 
 * @return The voicing value.
 */
int dve_val(int d, int v, int e) {
    return mod_rot(ve_val(v, e), d, primeDivision);
}

/**
 * @brief Collects the voicing bits for a given extension and degree.
 *
 * These values are used to find the minimum value of a voicing.
 * 
 * @param d The degree value.
 * @param v The voicing value.
 * @param e The extension value.
 * 
 * @return The collected values of the voicing in a kp-independent setting.
 */
int dve_chord_val(int d, int v, int e) {
    return mod_rot(ve_chord_val(v, e), d, primeDivision);
}

/**
 * @brief Creates a filter to remove or apply pattern distortion.
 *
 * @param p_val the second param of the KPDVE
 * @param breadth The breadth value.
 * 
 * @return the value which will function as a filter.
 */

int p_filt(int p_val, int breadth) {
    int f = 0;
    if (p_val > 0) {
        if (p_val > 3) {
            p_val = -(~p_val & 3);
        } else {
            p_val = p_val - 1;
        }
        f = mod_rot(key_filt(breadth), p_val, chromaCount);
    }
    return f;
}

int apply_p_filt(int val, int p) {
    int filter = p_filt(p, 7);
    return ((filter & val) > 0) ? val ^ filter : val;
}

/**
 * @brief Finds a dve value for a single-bit dve value then applies a pattern distortion.
 *
 * @param p The pattern value.
 * @param d The degree value.
 * @param v The voicing value.
 * @param e The extension value.
 * 
 * @return A Pattern-filtered DVE value.
 */
int pdve_val(int p, int d, int v, int e) {
    return apply_p_filt(dve_val(d, v, e), p);
}

/**
 * @brief Finds a chordal dve value for a given degree, voicing, and extension, then applies a pattern distortion.
 *
 * @param p The pattern value.
 * @param d The degree value.
 * @param v The voicing value.
 * @param e The extension value.
 * 
 * @return A Pattern-filtered DVE value.
 */
int pdve_chord_val(int p, int d, int v, int e) {
    return apply_p_filt(dve_chord_val(d, v, e), p);
}

/**
 * @brief Applies a key rotation to a PDVE value after it has been calculated.
 *
 * @param kpdve The KPDVE value, encoded as a 16-bit integer.
 * 
 * @return The KPDVE value for the given KPDVE, encoded as a 12-bit integer by fifths
 *         (right to left, as in Hebrew, starting at F).
 */
int kpdve_val(int kpdve) {
    // =========================================================================> HERE EXTRACT VALUES
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    return mod_rot(pdve_val(result[1], result[2], result[3], result[4]), result[0], chromaCount);
}

/**
 * @brief Gets the chord value for a given KPDVE, in circle order.
 *
 * @param kpdve The KPDVE value.
 * 
 * @return The chord value for the KPDVE, encoded as a 12-bit integer by fifths
 *         (right to left, as in Hebrew, starting at F).
 */
int kpdve_chord_val(int kpdve) {
    // =========================================================================> HERE EXTRACT VALUES
    int result[5];
    binaryEncodingToKPDVE(kpdve, result);
    
    return mod_rot(pdve_chord_val(result[1], result[2], result[3], result[4]), result[0], chromaCount);
}


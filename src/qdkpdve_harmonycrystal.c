//
//  qdkpdve_harmonycrystal.c
//  harmony_swiftdata
//
//  Created by Tim Summers on 4/6/24.
//

#include "../include/qdkpdve_harmonycrystal.h"

// this is the basic model for finding patterns of bits within prime-numbered buneches of bits.
// it treats the F Lydian mode as the core
// each bit, read right to left as in Hebrew,
harmonycrystal harmonycrystal_atmultiple(int twelvemult) {
    harmonycrystal a_crystal;
    
    a_crystal.twelvemultiple = twelvemult;
    
    // makes more sense to have -1, then +1, but for now it's like this.
    a_crystal.divs[0] = 6 * a_crystal.twelvemultiple + 1;
    a_crystal.divs[1] = 6 * a_crystal.twelvemultiple - 1;

    a_crystal.twinprimes = (is_prime( a_crystal.divs[0]) && is_prime( a_crystal.divs[1]));
    a_crystal.divindex = 0; // using the crystal or its complement...
    
    a_crystal.crystalsize = a_crystal.divs[a_crystal.divindex] * 12 * twelvemult;

    return a_crystal;
}

// This gives 7 and 5. A multiple of four would give 23 and 25... unclear whether higher multiples are any use
harmonycrystal default_harmonycrystal(void){
    return harmonycrystal_atmultiple(1);
}

int kp_for_harmonycrystal(harmonycrystal a_crystal, int k, int p)
{
    //
    int mode_model = bit_bunch(a_crystal.divs[a_crystal.divindex]);
    int mode_with_p = apply_p_filt(mode_model, p);
    int key_adjusted = mod_rot(mode_with_p, k, a_crystal.twelvemultiple * 12);
    
    return key_adjusted;
}

// just check to see if there are twin primes in the crystal division, so that they both... create unique divisions.
// what is interesting is that each prime group of bits contains all the lower primes inside as unique combinations.
// unique numbers, even...
// these would be worth graphing.
bool is_prime(int num) {
    if (num <= 1) {
        return false;  // 1 and numbers less than 1 are not prime
    }
    if (num <= 3) {
        return true;   // 2 and 3 are prime
    }
    if (num % 2 == 0 || num % 3 == 0) {
        return false;  // Numbers divisible by 2 or 3 are not prime (excluding 2 and 3)
    }
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) {
            return false;  // Numbers divisible by i or (i + 2) are not prime
        }
    }
    return true;  // If num is not divisible by any number less than or equal to its square root, it's prime
}

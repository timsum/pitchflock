//
//  qdkpdve_harmonycrystal.h
//  harmony_swiftdata
//
//  Created by Tim Summers on 4/6/24.
//

#ifndef qdkpdve_harmonycrystal_h
#define qdkpdve_harmonycrystal_h

#include <stdio.h>
#include <stdbool.h>
#include "qdkpdve.h"

struct harmonycrystal {
    int twelvemultiple;
    
    int divs[2];
    bool twinprimes;
    
    int divindex;
    int crystalsize;
};
typedef struct harmonycrystal harmonycrystal;

harmonycrystal harmonycrystal_atmultiple(int twelvemult);
harmonycrystal default_harmonycrystal(void);
int kp_for_harmonycrystal(harmonycrystal a_crystal, int k, int p);

bool is_prime(int num);

#endif /* qdkpdve_harmonycrystal_h */

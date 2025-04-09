#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "../include/harmony_state.h"
#include "../include/qdkpdve_statemaker.h"
#include "../include/qdkpdve_naming.h"

#define BIGLOOP 120
#define C7 0b10010010001
#define CM7 0b100010010001

/**
 * @brief Demonstrates a sequence of major triads modulating up by half steps.
 *
 * This function iterates through a series of chromatic modulations, updating
 * the harmony state and printing its properties at each step.
 */

 void print_colored_binary(int value, int bits)
{
    for (int i = bits - 1; i >= 0; i--)
    {
        if ((value >> i) & 1)
        {
            // Print 1 in green
            printf("\033[32m1\033[0m");
        }
        else
        {
            // Print 0 in white
            printf("\033[37m0\033[0m");
        }
    }
}

 // This function prints a summary of the analysis of a harmony_state structure.
// It displays the encoded state, chromatic notes, KPDVE representation, and the circle scale derived from KPDVE.
void printAnalysisSummary(harmony_state *state)
{
        // Print the binary representation of the encoded state in 32 bits, with colored formatting.
        print_colored_binary(state->encoded_state, 32);
        printf(" ");

        // Print the binary representation of the chromatic notes in 12 bits, with colored formatting.
        // print_colored_binary(chroma_to_circle(state->chromatic_notes), 12);
        // printf(" ");

        // Print the binary representation of the circle scale derived from KPDVE in 12 bits, with colored formatting.
        print_colored_binary(circle_scale_from_kpdve(state->kpdve), 12);
        printf(" ");

        // print the DVE and VE (D reduced to rightmost) values in 7 bits each, with colored formatting.
        print_colored_binary(state->dve, 7);
        printf(" ");

        print_colored_binary(state->ve, 7);
        printf(" ");

        char chordNotes[30];
        chordNotesStringForKPDVE(state->kpdve, chordNotes, sizeof(chordNotes));
        printf("%25s", chordNotes);
        printf("> %18s ", scaleNameForKPDVE(state->kpdve));
        printf("%3s", conventionalTonicStringForKPDVE(state->kpdve));
        printf("%14s ", modeNameForKPDVE(state->kpdve));
        printf("%3s",  patternDistortionForKPDVE(state->kpdve));
        printf("%3s", rootStringForKPDVE(state->kpdve));
        printf("%5s ", conventionalDegreeStringForKPDVE(state->kpdve));
        printf("%3d poss.", state->kpdve_list_length);

}

void majorTriadSequenceModulatingUp()
{
    printf("Major triad modulating up by half step\n");
    printf("~~~~k___p__d__v__e__B-A-G-FE-D-C bb###BEADGCF\n");
    int context = 0;
    harmony_state default_state = harmony_state_default();

    for (int i = 0; i < 12; i++)
    {
        adjust_harmony_state_from_chroma_and_context(&default_state, mod_rot(C7, i, 12), context);

        // Print the values of the harmony_state struct
        printAnalysisSummary(&default_state);
        printf("\n");
        context = default_state.kpdve;
    }
}

/**
 * @brief Demonstrates a sequence of major triads modulating up by fifths.
 *
 * This function iterates through a series of modulations by fifths, updating
 * the harmony state and printing its properties at each step.
 */
void majorTriadSequenceModulatingUpFifths()
{
    printf("Major triad modulating up by fifths\n");
    printf("~~~~k___p__d__v__e__B-A-G-FE-D-C bb###BEADGCF\n");
    int context = 0;
    harmony_state default_state = harmony_state_default();

    for (int i = 0; i <= 12; i++)
    {
        adjust_harmony_state_from_chroma_and_context(&default_state, mod_rot(C7, (i * 7) % 12, 12), context);

        // Print the values of the harmony_state struct
        printAnalysisSummary(&default_state);
        printf("\n");
        context = default_state.kpdve;
    }
}

/**
 * @brief Demonstrates a sequence of major triads modulating down by half steps.
 *
 * This function iterates through a series of chromatic modulations, updating
 * the harmony state and printing its properties at each step.
 */
void majorTriadSequenceModulatingDown()
{
    printf("Major triad modulating down by half step\n");
    printf("~~~~k___p__d__v__e__B-A-G-FE-D-C bb###BEADGCF\n");
    int context = 0;
    harmony_state default_state = harmony_state_from_binary_w_context(C7, 35);

    for (int i = 0; i < 12; i++)
    {
        adjust_harmony_state_from_chroma_and_context(&default_state, mod_rot(C7, -i, 12), context);

        // Print the values of the harmony_state struct
        printAnalysisSummary(&default_state);
        printf("\n");
        context = default_state.kpdve;
    }
}

/**
 * @brief Demonstrates a sequence of major triads modulating down by fifths.
 *
 * This function iterates through a series of modulations by fifths, updating
 * the harmony state and printing its properties at each step.
 */
void majorTriadSequenceModulatingDownFifths()
{
    printf("Major triad modulating down by fifths\n");
    printf("~~~~k___p__d__v__e__B-A-G-FE-D-C bb###BEADGCF\n");
    int context = 0;
    harmony_state default_state = harmony_state_from_binary_w_context(C7, 35);

    for (int i = 0; i <= 12; i++)
    {
        adjust_harmony_state_from_chroma_and_context(&default_state, mod_rot(C7, -(i * 7) % 12, 12), context);

        // Print the values of the harmony_state struct
        printAnalysisSummary(&default_state);
        printf("\n");
        context = default_state.kpdve;
    }
}

/**
 * @brief Scrolls through all possible binary values and analyzes their harmony state.
 *
 * This function iterates through all possible 12-bit binary values, generates the corresponding
 * harmony state, and prints the analysis summary for each state.
 */
void scrollBinaryValues()
{
    int context = 35;
    harmony_state default_state = harmony_state_default();

    for (int i = 0; i < 0b111111100000; i++)
    {
        adjust_harmony_state_from_chroma_and_context(&default_state, i, context);
        if (default_state.kpdve_list_length == 0)
        {
            printAnalysisSummary(&default_state);
            printf("\n");
        }
        else
        {
            printAnalysisSummary(&default_state);
            printf("\n");
            context = default_state.kpdve;
        }
    }
}

/**
 * @brief Scrolls through all possible KPDVE values and analyzes their harmony state.
 *
 * This function iterates through all possible KPDVE values, generates the corresponding
 * harmony state, and prints the analysis summary for each state.
 */
void scrollKPDEvalues()
{
    int kpdve[5];
    int kpdveInteger = 0;

    harmony_state default_state = harmony_state_from_kpdve(kpdveInteger);

    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                for (int l = 1; l < 7; l++)
                {
                    for (int m = 0; m < 7; m++)
                    {
                        kpdve[0] = i;
                        kpdve[1] = j;
                        kpdve[2] = k;
                        kpdve[3] = l;
                        kpdve[4] = m;

                        kpdveInteger = KPDVEtoBinaryEncoding(kpdve);
                        adjust_harmony_state_from_kpdve(&default_state, kpdveInteger);

                        printAnalysisSummary(&default_state);
                        printf("\n");
                    }
                }
            }
        }
    }
}

/**
 * @brief Continuously generates random binary values and analyzes their harmony state.
 *
 * This function generates random 12-bit numbers with up to 5 bits set to 1,
 * adjusts the harmony state based on the generated number and a context value,
 * and prints the analysis summary of the resulting harmony state.
 */
void continuous_binary_test()
{
    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    harmony_state continuous_state = harmony_state_default();
    int context = 35; // F major 7 KPDVE
    float pctValid = 0.5; // Percentage of valid states
    int numValidStates = 0;
    int totalStates = 0;
    int random_number = 0;
    int bits_set = 0;
    int chordMax = 3;

    while (totalStates < 200) // or Infinite loop for continuous testing
    {
        // Generate a random 12-bit number with up to 5 bits set to 1
        random_number = 0;
        bits_set = 0;

        while (bits_set < chordMax) {
            int bit_position = rand() % 12; // Random position between 0 and 11
            random_number |= (1 << bit_position); // Set the bit
            bits_set++;
        }

        adjust_harmony_state_from_chroma_and_context(&continuous_state, random_number, context);
        context = continuous_state.kpdve;
        if (continuous_state.kpdve_list_length > 0)
        {
            numValidStates++;
        }
        // Print the values of the harmony_state struct
        printAnalysisSummary(&continuous_state);

        totalStates++;
        pctValid = (float)numValidStates / totalStates;
        // printf("Pct valid: %.3f%%", pctValid * 100);
        // printf(" ");
        printf("\n");
    }
 
}

int main()
{
    // Call the harmony_state_default function
    //majorTriadSequenceModulatingUp();
    //majorTriadSequenceModulatingUpFifths();
    //majorTriadSequenceModulatingDown();
    scrollBinaryValues();
    scrollKPDEvalues();
    continuous_binary_test();
    majorTriadSequenceModulatingDownFifths();
    majorTriadSequenceModulatingDown();
    majorTriadSequenceModulatingUpFifths();
    majorTriadSequenceModulatingUp();
    return 0;
}

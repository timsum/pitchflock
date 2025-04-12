# pitchflock

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](../LICENSE)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](#)

## Project Title
pitchflock - An algorithm for analyzing harmony from chroma input. Naive, but fluid.

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Project Structure](#project-structure)
6. [Key Components](#key-components)
7. [Contributing](#contributing)
8. [License](#license)
9. [Contact](#contact)

## Introduction
*pitchflock* is in most ways a standard Mod12 chroma-based algorithm for analyzing harmonic relationships. It can be useful for parametrically navigating harmony or for analyzing audio signals. It is unique in that it relies on regular subdivisions of prime-numbered groups of bits to maintain longer-term memory of key area. This allows for flipped bits and non-harmonic tones to occur without disrupting the core orientation.

At its center is a struct called 'harmony_state' which contains two types of information, stored as two integers: 1) five values representing Key, Pattern, Degree, Voicing, and Extensions (encoded bitwise as KKKKPPPDDDVVVEEE), where K,P,D,V, and E have maximum values of 12, 7, 7, 7, and 7, respectively; 2)  a chroma representation of the notes (backwards, like a piano in Hebrew: b-a-g-fe-d-c, where the hyphens are 'black keys').

For efficiency, the notes and their complete analysis can be combined into a 32-bit 'encoded_state', which contains the complete analysis:

```bash
x---KKKKPPPDDDVVVEEEb-a-g-fe-d-c 
00001011110110100010010010001000
```

The leftmost bit (x) is a 1 if the state is non-harmonic (in this case the harmony state will not change its analysis, because it cannot establish sufficient consonance to change the listener's orientation). The other three bits (---) are empty, but may come into use. For now it is convenient to package a chord and its analysis in a single 32-bit integer.

*Either* KPDVE information or chroma information can serve as input. The chroma input yield an analysis in terms of functional harmony; the functional harmony yields the exact chord it names.

However, while a single KPDVE (functional harmony) value will yield a single chord, a single chord or note can play many harmonic roles. So the KPDVE output is almost always multiple, and the system must settle upon the most likely solution (chosen from a 'kpdve_list' by proximity to the previous solution in a 12x7x7 modular KPD space).  So an 'answer' is by definition not definitive: a 'KPDVE list' contains all possible chord functions, some of which might prove more correct in retrospect than the original best choice. To take a simple example: a C major chord can be I of C major or V of F major. At any moment a major triad could play any one of 18 possible functions, and if it turns out to have resolved to F7, then it will in retrospect turn out to have been V/V in Bb The simplest version of this algorithm simply finds the element of the list closest to the previous value in KPD-space, so it proceeds more or less like a markov chain. Further iterations may incorporate harmonic hindsight.

The net result is that consonance and harmonic direction become functions of bit entropy. The fundamental technique is to treat bits as powers of three rather than powers of two. This yields a kind of information harmony.

## Features
- The algorithm is extraordinarily low-latency, and can function fluidly for live audio; it also can react to live-input discrete KPDVE values, for music composition; since it operates like a very tiny neural network tuned for harmony, and because it maintains memory ('attention') in its K and P values, it may offer a stabilizing force in artificial intelligence audio applications. 

Maybe. 

- In this scheme, the harmonic analysis creates a kind of dynamic tokenization, in which the name (a binary representation of pitches and function) is not absolute in its determination, but still remains centered on a meaningful musical memory and prediction area.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/timsum/pitchflock.git
   ```
2. Navigate to the project directory:
   ```bash
   cd pitchflock
   ```
3. Makefile:
   ```bash
   make
   ```
4. Run test file (covers all possible inputs):
   ```bash
   ./build/test_harmony_state_default;
   ```
## Usage
1. The program will serve as a kernel for both music analysis and generation. Inputs can be parametric (Key, Pattern, Degree, Voicing, Extension), or Chroma.  The 'solution' consists in joining the two.

I will use it for harmony-responsive music visualization.

## Project Structure
- **include/**: Contains header files for core functionality, such as harmony analysis, state management, and naming conventions.
- **src/**: Contains implementation files for the algorithms and logic defined in the headers.
- **README.md**: Documentation for the project.

## Key Components
- **Harmony State Management**: (`harmony_state.h`) Defines the structure and logic for managing harmony states.
- **Harmony Crystal**: (`qdkpdve_harmonycrystal.h`) Provides tools for analyzing harmonic patterns using twin-prime-numbered sets of bits (e.g. 7 and 5)
- **KPDVE Analysis**: (`qdkpdve_analysis.h`) Implements algorithms for analyzing and minimizing harmonic values.
- **Naming Conventions**: (`qdkpdve_naming.h`) Maps harmonic values to a set of conventional musical names and patterns.
- **State Maker**: (`qdkpdve_statemaker.h`) Handles the creation and adjustment of harmony states. Most analysis takes place here.

## Building the Project
To build the library and test programs, run:
```bash
make
```

## Running Tests
To build and run the test programs:
```bash
make tests
./build/test_harmony_state_default
```
Generates analyses of: 1) all possible chroma inputs (4096) 2) all possible KPDVE inputs (28812)3) a stream of 200 random numbers

In the functions of 'test_harmony_state_default.c', you can see some examples of how information is retrieved from the harmony state, and how to create and adjust harmony_state structs.

## Cleaning Up
To clean up build artifacts:
```bash
make clean
```

## Contributing
We welcome contributions! Please follow these steps:
1. Fork the repository:
   - Open the repository on GitHub and click the "Fork" button in the top-right corner.
2. Clone your forked repository:
   ```bash
   git clone https://github.com/<your-username>/pitchflock.git
   ```
3. Create a new branch:
   ```bash
   git checkout -b feature-name
   ```
4. Commit your changes:
   ```bash
   git commit -m "Add feature-name"
   ```
5. Push to your branch:
   ```bash
   git push origin feature-name
   ```
6. Open a pull request:
   - Go to your forked repository on GitHub.
   - Click the "Pull Request" tab and then "New Pull Request."
   - Select the base repository and branch, and submit your pull request.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contact
For questions or feedback, please contact:
- Email: timsummers@mac.com
- GitHub: [timsum]
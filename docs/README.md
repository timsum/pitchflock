# PitchFlock

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](../LICENSE)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](#)

## Project Title
PitchFlock - An algorithm for analyzing harmony from chroma input. Naive, but fluid.

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
PitchFlock is in most ways a standard Mod12 chroma-based algorithm for analyzing harmonic relationships in audio signals. It is unique in that it relies on regular subdivisions of prime-numbered groups of bits to maintain longer-term memory of key area. This allows for flipped bits and non-harmonic tones to occur without disrupting the core orientation.

At its center is a struct called 'harmony_state' which contains two types of information, as two integers: five values representing Key, Pattern, Degree, Voicing, and Extensions encoded as a single int (KKKKPPPDDDVVVEEE), and a chroma representation of the notes (backwards, like a piano in Hebrew: B-A-G-FE-D-C, where the hyphens are 'black keys'), also a single int.

Either of these can be input: either the notes yield an analysis in terms of functional harmony, or the functional harmony yields the chord it names.

While a single KPDVE value will yield a single chord, a single chord or note can play manyharmonic roles. So the KPDVE output is almost always multiple, and the system must settle upon the most likely solution (closest in a 12x7x7 modular KPD space).  This 'answer, however, is not definitive, a 'KPDVE list' contains all alternate answers. To take a simple example: a C major chord can be I of C major or V of F major. At any moment it could *also* be the other. The simplest version of this algorithm simply finds the element of the list closest to the previous value, so it proceeds more or less like a markov chain.

The net result is that consonance is a function of bit entropy. The fundamental technique is to treat bits as powers of three rather than powers of two. This yields a kind of information harmony.

## Features
- The algorithm is extraordinarily low-latency, and can function fluidly for live audio; it also can react to live-input discrete KPDVE values, for music composition; since it operates like a very tiny neural network and maintains memory in its K value, it may offer a stabilizing force in artificial intelligence audio applications.

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
3. Install dependencies:
   ```bash
   npm install
   ```

## Usage
1.

## Project Structure
- **include/**: Contains header files for core functionality, such as harmony analysis, state management, and naming conventions.
- **src/**: Contains implementation files for the algorithms and logic defined in the headers.
- **README.md**: Documentation for the project.

## Key Components
- **Harmony State Management**: (`harmony_state.h`) Defines the structure and logic for managing harmony states.
- **Harmony Crystal**: (`qdkpdve_harmonycrystal.h`) Provides tools for analyzing harmonic patterns using twin-prime-numbered sets of bits (e.g. 7 and 5)
- **KPDVE Analysis**: (`qdkpdve_analysis.h`) Implements algorithms for analyzing and minimizing harmonic values.
- **Naming Conventions**: (`qdkpdve_naming.h`) Maps harmonic values to musical names and patterns.
- **State Maker**: (`qdkpdve_statemaker.h`) Handles the creation and adjustment of harmony states.

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

Generates a stream of random numbers to test the analysis capacities using chroma (chroma are here entered Right to Left, like hebrew: b a g fe d c)
```

## Cleaning Up
To clean up build artifacts:
```bash
make clean
```

## Contributing
We welcome contributions! Please follow these steps:
1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add feature-name"
   ```
4. Push to your branch:
   ```bash
   git push origin feature-name
   ```
5. Open a pull request.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contact
For questions or feedback, please contact:
- Email: timsummers@mac.com
- GitHub: [timsum]
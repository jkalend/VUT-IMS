# VUT-IMS

A discrete-event simulation model for analyzing SpaceX's Mars logistics operations, focusing on the transportation of materials and supplies to Mars using the Starship spacecraft system.

## Overview

This project simulates the complete logistics chain for establishing a Mars colony, from rocket assembly and launch authorization through orbital refueling to interplanetary transit. The model evaluates various operational parameters including fleet size, launch pad capacity, and tanker availability to optimize mission duration and resource utilization.

## Key Features

- **Multi-stage Rocket Simulation**: Models the complete lifecycle of Starship missions including booster separation, orbital operations, and interplanetary transit
- **Resource Management**: Tracks availability of Starships, Super Heavy Boosters, launch pads, and tanker spacecraft
- **Mars Transit Windows**: Incorporates realistic 780-day synodic period constraints for optimal launch timing
- **Orbital Refueling**: Simulates the complex tanker operations required to refuel Starships in Earth orbit (4-8 tanker missions per Starship)
- **Configurable Parameters**: Supports experimentation with fleet sizes, infrastructure capacity, and payload requirements

## Technology Stack

- **Language**: C++17
- **Simulation Framework**: SIMLIB/C++ (SIMulation LIBrary for C++)
- **Build System**: GNU Make

## Building and Running

### Compilation
```bash
make
```

### Execution
```bash
make run
```

### Custom Configuration
```bash
make run ARGS="-s <starships> -b <boosters> -l <launchpads> -t <tankers> -p <payload> [-S <initial_starships>] [-B <initial_boosters>]"
```

**Parameters:**
- `-s`: Number of Starships to manufacture
- `-b`: Number of Super Heavy Boosters to manufacture
- `-l`: Number of launch pads available
- `-t`: Number of tanker spacecraft
- `-p`: Total payload size in tons (rounded to nearest 100)
- `-S`: Initial number of pre-built Starships (optional)
- `-B`: Initial number of pre-built Boosters (optional)

### Example
```bash
make run ARGS="-s 50 -b 20 -l 3 -t 3 -p 10000"
```

## Simulation Model

The simulation implements a queuing system with the following key processes:

- **RocketProcess**: Handles rocket assembly, FAA authorization, launch, orbital refueling, and Mars transit
- **StarshipProcess**: Manages Starship manufacturing and availability
- **BoosterProcess**: Manages Super Heavy Booster manufacturing and reuse
- **TankerProcess**: Handles tanker operations for orbital refueling
- **LaunchPadProcess**: Manages launch pad availability and operations
- **FAAProcess**: Simulates Federal Aviation Administration launch authorization (3 weeks exponential)
- **MarsProcess**: Periodically enables Mars transit windows (every 780 days)

## Key Findings

Based on extensive experimentation, the simulation demonstrates that:
- Delivering 10,000 tons of payload to Mars requires a minimum of approximately 2.4 years
- Optimal configuration includes 3 launch pads and 3 tankers for efficient operations
- Increasing Starship count significantly reduces mission duration, while additional boosters provide diminishing returns
- The primary bottleneck is the Mars transit window frequency rather than manufacturing capacity

## Authors

- Andrea (Jan) Kalenda (xkalen07)
- Tereza Kubincova (xkubin27)

## Documentation

For detailed analysis, experimental results, and model validation, see `documentation.pdf`.

# UDP Raw Socket Simulation with NS3

This project implements a UDP raw socket simulation using NS3, with custom classes defined in the `sim` namespace. The NS3 library is included as a Git submodule in `third_party/ns3/`, and all source code is organized under `src/`. The project uses CMake for building.

---

## Prerequisites

* Git
* CMake (version 3.10 or higher)
* A C++23-compatible compiler
* NS3 dependencies (e.g., `libgtk-3-dev`, `libxml2-dev` on Ubuntu; see NS3 documentation for details)

To install dependencies on Debian-based Linux:

```sh
sudo apt update
sudo apt install build-essential cmake libgtk-3-dev libxml2-dev python3-dev ninja-build git
```

---

## Directory Structure

```
udp-raw-socket-sim/
├── CMakeLists.txt
├── README.md
├── configure.sh
├── src/
│   ├── main.cc
│   ├── RawUdpSender.hpp
│   ├── RawUdpSender.cc
│   ├── RawUdpReceiver.hpp
│   └── RawUdpReceiver.cc
└── third_party/
    └── ns3/

```

---

## Setup Instructions

### Step 1: Clone the Project

If cloning an existing repository:

```sh
git clone --recurse-submodules https://github.com/gabrielyanabraham/NS3-Usage-Example
cd udp-raw-socket-sim
```

### Step 2: Ensure NS3 Submodule is Initialized

The NS3 submodule should be in `third_party/ns3/`. If it’s not initialized:

```sh
git submodule update --init --recursive
```

### Step 3: Configure NS3

Run the provided `configure.sh` script:

```sh
chmod +x configure.sh
./configure.sh
```

This script:

* Updates the NS3 submodule.
* Configures and builds NS3 using CMake in `third_party/ns3/build/`.

---

## Build Instructions

### Method 1: Traditional Build

```sh
mkdir build
cd build
cmake ..
make
```

### Method 2: Out-of-Source Build

```sh
cmake -B build -S .
cmake --build build
```

Both methods generate an executable in the `build/` directory:

* `udp-raw-socket-sim`: Main simulation

---

## Run Instructions

### Run the Main Simulation

```sh
cd build
./udp-raw-socket-sim
```

This runs a simulation with 6 nodes (`n0` to `n5`), where `n0`, `n4`, and `n5` send UDP packets to `n0`, `n1`, and `n5`, respectively, over a CSMA network with bridges.

### Analyze Traffic

PCAP files (`udp-raw-socket-*.pcap`) are generated in the `build/` directory. Use Wireshark to inspect them:

```sh
wireshark udp-raw-socket-0-0.pcap
```

---

## Network Setup

- The link between n2 and n3 operates at 1.5 Mbps with 20 ms latency, while other links use 10 Mbps with 2 ms latency.

---

## Troubleshooting

### Missing Dependencies

Ensure NS3 dependencies are installed (see Prerequisites).

### Build Errors

If CMake or linker errors occur, verify that `configure.sh` completed successfully and NS3 libraries are in `third_party/ns3/build/lib/`.

---

## Notes

* The simulation generates console output showing received packets and their sizes/timestamps.

---

For issues or further customization, refer to the NS3 documentation or modify the source files in `src/`.

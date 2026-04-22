# Custom Radix-2 FFT & IFFT Engine

A lightweight, dependency-free C++ library for Fast Fourier Transform (FFT) and Inverse Fast Fourier Transform (IFFT) calculations, designed from scratch with a focus on memory efficiency for embedded systems.

## 🚀 Engineering Highlights
* **Zero External Dependencies:** Built entirely with standard C++ without relying on external math/DSP libraries like `numpy` or `fftw`.
* **Custom Complex Math:** Includes a from-scratch implementation of a `ComplexNum` class and dynamic `SignalVector` memory management.
* **In-Place Calculation:** Utilizes the Cooley-Tukey Radix-2 Decimation-in-Time (DIT) algorithm, ensuring $O(1)$ extra memory complexity. Ideal for memory-constrained microcontrollers (e.g., motor drivers, Arduino).
* **Hardware-Level Optimization:** Implements custom bit-reversal algorithms for initial signal reordering, minimizing CPU cycles during butterfly operations.
* **Python Visualization Pipeline:** Seamlessly exports processed frequency/time domain data to a `.csv` format, automatically visualized via a dedicated Matplotlib pipeline.

## 📁 Project Architecture
* `dijital_sinyal_isleme_kutuphanesi.cpp` : The core DSP engine containing `FFT`, `ComplexNum`, and `SignalVector` classes.
* `example_usage.cpp` : The main driver code that generates a test signal (e.g., 1Hz main wave + 3Hz harmonic noise), computes FFT/IFFT, and exports results.
* `plotter.py` : The Python script that parses the C++ output and plots the Time Domain and Frequency Spectrum graphs.

## 📊 Results & Visualization
*(Add your generated graph image here. For example: `![FFT Result](fft_grafik.png)`)*

As seen in the frequency spectrum, the algorithm successfully extracts the main 1Hz signal and the 3Hz harmonic noise, calculating exact magnitudes without spectral leakage. The IFFT engine also reconstructs the exact original signal with zero data loss.

## ⚙️ How to Build and Run
**1. Compile the C++ Engine:**
```bash
g++ example_usage.cpp -o dsp_test

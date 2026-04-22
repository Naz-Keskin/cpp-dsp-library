#include <iostream>
#include <cmath>
#include <fstream>

// Kütüphane dosyanı buraya dahil ediyorsun
#include "radix2_fft.cpp"

int main()
{
    SignalVector time_signal;

    // Generate a mixed signal: 1 Hz main wave + 3 Hz harmonic noise (N=8)
    for (int i = 0; i < 8; i++)
    {
        float t = i / 8.0f;

        // Main Signal (1 Hz, amplitude 1.0)
        float signal1 = std::sin(2.0f * M_PI * 1.0f * t);

        // Harmonic Noise (3 Hz, amplitude 0.5)
        float signal2 = 0.5f * std::sin(2.0f * M_PI * 3.0f * t);

        // Append to our signal vector (Eğer kütüphanede 'append' yaptıysan burayı 'append' olarak değiştir)
        time_signal.push(ComplexNum(signal1 + signal2, 0.0f));
    }

    // 1. Forward FFT (Time Domain -> Frequency Domain)
    SignalVector frequency_spectrum = FFT::computeFFT(time_signal);

    // 2. Inverse FFT (Frequency Domain -> Time Domain)
    SignalVector reconstructed_signal = FFT::computeIFFT(frequency_spectrum);

    // Print reconstructed values to verify the algorithm
    std::cout << "--- SIGNAL RECONSTRUCTED FROM IFFT ---\n";
    for (size_t i = 0; i < reconstructed_signal.size(); i++)
    {
        std::cout << "Sample " << i << ": " << reconstructed_signal[i].real() << "\n";
    }

    // 3. Export to CSV for Python Visualization
    std::ofstream file("fft_results.csv");

    // Write CSV Headers
    file << "Sample,Time_Amplitude,Freq_Magnitude\n";

    // Write data rows
    for (size_t i = 0; i < time_signal.size(); i++)
    {
        file << i << ","
             << time_signal[i].real() << ","
             << frequency_spectrum[i].magnitude() << "\n";
    }

    file.close();
    std::cout << "\nSuccess! Data exported to 'fft_results.csv' for visualization.\n";

    return 0;
}
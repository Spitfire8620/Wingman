#include <csignal>
#include <initializer_list>
#include <iostream>
#include <cmath>
#include <vector>
#include <armadillo>

// Notch filter to filter out powerline frequency, filters out 50HZ and 60HZ.
arma::vec notchFilter(const arma::vec& signal, double sampleRate, double humFrequency, double bandwidth) {
    // Compute the frequency response of the notch filter
    arma::vec frequencies = arma::linspace<arma::vec>(0, sampleRate, signal.n_elem);
    arma::cx_vec transferFunction(signal.n_elem, arma::fill::ones);

    for (size_t i = 0; i < frequencies.n_elem; ++i) {
        if (std::abs(frequencies(i) - humFrequency) < bandwidth / 2) {
            transferFunction(i) = 0; // Apply notch filter
        }
    }

    // Apply the frequency response to the signal using the Fourier transform
    arma::cx_vec fft_signal = arma::fft(signal);
    arma::cx_vec filtered_fft_signal = fft_signal % transferFunction;

    // Inverse transform the filtered signal to obtain the time-domain signal
    arma::vec filteredSignal = arma::real(filtered_fft_signal);

    return filteredSignal;
}

// Function to implement a low-pass filter with 'same' convolution behavior, filter out above 150HZ
arma::vec lowPassFilter(const arma::vec& signal, int windowSize) {
    int halfWindowSize = windowSize / 2;
    arma::vec window(windowSize, arma::fill::ones); // Define a window for averaging
    arma::vec filteredSignal(signal.n_elem, arma::fill::zeros); // Initialize filtered signal with zeros

    // Perform convolution with 'same' behavior
    for (int i = halfWindowSize; i < signal.n_elem - halfWindowSize; ++i) {
        // Extract the segment of the signal for convolution
        arma::vec signalSegment = signal.subvec(i - halfWindowSize, i + halfWindowSize);

        // Ensure window and signal segment have the same number of elements
        if (signalSegment.n_elem == window.n_elem) {
            filteredSignal(i) = arma::dot(signalSegment, window) / windowSize;
        }
        else {
            filteredSignal(i) = signal(i);
        }
    }

    return filteredSignal;
}

// Function to implement a high-pass filter to filter out below 20HZ
arma::vec highPassFilter(const arma::vec& signal, int windowSize) {
    arma::vec lowPassFiltered = lowPassFilter(signal, windowSize); // Apply low-pass filtering
    arma::vec highPassFiltered = signal - lowPassFiltered; // Subtract low-pass filtered signal from original signal
    return highPassFiltered;
}

//GESTURE CLASSES
enum Gesture {
    FIST,
    OPEN,
    TWO_FINGER_PINCH,
    THREE_FINGER_PINCH,
    POINTING,
    HOOK,
    THUMBS_UP,
    RING_FINGER_GRASP,
    NUM_GESTURES
};

//GESTURE STRUCTURE
struct GestureData {
    Gesture gesture;
    std::vector<double> emgSignal;
};

//GENERATES EMG SIGNAL (uses a matrix(MAT))
arma::mat generateEMGSignal(int numSample, Gesture gesture) {
    double mean = 1.0;
    double stddev = 1.0;

    arma::mat emgSignal(numSample, 1, arma::fill::zeros);
    switch (gesture) {
    case FIST:
        emgSignal.randn();
        break;
    case OPEN:
        emgSignal.randn();
        break;
    case TWO_FINGER_PINCH:
        emgSignal.randn();
        break;
    case THREE_FINGER_PINCH:
        emgSignal.randn();
        break;
    case POINTING:
        emgSignal.randn();
        break;
    case HOOK:
        emgSignal.randn();
        break;
    case THUMBS_UP:
        emgSignal.randn();
        break;
    case RING_FINGER_GRASP:
        emgSignal.randn();
        break;
    case NUM_GESTURES:
        break;
    }
    return emgSignal;
}

//Compute Root Mean Square of generate signal. RMS measures the power or energy in a signal. 
double computeRMS(const std::vector<double>& signal){
    double sumOfSquares = 0.0;
    for (double value : signal) {
    sumOfSquares += std::abs(value);
    }
    return std::sqrt(sumOfSquares / signal.size());
}

//Computes Mean Absolute Value of signal...
double computeMAV(const std::vector<double>& signal){
   double sumOfAbs = 0.0; 
    for (double value : signal) {
    sumOfAbs += std::abs(value);
    }
    return sumOfAbs / signal.size();
}

//Extract features from filtered EMG..
std::vector<double> extractFeatures(const std::vector<double>& filteredSignal) {
    std::vector<double> features;
    double rmsValue = computeRMS(filteredSignal);
    features.push_back(rmsValue);
    double mavValue = computeMAV(filteredSignal);
    features.push_back(mavValue);
    return features;
}

int main() {

    std::cout << "Loading Gestures....." << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Hand Gestures are classified as follows: " << std::endl; 
    std::cout << "----------------------" << std::endl;
    std::cout << "G1 = Fist" << std::endl;   
    std::cout << "G2 = Open" << std::endl;
    std::cout << "G3 = Two Finger Pinch" << std::endl;
    std::cout << "G4 = Three Finger Pinch" << std::endl;
    std::cout << "G5 = Pointing" << std::endl;
    std::cout << "G6 = Hook" << std::endl;
    std::cout << "G7 = Thumbs Up" << std::endl;
    std::cout << "G8 = Ring Finger Grasp" << std::endl; 
    std::cout << std::endl;
    std::cout << "Ready to generate EMGs....." << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "Please enter a sample size to generate an EMG signal for Wingman Gestures...." << std::endl;
    int numSample;
    std::cin >> numSample;
    std::cout << "Please enter a window size....." << std::endl;
    int windowSize;
    std::cin >> windowSize;
    std::cout << "------------------------" << std::endl;
    std::cout << "Please choose a gesture...." << std::endl;
//ALLOWS USER TO CHOOSE A GESTURE THEN GENERATE EMG...Using a switch statement allows us to efficiently map the user's input number to the appropriate gesture enum value. 
    int gestureNumber;
    std::cin >> gestureNumber;
    Gesture selectedGesture;
    switch(gestureNumber) {
        case 1:
            selectedGesture = FIST;
            break;
        case 2:
            selectedGesture = OPEN;
            break;
        case 3:
            selectedGesture = TWO_FINGER_PINCH;
            break;
        case 4:
            selectedGesture = THREE_FINGER_PINCH;
            break;
        case 5:
            selectedGesture = POINTING;
            break;
        case 6:
            selectedGesture = HOOK;
            break;
        case 7:
            selectedGesture = THUMBS_UP;
            break;
        case 8:
            selectedGesture = RING_FINGER_GRASP;
            break;
        default:
            std::cerr << "Invalid gesture number!" << std::endl;
            return 1; // Exit with error code
    }arma::rowvec extractFeatures(){}
     // Map gesture enum values to their corresponding names
    std::map<Gesture, std::string> gestureNames = {
        {FIST, "Fist"},
        {OPEN, "Open"},
        {TWO_FINGER_PINCH, "Two Finger Pinch"},
        {THREE_FINGER_PINCH, "Three Finger Pinch"},
        {POINTING, "Pointing"},
        {HOOK, "Hook"},
        {THUMBS_UP, "Thumbs Up"},
        {RING_FINGER_GRASP, "Ring Finger Grasp"}
    };

    //THIS GENERATES A EMG SIGNAL BASED ON USER INPUT OF SAMPLE SIZE, WINDOW SIZE, AND GESTURE SELECTION...
    arma::mat emgSignal = generateEMGSignal(numSample, selectedGesture);

    std::cout << "EMG Signal generated for " << gestureNames[selectedGesture] << " gesture:\n" << emgSignal << std::endl;
    std::cout << "Filtering EMG signal..." << std::endl;
    //FILTERS THE USERS GENERATED EMG SIGNALS...
    double sampleRate = 10; // If sample rate is too high, output will onyl be zeros..
    double humFrequency = 60;
    double bandwidth = 2;
    arma::vec filteredSignal = notchFilter(emgSignal.col(0), sampleRate, humFrequency, bandwidth);
    filteredSignal = lowPassFilter(filteredSignal, windowSize);
    filteredSignal = highPassFilter(filteredSignal, windowSize);
    std::cout << "Filtered EMG signal for " << gestureNames[selectedGesture] << " gesture\n" << filteredSignal << std::endl;
    std::cout << "Ready to extract features..." << std::endl;
    std::vector<double>featureVector = extractFeatures(filteredSignal);
    std::cout << "Extracted Features...." << std::endl;
    std::cout << "RMS: " << std::endl;
    std::cout << "MAV: " << std::endl;
    
    return 0;
}



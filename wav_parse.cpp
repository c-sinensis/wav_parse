// parse wav file using AudioFile library

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<cstdint>
#include<filesystem>
#include "AudioFile.h"

using namespace std;
using namespace std::filesystem;

// struct to hold audio file information
typedef struct audio_info{

    int sampleRate; // file sample rate
    int bitDepth; // file bit depth
    int samples; // number of samples per channel
    double duration; // length of file in seconds
    int channels; // number of channels (1=mono, 2=stereo)

} fileInfo;

// function prototypes
// obtain audio information
void getInfo(AudioFile<double> inAudio, fileInfo &info);

// output to file
void dataOut(fileInfo info, string inFilePath, AudioFile<double> inAudio);


// main
int main(){

    AudioFile<double> inAudio;
    
    // getting input
    string inFile;
    path filePath;
    do{
        filePath = current_path();
        cout << "Please be sure that your audio file is in the same directory as this program file. Enter the name of the audio input file: ";
        cin >> inFile;
        cout << inFile << endl; // echo input file name
    
        filePath /= inFile;
        cout << filePath << endl;
        if (!exists(filePath)){
            cout << "File not found. Please try again." << endl << endl;
        }
        else {
            cout << "File found successfully. Loading audio..." << endl;
        }
    } while(!exists(filePath));

    string inFilePath = filePath.string();
    bool loadGood = inAudio.load(inFilePath); // loading input file
    if (loadGood){
        cout << "File loaded successfully." << endl;
    }
    else {
        cout << "File loading error. Terminating program, please re-run and try again." << endl;
        return 1;
    }

    // getting file information
    fileInfo info;
    getInfo(inAudio, info);

    dataOut(info, inFilePath, inAudio);

    return 0;
}


// function definitions
void getInfo(AudioFile<double> inAudio, fileInfo &info){
    cout << "File Information" << endl;
    cout << "----------------" << endl;
    info.sampleRate = inAudio.getSampleRate();
    cout << "Sample Rate: " << info.sampleRate << endl;
    info.bitDepth = inAudio.getBitDepth();
    cout << "Bit Depth: " << info.bitDepth << endl;
    info.samples = inAudio.getNumSamplesPerChannel();
    cout << "Sample Count: " << info.samples << endl;
    info.duration = inAudio.getLengthInSeconds();
    cout << "Duration: " << info.duration << endl;
    info.channels = inAudio.getNumChannels();
    cout << "Channels: " << info.channels << endl;
    cout << "----------------" << endl;
}

void dataOut(fileInfo info, string inFilePath, AudioFile<double> inAudio){
    ofstream outFile;
    do{
        string outFileName;
        cout << "Enter the name of the output file: ";
        cin >> outFileName;
        cout << outFileName << endl; // echo

        outFile.open(outFileName.c_str());

        if (!outFile.good()){
            cout << "File opening Error. Please try again." << endl << endl;
        }
        else {
            cout << "File opened successfully" << endl;
        }

    } while (!outFile.good());

    // file header
    outFile << "Audio File Sample Data" << endl;
    outFile << "----------------------" << endl;
    outFile << "File Path: " << inFilePath;
    outFile << "Sample Rate: " << info.sampleRate;
    outFile << "Bit Depth: " << info.bitDepth;
    outFile << "Sample Count: " << info.samples;
    outFile << "File Duration: " << info.duration;
    outFile << "Number of Channels" << info.channels;
    outFile << "----------------------" << endl;

    outFile << "CH1" << endl;
    outFile << "---" << endl;

    // sample data
    for (int i = 0; i < info.samples; i++){
        outFile << inAudio.samples[0][i] << endl;
    }

    cout << endl << "Data output complete." << endl;
    outFile.close();
}

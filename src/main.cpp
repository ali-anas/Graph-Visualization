/*
 * TODO: complete this file comment.
 */
#include <iostream>
#include <fstream>
#include <string>
#include "SimpleGraph.h"

using namespace std;

void Welcome();
void openFileAndSetStream(ifstream& stream, const string& prompt = "Enter file name: ",
               const string& repromt = "Unable to open that file... Try again.");
void readGraph(SimpleGraph& graph, ifstream& stream);

unsigned int readSimulationTime(const string& prompt = "Enter simulation time : ",
                                const string& reprompt = "Bad input!(should be > 0) Try again...");



// Main method
int main() {
    Welcome();
    /* TODO: your implementation here */

    // initialize graph object
    SimpleGraph graph;
    ifstream stream;
    openFileAndSetStream(stream);
    readGraph(graph, stream);

    unsigned int stime = readSimulationTime();


    // set initial positions of nodes

    // implement FDL algorithm

    // implement the code to take time of simulation

    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}



void openFileAndSetStream(ifstream& stream, const string& prompt,
               const string &reprompt) {
    while(true) {
        cout << prompt;
        string filename;
        if(!getline(cin, filename)) {
            throw domain_error("getline: End of input reached while waiting for line.");
        }

        if(!filename.empty()) {
            // ifstream istream(filename, ios::in);
            stream.clear();
            stream.open(filename);
            if (stream.is_open()) {
                return;
            }
        }

        cerr << reprompt << endl;
    }
}



#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "SimpleGraph.h"

using namespace std;

void Welcome();
void openFileAndSetStream(ifstream& stream, const string& prompt = "Enter file name: ",
               const string& reprompt = "Unable to open that file... Try again.");
void readGraph(SimpleGraph& graph, ifstream& stream);

unsigned int readSimulationTime(const string& prompt = "Enter simulation time : ",
                                const string& reprompt = "Bad input! (should be an integer >= 0)... Try again.");



// Main method
int main() {
    Welcome();

    // initialize graph object
    SimpleGraph graph;
    ifstream stream;
    openFileAndSetStream(stream);

    // read the graph from disk
    readGraph(graph, stream);

    // implement the code to take time of simulation
    unsigned int stime = readSimulationTime();


    // set initial positions of nodes

    // implement FDL algorithm



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
                cout << "Opening file stream... Success" << endl;
                return;
            }
        }

        cerr << reprompt << "\n";
    }
}


/*
struct Node {
  double x, y;
};

struct Edge {
  std::size_t start, end;
};

struct SimpleGraph {
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};
*/

void readGraph(SimpleGraph& graph, ifstream& stream) {
    unsigned int n;
    stream >> n;
    // set no of nodes of graph
    graph.nodes.resize(n);

    size_t start, end;
    while(stream >> start >> end) {
        graph.edges.push_back(Edge{start, end});
    }
    cout << "Loading graph...Success" << endl;
}


unsigned int readSimulationTime(const string& prompt, const string& reprompt) {
    while(true) {
        cout << prompt;
        string line; int result; char garbage;
        if(!getline(cin, line)) {
            throw domain_error("getline: End of input reached while waiting for line.");
        }
        istringstream iss(line);
        if(iss >> result && !(iss >> garbage) && !(result < 0)) {
            cout << "Reading simulation time... Success" << endl;
            return result;
        }

        cerr << reprompt << "\n";

    }
}

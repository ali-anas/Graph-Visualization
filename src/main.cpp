/*
 * Provided functions -
 *      InitGraphVisualizer(SimpleGraph)
 *          accepts a SimpleGraph, sets up the internal state of the visualizer,
 *
 *      DrawGraph(SimpleGraph)
 *          accepts a SimpleGraph and displays it on screen
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

#include "SimpleGraph.h"

const double PI = 3.14159265358979323;
const double K_REPEL = 0.001;
const double K_ATTRACT = 0.001;

using namespace std;

void Welcome();
void openFileAndSetStream(ifstream& stream, const string& prompt = "Enter file name: ",
               const string& reprompt = "Unable to open that file... Try again.");
void readGraph(SimpleGraph& graph, ifstream& stream);

unsigned int readSimulationTime(const string& prompt = "Enter simulation time(in sec) : ",
                                const string& reprompt = "Bad input! (should be an integer >= 0)... Try again.");

void setInitialNodePosition(SimpleGraph& graph);

void updateGraph(SimpleGraph& graph);

void applyAttractiveForcesAndUpdateDelta(SimpleGraph& graph,vector<Node>& delta);
void applyRepulsiveForcesAndUpdateDelta(SimpleGraph& graph,vector<Node>& delta);

bool promptUserForMoreVisualization(const string& prompt = "Do you want to visualize another graph? (Y/N): ",
                                    const string& reprompt = "Can't get your choice... Try again.");

// Main method
int main() {
    Welcome();

    bool is_to_visualize_more = false;
    do {
        // initialize graph object
        SimpleGraph graph;
        ifstream stream;
        openFileAndSetStream(stream);

        // read the graph from disk
        readGraph(graph, stream);

        // visualize graph
        InitGraphVisualizer(graph);     // from SimpleGraph.h

        // set initial positions to nodes
        setInitialNodePosition(graph);

        // First Draw initial graph then after ask for simulation time.
        DrawGraph(graph);   // from SimpleGraph.h

        // implement the code to take time of simulation
        unsigned int stime = readSimulationTime();

        cout << "Visualizing graph..." << endl;

        // simulate graph for time- stime
        // implement FDL algorithm
        time_t startTime = time(NULL);
        while(difftime(time(NULL), startTime) < stime) {
            updateGraph(graph);
            DrawGraph(graph);
        }

        cout << "Visualization Completed..." << endl;

        is_to_visualize_more = promptUserForMoreVisualization();
    } while(is_to_visualize_more);


    cout << "Exiting Normally... Press ctrl+c to close the window." << endl;

    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << "Please decide simulation time appropraitely, some" << endl;
    cout << "graphs structure takes 1-5 sec to get into better shape" << endl;
    cout << "while some may take 25-30 even 60-80 sec(ex-10grid) to get" << endl;
    cout << "into better shape." << endl;
    cout << endl;
}

/*
 * prompt user if he wants to visualize another graph or wants to exit.
 */

bool promptUserForMoreVisualization(const string& prompt, const string& reprompt) {

    while(true) {
        cout << prompt;
        string line;
        char choice; char trash;

        if(!getline(cin, line)) {
            throw domain_error("getline: End of input reached while waiting for line.");
        }

        istringstream iss(line);
        if(iss >> choice && !(iss >> trash)) {
            if(choice=='y' || choice=='Y') {
                return true;
            }
            if(choice == 'N' || choice == 'n') {
                return false;
            }
        }
        cerr << reprompt << "\n";
    }

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
 * From SimpleGraph.h
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

/*
 * Read simulation time of algorithm
 *      must be positive integer
 */

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
            return static_cast<unsigned int>(result);
        }

        cerr << reprompt << "\n";

    }
}

/*
 * set initial x and y co-ordinate of each node at pos k as
 * (cos(2*pi*k / n), sin(2*pi*k / n))
 */

void setInitialNodePosition(SimpleGraph& graph) {
    size_t num_of_nodes = graph.nodes.size();

    for(size_t node_pos = 0; node_pos < num_of_nodes; node_pos++) {
        graph.nodes[node_pos].x = cos((2 * PI * node_pos) / num_of_nodes);
        graph.nodes[node_pos].y = sin((2 * PI * node_pos) / num_of_nodes);
    }
}

/*
 * Update delta after calculating repulsive force on each node
 * due to every other node
 */
void applyRepulsiveForcesAndUpdateDelta(SimpleGraph& graph,vector<Node>& delta) {
    for(size_t i = 0; i < delta.size(); i++) {
        for(size_t j = 0; j < delta.size(); j++) {
            if(i != j) {
                Node first = graph.nodes[i];
                Node second = graph.nodes[j];
                double force_repel = K_REPEL / sqrt(pow(second.y - first.y,2) + pow(second.x - first.x, 2));
                double theta = atan2(second.y - first.y, second.x - first.x);

                delta[i].x -= force_repel * cos(theta);
                delta[i].y -= force_repel * sin(theta);
                delta[j].x += force_repel * cos(theta);
                delta[j].y += force_repel * sin(theta);
            }
        }
    }
}

/*
 * Update delta after calculating attractive force on each node
 * due to every other node connected with common edge
 */
void applyAttractiveForcesAndUpdateDelta(SimpleGraph& graph,vector<Node>& delta) {

    for(size_t i = 0; i < graph.edges.size(); i++) {
        Edge e = graph.edges[i];
        Node first = graph.nodes[e.start];
        Node second = graph.nodes[e.end];

        double force_attract = K_ATTRACT * (pow(second.y - first.y,2) + pow(second.x - first.x, 2));
        double theta = atan2(second.y - first.y, second.x - first.x);

        delta[e.start].x += force_attract * cos(theta);
        delta[e.start].y += force_attract * sin(theta);
        delta[e.end].x -= force_attract * cos(theta);
        delta[e.end].y -= force_attract * sin(theta);
    }
}



/*
 * compute resultant delta by applying attractive forces and repulsive forces
 * to each node by every other node
 * resultant of forces might change x & y component of each node
 */
void updateGraph(SimpleGraph& graph) {
    vector<Node> delta(graph.nodes.size());

    applyRepulsiveForcesAndUpdateDelta(graph, delta);
    applyAttractiveForcesAndUpdateDelta(graph, delta);

    // update position of each node according to delta
    for(size_t i = 0; i < graph.nodes.size(); i++) {
        graph.nodes[i].x += delta[i].x;
        graph.nodes[i].y += delta[i].y;
    }
}

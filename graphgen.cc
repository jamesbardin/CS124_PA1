#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <cmath>
#include <omp.h>
#include <pcg_random.hpp>
#include <random>

using namespace std;

struct Edge {
    uint32_t u;       
    uint32_t v;       
    float weight;
};

struct Vertex {
    uint32_t id;      
    uint32_t parent;   
    uint32_t rank;
};

struct Graph {
    vector<Edge> edges;
    vector<Vertex> vertices;
};

double pcg32_random() {
    static thread_local pcg32 rng((unsigned int)time(NULL));
    return static_cast<double>(rng()) / static_cast<double>(std::numeric_limits<uint32_t>::max());
}


float get_cutoff(int n) {
    switch (n){
        case 0 ... 2048:
            return 1;
        case 2049 ... 16384:
            return 0.01;
        case 16385 ... 65536:
            return 0.001;
        case 65537 ... 131072:
            return 0.0001;
        case 131073 ... 262144:
            return 0.00001;
        default:
            cerr << "N not within project specs.";
            return NAN;
    }
}



Graph generate_graph_0(int n) {
    Graph graph;
    // graph.vertices = new Vertex[n];
    // graph.edges = new Edge[n * (n - 1) / 2];
    graph.vertices.resize(n);
    graph.edges.reserve(n * (n - 1) / 2);
    
    thread_local std::mt19937 rng((unsigned int)time(NULL) + omp_get_thread_num());
    thread_local std::uniform_real_distribution<float> weight_dist(0.0f, 1.0f);
    thread_local std::uniform_int_distribution<uint32_t> vertex_dist(0, n - 1);

    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        graph.vertices[i] = {
            i, 
            i, 
            0
        };
    }
    cout << "vertices: " << graph.vertices.size();


    // generate (n*CUTOFF) weights, and assign to random sel. of nodes
    // function to pick n*CUTOFF pairs of random vertices, no repeat pairs (x,y) = (y,x)

    // float CUTOFF = 0.1
    // for (i = i; (n * CUTOFF) < i; i++){

    // }


    // BAD 
    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = i+1; j < n; j++) {
            Edge e;
            e.u = i;
            e.v = j;
            e.weight = weight_dist(rng);
            float cutoff = get_cutoff(n);
            if (e.weight < cutoff) {
                #pragma omp critical
                {
                    graph.edges.push_back(e);
                }
            }
        }
    }
    cout << " | edges: " << graph.edges.size() << "  ";

    return graph;
    
}



int main(int argc, char* argv[]) {
    // seed for random gen.
    srand(time(NULL));
    int flag = atoi(argv[1]);
    int n = atoi(argv[2]);
    int num_trials = atoi(argv[3]);
    int dimension = atoi(argv[4]);
    double total_weight = 0;

    switch (dimension) {
        case 0:
            for (int i = 0; i < num_trials; i++) {
                Graph graph = generate_graph_0(n);
                // total_weight += ??mst_alg??
            }
            break;
        // case 2:
        //     for (int i = 0; i < num_trials; i++) {
        //         Graph graph = generate_graph_2(n);
        //         // total_weight += ??mst_alg??
        //     }
        //     break;
        // case 3:
        //     for (int i = 0; i < num_trials; i++) {
        //         Graph graph = generate_graph_3(n);
        //         // total_weight += ??mst_alg??
        //     }
        //     break;
        // case 4:
        //     for (int i = 0; i < num_trials; i++) {
        //         Graph graph = generate_graph_4(n);
        //         // total_weight += ??mst_alg??
        //     }
        //     break;
        default:
            std::cerr << "dimension must be 0, 2, 3, or 4." << std::endl;
            break;
    }
    // Debugging ->
    // int k, j;
    // for(k = 0; k < 10; k++) {
    //     for(j = 0; j < 10; j++) {
    //         cout << graph[k][j] << " ";
    //     }
    //     cout << endl;
    // }
    // double avg_weight = total_weight / num_trials;
    // cout << avg_weight << endl;
    
    return 0;
}
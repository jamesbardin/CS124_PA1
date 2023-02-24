#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <cmath>
#include <omp.h>
#include <pcg_random.hpp>
#include <random>
#include <unordered_set>
#include <algorithm>

using namespace std;

class UnionFind {
public:
    vector<int> parent, size;
    UnionFind(int n) {
        parent.resize(n);
        size.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }
    int find(int x) {
        if (parent[x] == x) {
            return x;
        }
        return parent[x] = find(parent[x]);
    }
    void union_sets(int x, int y) {
        x = find(x);
        y = find(y);
        if (x != y) {
            if (size[x] < size[y]) {
                swap(x, y);
            }
            parent[y] = x;
            size[x] += size[y];
        }
    }
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

struct Edge {
    uint32_t u;       
    uint32_t v;       
    float weight;
};

struct Vertex {
    float vertex[4];
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

    if (n < 2048) {
        return 1;
    }
    else if (n > 131072) {
        return 0.00001;
    }
    else if (n > 65536) {
        return 0.0005;
    }
    else if (n > 16384) {
        return 0.0009;
    }
    else if (n > 2048) {
        return 0.1;
    }
    else {
        return NAN;
    }
}

float get_threshold(int n, int dim) {
    if (dim == 0) {
        return (100.0)/(n-1);
    }
    else if (dim == 2) {
        return 2.5*pow(n, -1.0/2.0);
    }
    else if (dim == 3) {
        return 3*pow(n, -1.0/3.0);
    }
    else if (dim == 4) {
        return 2.5*pow(n, -1.0/4.0);
    }
    else {
        return NAN;
    }
}


Graph generate_graph_0(long long n) {
    Graph graph;

    float cutoff = get_threshold(n, 0);
    long long num_edges = cutoff * n*(n-1)*0.5;
    // cout << " Num edges: " << num_edges << cutoff;
    
    graph.vertices.resize(n);
    graph.edges.reserve(num_edges);
    
    default_random_engine generator;
    uniform_real_distribution<float> weight_dist(0.0f, 1.0f);
    uniform_int_distribution<uint32_t> vertex_dist(0, n - 1);

    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        graph.vertices[i].id = i;
        graph.vertices[i].parent = i; 
        graph.vertices[i].rank = 0;
    }
    cout << "vertices: " << graph.vertices.size();
    #pragma omp parallel for
    for (int i = 0; i < num_edges; i++) {
        uint32_t u = vertex_dist(generator);
        uint32_t v = vertex_dist(generator);
        while (u == v) {
            u = vertex_dist(generator);
            v = vertex_dist(generator);
        }
        Edge e = {u, v, weight_dist(generator)};
        #pragma omp critical
        graph.edges.push_back(e);
    }
    cout << " | edges: " << graph.edges.size() << "  ";
    return graph;
}

float calc_dist(float* v1, float* v2, int dim, float cutoff) {
    float dist = 0;
    for (int i = 0; i < dim; i++) {
        dist += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    float cutsq = cutoff * cutoff;
    // cout << cutsq << " vs. " << dist;

    if (dist <= cutsq) {
        // cout << sqrt(dist);
        return sqrtf(dist);
    }
    else {
        // cout << "here";
        return -1;
    }
}

Graph generate_graph_2(long long n, int dim) {
    Graph graph;

    float cutoff = get_threshold(n, dim);
    // cout << cutoff;

    long long num_edges = cutoff * n*(n-1)*0.5;
    // cout << " Num edges: " << num_edges;

    graph.vertices.resize(n);
    graph.edges.reserve(num_edges);
    
    default_random_engine generator;
    uniform_real_distribution<float> coord_dist(0.0f, 1.0f);
    uniform_int_distribution<uint32_t> vertex_dist(0, n - 1);

    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++) {
        graph.vertices[i].vertex[0] = coord_dist(generator);
        graph.vertices[i].vertex[1] = coord_dist(generator);
        if (dim == 3) {
            graph.vertices[i].vertex[2] = coord_dist(generator);
        }
        if (dim == 4) {
            graph.vertices[i].vertex[3] = coord_dist(generator);
        }
        graph.vertices[i].id = i;
        graph.vertices[i].parent = i; 
        graph.vertices[i].rank = 0;
    }

    #pragma omp parallel for collapse(2)
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = i+1; j < n; j++) {
            if (
                abs(graph.vertices[i].vertex[0] - graph.vertices[j].vertex[0]) < cutoff &&              
                abs(graph.vertices[i].vertex[1] - graph.vertices[j].vertex[1]) < cutoff
            ) {
                float dist = calc_dist(graph.vertices[i].vertex, graph.vertices[j].vertex, dim, cutoff);
                if (dist >= 0) {
                    Edge e = {i, j, dist};
                    #pragma omp critical
                    graph.edges.push_back(e);
                }
            }
        }
    }
    cout << "vertices: " << graph.vertices.size();
    cout << " | edges: " << graph.edges.size() << "  ";
    return graph;
}


bool edge_compare(const Edge& e1, const Edge& e2) {
    return e1.weight < e2.weight;
}

double kruskal_mst_weight(Graph& graph) {
    std::sort(graph.edges.begin(), graph.edges.end(), edge_compare);
    UnionFind uf(graph.vertices.size());

    double total_weight = 0.0;
    int num_edges_in_mst = 0;

    for (const auto& edge : graph.edges) {
        int u = edge.u;
        int v = edge.v;
        double weight = edge.weight;

        if (!uf.connected(u, v)) {
            uf.union_sets(u, v);
            total_weight += weight;
            num_edges_in_mst++;
        }

        if (num_edges_in_mst == graph.vertices.size() - 1) {
            bool is_connected = true;
            for (int i = 1; i < graph.vertices.size(); i++) {
                if (!uf.connected(0, i)) {
                    is_connected = false;
                    break;
                }
            }
            if (is_connected) {
                return total_weight;
            } else {
                return -1;
            }
        }
    }
    return -1;
}


int main(int argc, char* argv[]) {
    int flag = atoi(argv[1]);
    long long n = atoi(argv[2]);
    int num_trials = atoi(argv[3]);
    int dimension = atoi(argv[4]);
    double total_weight = 0;

    switch (dimension) {
        case 0:
            for (int i = 0; i < num_trials; i++) {
                Graph graph = generate_graph_0(n);
                total_weight += kruskal_mst_weight(graph);
                cout << " Summed Weight: " << total_weight << " " << endl;
            }
            cout << " Average Weight: " << total_weight/5.0 << " ";
            break;
        case 2:
            for (int i = 0; i < num_trials; i++) {
                Graph graph = generate_graph_2(n, 2);
                total_weight += kruskal_mst_weight(graph);
                cout << " Summed Weight: " << total_weight << " " << endl;
            }
            cout << " Average Weight: " << total_weight/5.0 << " ";
            break;
        case 3:
            for (int i = 0; i < num_trials; i++) {
                Graph graph = generate_graph_2(n, 3);
                total_weight += kruskal_mst_weight(graph);
                cout << " Summed Weight: " << total_weight << " " << endl;
            }
            cout << " Average Weight: " << total_weight/5.0 << " ";
            break;
        case 4:
            for (int i = 0; i < num_trials; i++) {
                Graph graph = generate_graph_2(n, 4);
                total_weight += kruskal_mst_weight(graph);
                cout << " Summed Weight: " << total_weight << " " << endl;
            }
            cout << " Average Weight: " << total_weight/5.0 << " ";
            break;
        default:
            std::cerr << "dimension must be 0, 2, 3, or 4." << endl;
            break;
    }

    return 0;
}
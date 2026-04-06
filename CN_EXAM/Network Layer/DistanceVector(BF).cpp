#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, w;
};

int main() {
    int V = 5;
    vector<Edge> edges = {
        {0, 1, -1},
        {0, 2, 4},
        {1, 2, 3},
        {1, 3, 2},
        {1, 4, 2},
        {3, 2, 5},
        {3, 1, 1},
        {4, 3, -3}
    };

    int src = 0;

    vector<int> dist(V, INT_MAX);
    vector<int> hops(V, INT_MAX);

    dist[src] = 0;
    hops[src] = 0;

    // Relax edges V-1 times
    for (int i = 1; i <= V - 1; i++) {
        for (auto e : edges) {
            if (dist[e.u] != INT_MAX) {
                int newDist = dist[e.u] + e.w;
                int newHops = hops[e.u] + 1;

                // Update if better cost OR same cost but fewer hops
                if (newDist < dist[e.v] || 
                   (newDist == dist[e.v] && newHops < hops[e.v])) {
                    
                    dist[e.v] = newDist;
                    hops[e.v] = newHops;
                }
            }
        }
    }

    // Check negative cycle
    for (auto e : edges) {
        if (dist[e.u] != INT_MAX && dist[e.u] + e.w < dist[e.v]) {
            cout << "Negative weight cycle detected\n";
            return 0;
        }
    }

    // Output
    cout << "Node\tDist\tHops\n";
    for (int i = 0; i < V; i++) {
        cout << i << "\t" << dist[i] << "\t" << hops[i] << "\n";
    }

    return 0;
}
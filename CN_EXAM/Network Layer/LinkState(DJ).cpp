#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Node {
    int u, dist, hops;
};

// custom comparator (min heap)
struct cmp {
    bool operator()(Node a, Node b) {
        if(a.dist == b.dist) return a.hops > b.hops;
        return a.dist > b.dist;
    }
};

int main() {
    int n, e, ttl;
    
    cout<<"enter number of routers: ";
    cin>>n;

    cout<<"enter number of edges: ";
    cin>>e;

    cout<<"enter TTL (max hops): ";
    cin>>ttl;

    vector<vector<pair<int,int>>> adj(n+1);

    cout<<"enter edges (u v weight):\n";
    for(int i=0;i<e;i++){
        int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

    // run for each router
    for(int src=1;src<=n;src++){

        vector<int> dist(n+1, 1e9);
        vector<int> hops(n+1, 1e9);

        priority_queue<Node, vector<Node>, cmp> pq;

        dist[src]=0;
        hops[src]=0;

        pq.push({src,0,0});

        while(!pq.empty()){
            Node cur = pq.top();
            pq.pop();

            int u = cur.u;

            for(auto edge : adj[u]){
                int v = edge.first;
                int w = edge.second;

                int newDist = dist[u] + w;
                int newHops = hops[u] + 1;

                // TTL condition
                if(newHops > ttl) continue;

                if(newDist < dist[v] || 
                  (newDist == dist[v] && newHops < hops[v])){
                    
                    dist[v] = newDist;
                    hops[v] = newHops;

                    pq.push({v, dist[v], hops[v]});
                }
            }
        }

        // print routing table
        cout<<"\nRouter "<<src<<"\n";
        cout<<"Dest Cost Hops\n";

        for(int i=1;i<=n;i++){
            if(dist[i] == 1e9)
                cout<<i<<" INF -\n";
            else
                cout<<i<<" "<<dist[i]<<" "<<hops[i]<<"\n";
        }
    }

    return 0;
}
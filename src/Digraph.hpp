#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <vector>
#include <set>

class Digraph {
    unsigned int V;
    unsigned int E;
    std::vector<std::set<unsigned int>> adj;

public:
    Digraph(unsigned int v) : V(v), E(0), adj(v) {}
    unsigned int getV() const { return V; }
    unsigned int getE() const { return E; }
    void addEdge(unsigned int v, unsigned int w) { adj[v].insert(w); E++; }
    std::set<unsigned int> getAdj(unsigned int v) const { return adj[v]; }
};

#endif // DIGRAPH_HPP


#ifndef DIRECTEDDFS_HPP
#define DIRECTEDDFS_HPP

#include "Digraph.hpp"
#include <vector>

class DirectedDFS {
    std::vector<bool> marked;

    void dfs(const Digraph& G, unsigned int v) {
        marked[v] = true;
        for (unsigned int w : G.getAdj(v))
            if (!marked[w]) dfs(G, w);
    }

public:
    DirectedDFS(const Digraph& G, unsigned int s) : marked(G.getV(), false) {
        dfs(G, s);
    }

    DirectedDFS(const Digraph& G, const std::vector<unsigned int>& sources) : marked(G.getV(), false) {
        for (unsigned int s : sources)
            if (!marked[s]) dfs(G, s);
    }

    bool isMarked(unsigned int v) const { return marked[v]; }
};

#endif // DIRECTEDDFS_HPP


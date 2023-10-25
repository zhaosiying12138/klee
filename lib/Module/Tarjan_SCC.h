//===-- Tarjan_SCC.h ----------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_TARJAN_SCC_H
#define KLEE_TARJAN_SCC_H

#include <stack>
#include <vector>

namespace klee {
class Tarjan_SCC {
public:
    explicit Tarjan_SCC(int size) : size{size}, TarjanIDX{0}, SCC_size{0} {
        edges = new int[size * size]();
        DFN = new int[size]();
        LOW = new int[size]();
        on_stack = new int[size]();
        BELONG = new int[size]();
    }

    ~Tarjan_SCC() {
        delete[] edges;
        delete[] DFN;
        delete[] LOW;
        delete[] on_stack;
        delete[] BELONG;
        delete[] SCC_edges;
        delete[] state_SCC_toposort;
    }

    Tarjan_SCC(const Tarjan_SCC &) = delete;
    Tarjan_SCC(Tarjan_SCC &&) = delete;
    Tarjan_SCC& operator=(const Tarjan_SCC &) = delete;
    Tarjan_SCC& operator=(Tarjan_SCC &&) = delete;

    void add_edge(int i, int j);
    std::vector<std::vector<int>> compute_scc();
    // lifetime of the return array is the same as the tarjan object, be careful when using it!
    int *get_SCC_edges_view();
    std::vector<int> get_SCC_toposort();

private:
    int size;
    int TarjanIDX;
    int SCC_size;
    int *edges;
    int *DFN;
    int *LOW;
    std::stack<int> stack{};
    int *on_stack;
    int *BELONG;
    std::vector<std::vector<int>> result{};
    int *SCC_edges;
    // 0: Unsearched; 1: Searching; 2: Done
    int *state_SCC_toposort;
    std::vector<int> result_toposort_SCC{};

    std::vector<int> find_adjacent_nodes(int i);
    void tarjan(int i);
    void dfs_SCC(int i);
};
}


#endif /* KLEE_TARJAN_SCC_H */
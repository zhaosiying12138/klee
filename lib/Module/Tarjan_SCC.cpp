//===-- Tarjan_SCC.cpp ----------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Tarjan_SCC.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cassert>

using namespace klee;

void Tarjan_SCC::add_edge(int i, int j)
{
    assert(i < size && j < size);
    edges[i * size + j] = 1;
}

std::vector<int> Tarjan_SCC::find_adjacent_nodes(int i)
{
    assert(i < size);
    std::vector<int> ret{};
    for (int j = 0; j < size; j++) {
        if (edges[i * size + j]) {
            ret.push_back(j);
        }
    }
    return ret;
}

std::vector<std::vector<int>> Tarjan_SCC::compute_scc()
{
    for (int i = 0; i < size; i++) {
        if (DFN[i] == 0) {
            tarjan(i);
        }
    }
    llvm::outs() << "Node Belongs to SCCs: ";
    for (int i = 0; i < size; i++) {
        llvm::outs() << BELONG[i] << ", ";
    }
    llvm::outs() << "\n";
    llvm::outs() << "SCC_size: " << SCC_size << "\n";
    SCC_edges = new int[SCC_size * SCC_size]();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if(edges[i * size + j] == 1 && BELONG[i] != BELONG[j]) {
                SCC_edges[BELONG[i] * SCC_size + BELONG[j]] = 1;
            }
        }
    }
    return result;
}

void Tarjan_SCC::tarjan(int u)
{
    LOW[u] = ++TarjanIDX;
    DFN[u] = LOW[u];
    stack.push(u);
    on_stack[u] = 1;
    for (int v : find_adjacent_nodes(u)) {
        if (DFN[v] == 0) { // node v is not visited
            tarjan(v);
            LOW[u] = std::min(LOW[u], LOW[v]);
        } else if (on_stack[v]) {
            LOW[u] = std::min(LOW[u], DFN[v]);
        }
    }
    if (DFN[u] == LOW[u]) {
        std::vector<int> tmp_scc{};
        int v;
        do {
            v = stack.top();
            stack.pop();
            on_stack[v] = 0;
            tmp_scc.push_back(v);
            BELONG[v] = SCC_size;
        } while(u != v);
        std::sort(tmp_scc.begin(), tmp_scc.end());
        result.push_back(tmp_scc);
        ++SCC_size;
    }
}

int *Tarjan_SCC::get_SCC_edges_view()
{
    return SCC_edges;
}
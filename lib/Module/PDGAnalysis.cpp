//===-- PDGAnalysis.cpp ----------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Passes.h"
#include "Tarjan_SCC.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/CFG.h"
#include "llvm/Analysis/PostDominators.h"
#include <vector>
#include <map>
#include <algorithm>
#include <list>

using namespace llvm;

void klee::PDGAnalysis::po_visit_pdt_node(DomTreeNode *dtnode, std::list<DomTreeNode *> &l) {
  for (DomTreeNode *dtnode_child : dtnode->children()) {
    po_visit_pdt_node(dtnode_child, l);
  }
  l.push_back(dtnode);
}

bool klee::PDGAnalysis::runOnFunction(Function &f) {
  if (f.getName() != "zsy_test")
    return false;

  outs() << "[ZSY_PDGAnalysis] PDGAnalysis Pass run!\n";
  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  PostDominatorTree &PDT = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();

  for (Loop *LP : LI) {
    assert(LP->getLoopDepth() == 1);
    func_loop_map.insert({&f, LP});

    klee::PDG_LoopInfo info{};
    info.preheader = LP->getLoopPreheader();
    info.header = LP->getHeader();
    info.latch = LP->getLoopLatch();
    info.exit = LP->getUniqueExitBlock();
    auto loop_body = LP->getBlocksVector();
    loop_body.erase(std::remove(loop_body.begin() ,loop_body.end(), info.header), loop_body.end());
    loop_body.erase(std::remove(loop_body.begin(), loop_body.end(), info.latch), loop_body.end());
    info.bodies = std::move(loop_body);
    loopinfo_map.insert({LP, info});

    outs() << "\nLoopInfo Result:\n";
    outs() << "Loop Preheader: " << info.preheader->getNameOrAsOperand() << "\n";
    outs() << "Loop Header: " << info.header->getNameOrAsOperand() << "\n";
    outs() << "Loop Latch: " << info.latch->getNameOrAsOperand() << "\n";
    outs() << "Loop Exit: " << info.exit->getNameOrAsOperand() << "\n";
    outs() << "Loop body: ";
    for (auto body : info.bodies) {
      outs() << body->getNameOrAsOperand() << ", ";
    }
    outs() << "\n";

    std::list<DomTreeNode *> construct_cd_working_list{};
    DomTreeNode *dtnode_loop_header = PDT.getNode(info.header);
    po_visit_pdt_node(dtnode_loop_header, construct_cd_working_list);

    construct_cd_working_list.remove_if([&info](const DomTreeNode *dtnode) {
      if (std::find(info.bodies.begin(), info.bodies.end(), dtnode->getBlock()) !=
        info.bodies.end()) {
        return false;
      }
      return true;
    });

    outs() << "\nStart to Calculate Control Dependence Graph:\n";
    std::multimap<BasicBlock *, klee::CDGNode> cdginfo_map{};

    for (DomTreeNode *x_dtnode : construct_cd_working_list) {
      BasicBlock *x_bb = x_dtnode->getBlock();
      outs() << "Calculate " << x_bb->getNameOrAsOperand() << ": \n";
      // Case 1:
      outs() << "Case 1:\n";
      for (BasicBlock *y_bb : predecessors(x_bb)) {
        int flag;
        outs() << "\tPredecessor: "<< y_bb->getNameOrAsOperand() << "\n";
        if (PDT.getNode(y_bb)->getIDom() != x_dtnode) {
          const Instruction *TInst = y_bb->getTerminator();
          assert(TInst->getNumSuccessors() == 2);
          if (TInst->getSuccessor(0) == x_bb) {
            flag = 1;
          } else {
            flag = 0;
            assert(TInst->getSuccessor(1) == x_bb);
          }
          klee::CDGNode tmp_cdgNode{y_bb, flag};
          cdginfo_map.insert({x_bb, tmp_cdgNode});
          outs() << "\tadd " << y_bb->getNameOrAsOperand() << "-" << (flag ? "T" : "F")
            <<" to CD(" << x_bb->getNameOrAsOperand() << ")\n";
        }
      }

      // Case 2:
      outs() << "Case 2:\n";
      for (DomTreeNode *z_dtnode : x_dtnode->children()) {
        std::multimap<BasicBlock *, klee::CDGNode> tmp_cdginfo_map{};
        BasicBlock *z_bb = z_dtnode->getBlock();
        auto cdg_multimap_it = cdginfo_map.equal_range(z_bb);
        for (auto it = cdg_multimap_it.first; it != cdg_multimap_it.second; ++it) {
          CDGNode tmp_cdgNode = it->second;
          BasicBlock *y_bb = tmp_cdgNode.bb;
          int y_bb_flag = tmp_cdgNode.flag;
          if (PDT.getNode(y_bb)->getIDom() != x_dtnode) {
            // cdginfo_map.insert({x_bb, tmp_cdgNode}); // BUG! Cannot insert at the same time as iteration
            tmp_cdginfo_map.insert({x_bb, tmp_cdgNode});
            outs() << "\tadd " << y_bb->getNameOrAsOperand() << "-" << (y_bb_flag ? "T" : "F")
              <<" to CD(" << x_bb->getNameOrAsOperand() << ")\n";
          }
        }
        cdginfo_map.insert(tmp_cdginfo_map.begin(), tmp_cdginfo_map.end());
      }
      outs() << "=================\n";
    }

    outs() << "\nCDG Result:\n";
    for (BasicBlock *bb : info.bodies) {
      outs() << bb->getNameOrAsOperand() << ":\t";
      auto cdg_multimap_it = cdginfo_map.equal_range(bb);
      for (auto it = cdg_multimap_it.first; it != cdg_multimap_it.second; ++it) {
        CDGNode tmp_cdgNode = it->second;
        outs() << tmp_cdgNode.bb->getNameOrAsOperand() << "-" << (tmp_cdgNode.flag ? "T" : "F") << ", ";
      }
      outs() << "\n";
    }

    outs() << "\nTarjan-SCC Result:\n";
    klee::Tarjan_SCC tarjan{7};
    tarjan.add_edge(1, 2);
    tarjan.add_edge(1, 3);
    tarjan.add_edge(2, 4);
    tarjan.add_edge(3, 4);
    tarjan.add_edge(3, 5);
    tarjan.add_edge(4, 1);
    tarjan.add_edge(4, 6);
    tarjan.add_edge(5, 6);
    auto sccs = tarjan.compute_scc();
    {
      int i = 0;
      for (auto scc : sccs) {
        outs() << "SCC(" << i++ << "): [";
        for (int u : scc) {
          outs() << u << ", ";
        }
        outs() << "], ";
      }
    }
    outs() << "\n";
    int *scc_edges_view = tarjan.get_SCC_edges_view();
    for (std::size_t i = 0; i != sccs.size(); i++) {
      for (std::size_t j = 0; j != sccs.size(); j++) {
        if (scc_edges_view[i * sccs.size() + j]) {
          outs() << "SCC(" << i << ") -> SCC(" << j << "), ";
        }
      }
    }
    outs() << "\n";
  }
  assert(0);



  // DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  // DT.print(outs());
  return false;
}

void klee::PDGAnalysis::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<LoopInfoWrapperPass>();
  AU.addPreserved<LoopInfoWrapperPass>();
  AU.addRequired<PostDominatorTreeWrapperPass>();
  AU.addPreserved<PostDominatorTreeWrapperPass>();
}

char klee::PDGAnalysis::ID = 0;
std::multimap<Function *, Loop *> klee::PDGAnalysis::func_loop_map{};
std::map<Loop *, klee::PDG_LoopInfo> klee::PDGAnalysis::loopinfo_map{};

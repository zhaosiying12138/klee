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
#include "llvm/Analysis/LoopIterator.h"
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

  outs() << "\nLoopInfo Result:\n";
  outs() << "Loop BasicBlocks: ";
  for (Loop *LP : LI) {
    assert(LP->getLoopDepth() == 1);
    func_loop_map.insert({&f, LP});
    LoopBlocksRPO LBRPO{LP};
    LBRPO.perform(&LI);
    for (auto it = LBRPO.begin(); it != LBRPO.end(); ++it) {
      outs() << (*it)->getNameOrAsOperand() << ", ";
    }
    outs() << "\n";

    klee::PDG_LoopInfo info{};
    std::map<BasicBlock *, int> bb_reverse_id_map{};
    std::map<std::string, int> bb_name_id_map{};
    info.preheader = LP->getLoopPreheader();
    info.header = LP->getHeader();
    info.latch = LP->getLoopLatch();
    info.exit = LP->getUniqueExitBlock();
    auto loop_body = LP->getBlocksVector();
    loop_body.erase(std::remove(loop_body.begin() ,loop_body.end(), info.header), loop_body.end());
    loop_body.erase(std::remove(loop_body.begin(), loop_body.end(), info.latch), loop_body.end());
    info.bodies = std::move(loop_body);
    loopinfo_map.insert({LP, info});

    outs() << "Loop Preheader: " << info.preheader->getNameOrAsOperand() << "\n";
    outs() << "Loop Header: " << info.header->getNameOrAsOperand() << "\n";
    outs() << "Loop Latch: " << info.latch->getNameOrAsOperand() << "\n";
    outs() << "Loop Exit: " << info.exit->getNameOrAsOperand() << "\n";
    outs() << "Loop body: ";
    {
      int id = 0;
      bb_reverse_id_map.insert({info.header, id});
      bb_name_id_map.emplace(info.header->getNameOrAsOperand(), id++);
      for (auto body : info.bodies) {
        outs() << body->getNameOrAsOperand() << ", ";
        bb_reverse_id_map.insert({body, id});
        bb_name_id_map.emplace(body->getNameOrAsOperand(), id++);
      }
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
    std::multimap<BasicBlock *, klee::CDGNode> cdginfo{};

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
          cdginfo.insert({x_bb, tmp_cdgNode});
          outs() << "\tadd " << y_bb->getNameOrAsOperand() << "-" << (flag ? "T" : "F")
            <<" to CD(" << x_bb->getNameOrAsOperand() << ")\n";
        }
      }

      // Case 2:
      outs() << "Case 2:\n";
      for (DomTreeNode *z_dtnode : x_dtnode->children()) {
        std::multimap<BasicBlock *, klee::CDGNode> tmp_cdginfo{};
        BasicBlock *z_bb = z_dtnode->getBlock();
        auto cdg_multimap_it = cdginfo.equal_range(z_bb);
        for (auto it = cdg_multimap_it.first; it != cdg_multimap_it.second; ++it) {
          CDGNode tmp_cdgNode = it->second;
          BasicBlock *y_bb = tmp_cdgNode.bb;
          int y_bb_flag = tmp_cdgNode.flag;
          if (PDT.getNode(y_bb)->getIDom() != x_dtnode) {
            // cdginfo.insert({x_bb, tmp_cdgNode}); // BUG! Cannot insert at the same time as iteration
            tmp_cdginfo.insert({x_bb, tmp_cdgNode});
            outs() << "\tadd " << y_bb->getNameOrAsOperand() << "-" << (y_bb_flag ? "T" : "F")
              <<" to CD(" << x_bb->getNameOrAsOperand() << ")\n";
          }
        }
        cdginfo.insert(tmp_cdginfo.begin(), tmp_cdginfo.end());
      }
      outs() << "=================\n";
    }
    cdginfo_map.insert({LP, cdginfo});

    outs() << "\nCDG Result:\n";
    // PDG Construction Step 1: add control dependence
    klee::Tarjan_SCC tarjan{static_cast<int>(info.bodies.size()) + 1};
    for (BasicBlock *bb : info.bodies) {
      outs() << bb->getNameOrAsOperand() << ":\t";
      auto cdg_multimap_it = cdginfo.equal_range(bb);
      for (auto it = cdg_multimap_it.first; it != cdg_multimap_it.second; ++it) {
        CDGNode tmp_cdgNode = it->second;
        outs() << tmp_cdgNode.bb->getNameOrAsOperand() << "-" << (tmp_cdgNode.flag ? "T" : "F") << ", ";
        auto it_u = bb_reverse_id_map.find(tmp_cdgNode.bb);
        auto it_v = bb_reverse_id_map.find(bb);
        assert(it_u != bb_reverse_id_map.end() && it_v != bb_reverse_id_map.end());
        tarjan.add_edge(it_u->second, it_v->second);
      }
      outs() << "\n";
    }

    // PDG Construction Step 2: add data dependence
    auto add_edge = [&tarjan, &bb_name_id_map](auto str1, auto str2) {
      auto it_u = bb_name_id_map.find(str1);
      auto it_v = bb_name_id_map.find(str2);
      assert(it_u != bb_name_id_map.end() && it_v != bb_name_id_map.end());
      tarjan.add_edge(it_u->second, it_v->second);
    };
    {
      // dump output directly from isl_autovec: lib/Module/isl_autovec.c
      add_edge("S5", "S5");
      add_edge("S5", "S4");
      add_edge("S5", "S6");
      add_edge("S5", "S7");
      add_edge("S3", "S5");
      add_edge("S3", "S4");
      add_edge("S3", "S6");
      add_edge("S3", "S7");
      add_edge("S4", "S5");
      add_edge("S4", "S6");
      add_edge("S1", "S3");
      add_edge("S6", "S5");
      add_edge("S6", "S4");
      add_edge("S6", "S6");
      add_edge("S6", "S7");
      add_edge("S2", "S3");
      add_edge("S2", "S7");
      add_edge("S7", "S8");
      // Expect Result: [%4] -> [S1] -> [S2] -> [S3] -> [S4,S6,S5] -> [S7] -> [S8]
    }
    {
      // P421 Result, some data dependence missing! Bug!
      // Get Wrong Result: [%4] -> [S1] -> [S2] -> [S4,S6,S5] -> [S7] -> [S8] -> [S3]
      // add_edge("S4", "S5");
      // add_edge("S5", "S4");
      // add_edge("S4", "S6");
      // add_edge("S6", "S4");
      // add_edge("S7", "S8");
    }

    outs() << "\nTarjan-SCC Result:\n";
    outs() << "BasicBlock-ID-Mapping: ";
    outs() << info.header->getNameOrAsOperand() << "(" << bb_reverse_id_map.find(info.header)->second << "), ";
    for (auto bb : info.bodies) {
      outs() << bb->getNameOrAsOperand() << "(" << bb_reverse_id_map.find(bb)->second << "), ";
    }
    outs() << "\n";
    auto sccs = tarjan.compute_scc();
    {
      int i = 0;
      for (auto scc : sccs) {
        outs() << "SCC(" << i++ << "): [";
        for (int u : scc) {
          outs() << u << ",";
        }
        outs() << "], ";
      }
    }
    outs() << "\n";
    // SCC Edges:
    // [S7] -> [S8]
    // [S4,S6,S5] -> [S7]
    // [S2] -> [S3]
    // [S2] -> [S4,S6,S5]
    // [S1] -> [S3]
    // [S1] -> [S2]
    // [%4] -> [S8]
    // [%4] -> [S1]

    int *scc_edges_view = tarjan.get_SCC_edges_view();
    auto print_scc = [&info] (std::vector<int> scc) {
      assert(!scc.empty());
      BasicBlock *bb;
      bb = scc[0] == 0 ? info.header : info.bodies[scc[0] - 1];
      outs() << "[" << bb->getNameOrAsOperand();
      for (std::size_t i = 1; i != scc.size(); ++i) {
        BasicBlock *bb = scc[i] == 0 ? info.header : info.bodies[scc[i] - 1];
        outs() << "," << bb->getNameOrAsOperand();
      }
      outs() << "]";
    };

    outs() << "SCC Edges:\n";
    for (std::size_t i = 0; i != sccs.size(); i++) {
      for (std::size_t j = 0; j != sccs.size(); j++) {
        if (scc_edges_view[i * sccs.size() + j]) {
          auto scc1 = sccs[i];
          auto scc2 = sccs[j];
          print_scc(scc1);
          outs() << " -> ";
          print_scc(scc2);
          outs() << "\n";
        }
      }
    }
    outs() << "\n";

    outs() << "SCC Toposort:\n";
    std::queue<std::vector<BasicBlock *>> sccs_worklist{};
    for (int scc_id : tarjan.get_SCC_toposort()) {
      std::vector<BasicBlock *> tmp_scc_bbs{};
      BasicBlock *tmp_bb;
      for (int i : sccs[scc_id]) {
        tmp_bb = i == 0 ? info.header : info.bodies[i - 1];
        tmp_scc_bbs.push_back(tmp_bb);
      }
      sccs_worklist.push(tmp_scc_bbs);
      print_scc(sccs[scc_id]);
      outs() << " -> ";
    }
    outs() << "\n";
    sccs_worklist_map.insert({LP, sccs_worklist});
  }

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
std::map<Loop *, std::multimap<BasicBlock *, klee::CDGNode>> klee::PDGAnalysis::cdginfo_map{};
std::map<Loop *, std::queue<std::vector<BasicBlock *>>> klee::PDGAnalysis::sccs_worklist_map{};

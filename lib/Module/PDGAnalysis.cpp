//===-- PhiCleaner.cpp ----------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Passes.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include <vector>
#include <map>
#include <algorithm>

using namespace llvm;

char klee::PDGAnalysis::ID = 0;
std::multimap<Function *, Loop *> klee::PDGAnalysis::func_loop_map{};
std::map<Loop *, klee::PDG_LoopInfo> klee::PDGAnalysis::loopinfo_map{};

bool klee::PDGAnalysis::runOnFunction(Function &f) {
  if (f.getName() != "zsy_test")
    return false;

  outs() << "[ZSY_PDGAnalysis] PDGAnalysis Pass run!\n";
  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

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
  }



  // DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  // DT.print(outs());
  return false;
}

void klee::PDGAnalysis::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<LoopInfoWrapperPass>();
  AU.addPreserved<LoopInfoWrapperPass>();

  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addPreserved<DominatorTreeWrapperPass>();

}

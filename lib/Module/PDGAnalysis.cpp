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
#include <set>

using namespace llvm;

char klee::PDGAnalysis::ID = 0;
std::string klee::PDGAnalysis::analysisInfo{};

bool klee::PDGAnalysis::runOnFunction(Function &f) {
  if (f.getName() != "zsy_test")
    return false;

  llvm::outs() << "[ZSY] PDGAnalysis Pass run!\n";
  f.dump();
  analysisInfo = "[ZSY_PDG] " + f.getName().str();
  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  LI.print(outs());
  DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  DT.print(outs());
  return false;
}

std::string klee::PDGAnalysis::getAnalysisInfo() {
  return analysisInfo;
}

void klee::PDGAnalysis::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<LoopInfoWrapperPass>();
  AU.addPreserved<LoopInfoWrapperPass>();

  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addPreserved<DominatorTreeWrapperPass>();

}

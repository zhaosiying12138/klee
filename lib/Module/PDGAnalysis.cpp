//===-- PhiCleaner.cpp ----------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Passes.h"

#include <set>

using namespace llvm;

char klee::PDGAnalysis::ID = 0;
std::string klee::PDGAnalysis::analysisInfo{};

bool klee::PDGAnalysis::runOnFunction(Function &f) {
  llvm::outs() << "[ZSY] PDGAnalysis Pass run!\n";
  analysisInfo = "[ZSY_PDG] " + f.getName().str();
  return false;
}

std::string klee::PDGAnalysis::getAnalysisInfo() {
  return analysisInfo;
}

//===-- PDGInfo.h --------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_PDGINFO_H
#define KLEE_PDGINFO_H

#include "llvm/IR/Function.h"
#include <vector>

namespace klee {
  struct PDG_LoopInfo {
    llvm::BasicBlock *preheader;
    llvm::BasicBlock *header;
    llvm::BasicBlock *latch;
    llvm::BasicBlock *exit;
    std::vector<llvm::BasicBlock *> bodies{};
  };
}

#endif /* KLEE_PDGINFO_H */

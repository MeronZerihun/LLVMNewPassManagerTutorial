
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;


namespace {
	

	struct AddToSubPass : public PassInfoMixin<AddToSubPass> {
		
		PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM){
			
			for(Function &F : M){

				for(BasicBlock &BB : F){
					for(Instruction &I : BB){
						if(auto *oldInst = dyn_cast<Instruction>(&I)){
							IRBuilder<> builder(oldInst);
							
							if(auto *addInst = dyn_cast<BinaryOperator>(oldInst)){
								if(addInst->getOpcode() == Instruction::Add){
									Value *subInst = builder.CreateSub(addInst->getOperand(0), addInst->getOperand(1));
									addInst->replaceAllUsesWith(subInst);
						
								}
							}
						}
					}
				}	
			}
			// The program has been modified. 
			return PreservedAnalyses::none();
		}
	};
}


// Register Pass 
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "AddToSubPass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "add-to-sub-pass"){
            MPM.addPass(AddToSubPass());
            return true;
          }
          return false;
        });
	}};
 }



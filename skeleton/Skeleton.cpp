#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/InstIterator.h"
using namespace llvm;

namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
    	LLVMContext &Ctx = F.getContext();
		Type *retTypeVoid = Type::getVoidTy(Ctx);
      	IRBuilder<> IRB(F.getEntryBlock().getFirstNonPHI());//PHI is an instruction type
     
		if(F.getName() == "main"){//create Shadow Stack before main starts
			outs() << "I am main function!!\n";
			FunctionType *createSSFuncType = FunctionType::get(retTypeVoid,false);
        	FunctionCallee createSSFunc = F.getParent()->getOrInsertFunction("createSS", createSSFuncType);
			IRB.CreateCall(createSSFunc);

		}

	 	//Insert llvm.returnaddress in the beginning of of each function   
     	Value *ReturnAddress = IRB.CreateCall(
        Intrinsic::getDeclaration(F.getParent(), Intrinsic::returnaddress),IRB.getInt32(0));
		
		//void insertPC(char *k) definition
		std::vector<Type*> paramTypes_insertPC = {PointerType::get(Type::getInt8Ty(Ctx),0)};	
    	FunctionType *FuncType_insertPC = FunctionType::get(retTypeVoid,paramTypes_insertPC,false);
    	FunctionCallee insertPCFunc = F.getParent()->getOrInsertFunction("insertPC", FuncType_insertPC);


        //Insert insertPC in the beginning of each function   
	    Value* args2[] = {ReturnAddress};
        IRB.CreateCall(insertPCFunc, args2);
       


	

	// Suppose F is a ptr to a function, iterate over instruction
	for (inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){

           Instruction *I = &*i;    		
	   if (CallInst *CI = dyn_cast<CallInst>(&*i)){
		//outs() << "I am call: " << F.getName() << *i << "\n";

	   }

	   if (ReturnInst *RI =dyn_cast<ReturnInst>(&*i)){
	   	//outs() << "I am ret: " << *i << "\n";
	
		IRBuilder <> IRB(RI);
		//Insert llvm.returnaddress at the end of each function
		Value *ReturnAddress = IRB.CreateCall(
       	Intrinsic::getDeclaration(F.getParent(), Intrinsic::returnaddress),IRB.getInt32(0),"RA");
	
		//void checkPC(char *k) definition
		std::vector<Type*> paramTypes_checkPC = {PointerType::get(Type::getInt8Ty(Ctx),0)};	
    	FunctionType *logFuncType_checkPC = FunctionType::get(retTypeVoid,paramTypes_checkPC,false);
    	FunctionCallee checkPC_Func = F.getParent()->getOrInsertFunction("checkPC", logFuncType_checkPC);

		
		//Insert checkPC at the end of each function   
        Value* args2[] = {ReturnAddress};
        IRB.CreateCall(checkPC_Func, args2);


	 }

       }

       	return true;
       }
  };
}

char SkeletonPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);

#include "../include/optimization.h"

bool PatternBase::isSameOperands(Operation* op1, Operation* op2) const {
    if (op1->getOperands().size() != op2->getOperands().size()) {
        return false;
    }
    for(auto index = 0; index < op1->getOperands().size(); ++index) {
        if (op1->getOperands()[index] != op2->getOperands()[index]) {
            return false;
        }
    }
    return true;
}

void OptimizationBase::addPattern(PatternBase* pattern) {
    patterns.push_back(pattern);
}

bool OptimizationBase::applyPatterns(Operation* op, Module* ir) {
    bool irWasChanged = false;
    for(auto pattern : patterns) {
        if (irWasChanged) {
            return true;
        }
        irWasChanged |= pattern->matchAndRewrite(op, ir);
    }
    return irWasChanged;
}

void OptimizationBase::runOnOperation() {
    bool irWasChanged = true;
        while(irWasChanged) {
            irWasChanged = false;
            auto&& tmp_bbs = ir->getBBs();
            for(auto& bb : tmp_bbs) {
                auto&& tmp_ops = bb->getOps();
                for(auto& op : tmp_ops) {
                    irWasChanged |= applyPatterns(op, ir);
                }
            }
        }
}

void OptimizationBase::runOnOperationReverse() {
    ir->DFS();
    bool irWasChanged = true;
    auto order = ir->rpo();

    while(irWasChanged) {
        irWasChanged = false;
        for(auto&& bb : order) {
            for(int idx = bb->getOps().size() - 1; idx >= 0; idx--) {
                auto op = bb->getOps()[idx];
                irWasChanged |= applyPatterns(op, ir);
            }
        }
    }
}

bool CheckEliminationPattern::matchAndRewrite(Operation* op, Module* ir) {
    if (op->getName() == "BoundsCheck" || op->getName() == "ZeroCheck") {
        for(auto&& bb : ir->getBBs()) {
            for(auto&& op2 : bb->getOps()) {
                if (op->getName() == op2->getName() && op != op2 && isSameOperands(op, op2)) {
                    if (ir->isDominator(ir->getBBFromOp(op), ir->getBBFromOp(op2))) {
                        ir->eraseWithoutResult(op2);
                    } else {
                        ir->eraseWithoutResult(op);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool DCEPattern::matchAndRewrite(Operation* op, Module* ir) {
    if (!op->hasMemoryEffect()) {
        if (!ir->haveUsers(op->getIndex())) {
            ir->deleteOp(op);
            return true;
        }
    }
    return false;
}
bool AddConstant::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "Add") {
        BinaryOperation* addOperation = dynamic_cast<BinaryOperation*>(op);
        //analyze
        auto a = addOperation->getA();
        auto b = addOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() == "Constant" && op2->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            ConstantOperation* const2 = dynamic_cast<ConstantOperation*>(op2);
            if (const1->getDType() != const2->getDType()) {
                return false;//don't support different types
            }
            auto constResult = ir->create<ConstantOperation>(const1->getValue() + const2->getValue(), const1->getDType());
            ir->replace(op, constResult);
            return true;
        }
        
    }
    return false;
}

bool MulConstantPattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "Mul") {
        BinaryOperation* mulOperation = dynamic_cast<BinaryOperation*>(op);
        //analyze
        auto a = mulOperation->getA();
        auto b = mulOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() == "Constant" && op2->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            ConstantOperation* const2 = dynamic_cast<ConstantOperation*>(op2);
            if (const1->getDType() != const2->getDType()) {
                return false;//don't support different types
            }
            auto constResult = ir->create<ConstantOperation>(const1->getValue() * const2->getValue(), const1->getDType());
            ir->replace(op, constResult);
            return true;
        }
        
    }
    return false;
}

bool ShiftConstantPattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "Shift") {
        ShiftOperation* shiftOperation = dynamic_cast<ShiftOperation*>(op);
        //analyze
        if (shiftOperation->getDirection() != "Left" && shiftOperation->getDirection() != "Right") {
            return false; //don't support these directions
        }
        auto a = shiftOperation->getA();
        auto b = shiftOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() == "Constant" && op2->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            ConstantOperation* const2 = dynamic_cast<ConstantOperation*>(op2);
            if (const1->getDType() != "I32" && const2->getDType() != "I32") {
                return false;//don't support different types
            }
            ConstantOperation* constResult;
            if (shiftOperation->getDirection() == "Left") {
                constResult = ir->create<ConstantOperation>(const1->getValue() << const2->getValue(), const1->getDType());
            } else {
                constResult = ir->create<ConstantOperation>(const1->getValue() >> const2->getValue(), const1->getDType());
            }
            ir->replace(op, constResult);
            return true;
        }
        
    }
    return false;
}

bool AndConstantPattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "And") {
        BinaryOperation* andOperation = dynamic_cast<BinaryOperation*>(op);
        //analyze
        auto a = andOperation->getA();
        auto b = andOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() == "Constant" && op2->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            ConstantOperation* const2 = dynamic_cast<ConstantOperation*>(op2);
            if (const1->getDType() != "I32" || const2->getDType() != "I32") {
                return false;//don't support different types
            }
            auto constResult = ir->create<ConstantOperation>(const1->getValue() & const2->getValue(), const1->getDType());
            ir->replace(op, constResult);
            return true;
        }
        
    }
    return false;
}

bool MulZeroPattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "Mul") {
        BinaryOperation* mulOperation = dynamic_cast<BinaryOperation*>(op);
        //analyze
        auto a = mulOperation->getA();
        auto b = mulOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() != "Constant" && op2->getName() != "Constant") {
            return false;
        }
        if (op1->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            if (const1->getValue() == 0) {
                auto constResult = ir->create<ConstantOperation>(0, const1->getDType());
                ir->replace(op, constResult);
                return true;
            }
        }
        if (op2->getName() == "Constant") {
            ConstantOperation* const2 = dynamic_cast<ConstantOperation*>(op2);
            if (const2->getValue() == 0) {
                auto constResult = ir->create<ConstantOperation>(0, const2->getDType());
                ir->replace(op, constResult);
                return true;
            }
        }
    }
    return false;
}

bool MulOnePattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "Mul") {
        BinaryOperation* mulOperation = dynamic_cast<BinaryOperation*>(op);
        //analyze
        auto a = mulOperation->getA();
        auto b = mulOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() != "Constant" && op2->getName() != "Constant") {
            return false;
        }
        if (op1->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            if (const1->getValue() == 1) {
                ir->replaceAllUses(op, op2);
                return true;
            }
        }
        if (op2->getName() == "Constant") {
            ConstantOperation* const2 = dynamic_cast<ConstantOperation*>(op2);
            if (const2->getValue() == 1) {
                ir->replaceAllUses(op, op1);
                return true;
            }
        }
    }
    return false;
}

bool ShiftZeroPattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "Shift") {
        ShiftOperation* shiftOperation = dynamic_cast<ShiftOperation*>(op);
        //analyze
        auto a = shiftOperation->getA();
        auto b = shiftOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() != "Constant" && op2->getName() != "Constant") {
            return false;
        }
        if (op1->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            if (const1->getValue() == 0) {
                ir->replaceAllUses(op, op2);
                return true;
            }
        }
        if (op2->getName() == "Constant") {
            ConstantOperation* const2 = dynamic_cast<ConstantOperation*>(op2);
            if (const2->getValue() == 0) {
                ir->replaceAllUses(op, op1);
                return true;
            }
        }
    }
    return false;
}

bool MulDecomposeShiftPattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "Mul") {
        BinaryOperation* mulOperation = dynamic_cast<BinaryOperation*>(op);
        //analyze
        auto a = mulOperation->getA();
        auto b = mulOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() != "Constant" && op2->getName() != "Constant") {
            return false;
        }
        if (op1->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            //n > 0 && (n & (n - 1)) == 0
            if (const1->getValue() > 0 && (const1->getValue() & const1->getValue() - 1) == 0) {
                if (ir->getNumUsers(const1->getIndex()) > 1) {
                    return false;
                }
                auto constantShift = ir->create<ConstantOperation>(-1, std::log2(const1->getValue()), "I32");
                ir->replace(const1, constantShift);
                auto shift = ir->create<ShiftOperation>(0, "Left", std::make_pair(ir->getBBs()[0], op2->getIndex()), std::make_pair(ir->getBBs()[0], constantShift->getIndex()));
                ir->replace(op, shift);
                return true;
            }
        }
        if (op2->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op2);
            //n > 0 && (n & (n - 1)) == 0
            if (const1->getValue() > 0 && (const1->getValue() & const1->getValue() - 1) == 0) {
                if (ir->getNumUsers(const1->getIndex()) > 1) {
                    return false;
                }
                auto constantShift = ir->create<ConstantOperation>(-1, std::log2(const1->getValue()), "I32");
                ir->replace(const1, constantShift);
                auto shift = ir->create<ShiftOperation>(0, "Left", std::make_pair(ir->getBBs()[0], op1->getIndex()), std::make_pair(ir->getBBs()[0], constantShift->getIndex()));
                ir->replace(op, shift);
                return true;
            }
        }
    }
    return false;
}

bool AndZeroPattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "And") {
        BinaryOperation* mulOperation = dynamic_cast<BinaryOperation*>(op);
        //analyze
        auto a = mulOperation->getA();
        auto b = mulOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1->getName() != "Constant" && op2->getName() != "Constant") {
            return false;
        }
        if (op1->getName() == "Constant") {
            ConstantOperation* const1 = dynamic_cast<ConstantOperation*>(op1);
            if (const1->getValue() == 0) {
                auto constResult = ir->create<ConstantOperation>(0, const1->getDType());
                ir->replace(op, constResult);
                return true;
            }
        }
        if (op2->getName() == "Constant") {
            ConstantOperation* const2 = dynamic_cast<ConstantOperation*>(op2);
            if (const2->getValue() == 0) {
                auto constResult = ir->create<ConstantOperation>(0, const2->getDType());
                ir->replace(op, constResult);
                return true;
            }
        }
    }
    return false;
}

bool AndEqualPattern::matchAndRewrite(Operation *op, Module* ir) {
    if (op->getName() == "And") {
        BinaryOperation* mulOperation = dynamic_cast<BinaryOperation*>(op);
        //analyze
        auto a = mulOperation->getA();
        auto b = mulOperation->getB();
        Operation* op1 = ir->getOperation(a.first, a.second);
        Operation* op2 = ir->getOperation(b.first, b.second);
        if (op1 == op2) {
            ir->replaceAllUses(op, op1);
        }   
    }
    return false;
}

BasicBlock* StaticInlinePattern::splitCallerBlock(Module* ir, Module* callee, Operation* op) {
    auto newBB = ir->create<BasicBlock>((int)(ir->getBBs().size() + callee->getBBs().size()));
    auto bb = ir->getBBs()[ir->getBBFromOp(op)];
    bool older = false;
    int idx = 0;
    for(auto&& op2 : bb->getOps()) {
        if (older) {
            newBB->insert(op2);
            bb->deleteOp(idx);
        }
        older |= (op == op2);
        idx++;
    }
    return newBB;
}

void StaticInlinePattern::updateDF(Module* ir, Module* callee, Operation* op, BasicBlock* newBB) {
    std::vector<int> returns;
    std::vector<BasicBlock*> inputs;
    std::vector<Operation*> prevOps;
    int newidx;

    for(auto idx : op->getOperands()) {
        prevOps.push_back(ir->getOperation(idx));
    }

    for(auto&& bb : callee->getBBs()) {
        int index = 0;
        for(auto&& op2 : bb->getOps()) {
            if (op2->getName() == "Move") {
                auto mov = dynamic_cast<MoveOperation*>(op2);
                callee->replaceAllUses(mov, prevOps[mov->getArg()]);                
                bb->deleteOp(index);
            } else if (op2->getName() == "Return") {
                returns.push_back(op2->getOperands()[0]);
                inputs.push_back(bb);
                newidx = op2->getIndex();
                bb->deleteOp(index);
            } else {
                index++;
            }
        }
    }
    auto newPhi = ir->create<PhiOperation>(newidx);

    newPhi->addInputs(inputs, returns);
    newBB->insertHead(newPhi);
    ir->replaceAllUses(op, newPhi);
    
}

void StaticInlinePattern::moveConstants(Module* ir, Module* callee) {
    for(auto&& bb : callee->getBBs()) {
        int index = 0;
        for(auto&& op : bb->getOps()) {
            if (op->getName() == "Constant") {
                ir->getBBs()[0]->insert(op);
                bb->deleteOp(index);
            } else {
                index++;
            }
        }
    }

}

void StaticInlinePattern::connectBlocks(Module* ir, Module* callee, BasicBlock* startBB, BasicBlock* endBB, Operation* op) {

    for(auto&& bb : callee->getBBs()) {
        ir->insertIndex(bb);
    }

    auto firstInlinedBB = ir->getBBs()[startBB->getID() + 1];
    startBB->addSucessor({firstInlinedBB});
    firstInlinedBB->addPredessor({startBB});
    auto jmp = ir->create<JumpOperation>(0, callee->getBBs()[0]);
    ir->replace(op, jmp);

    auto phi = dynamic_cast<PhiOperation*>(endBB->getOps()[0]);
    
    for(auto input : phi->getInputs()) {
        endBB->addPredessor({input});
        input->addSucessor({endBB});
    }
}

bool StaticInlinePattern::matchAndRewrite(Operation* op, Module* ir) {
    if (op->getName() == "CallStatic") {
        auto callOp = dynamic_cast<CallStaticOperation*>(op);
        auto callee = callOp->getCallee();
        auto newBB = splitCallerBlock(ir, callee, op);
        updateDF(ir, callee, op, newBB);
        moveConstants(ir, callee);
        auto startBB = ir->getBBs()[ir->getBBFromOp(op)];
        connectBlocks(ir, callee, startBB, newBB, op);
        return true;   
    }
    return false;
}

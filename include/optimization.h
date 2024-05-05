
#include "module.h"


class PatternBase {
    std::string nameOp;
public:
    PatternBase(std::string name) : nameOp(name) {}
    virtual bool matchAndRewrite(Operation* op, Module* ir) = 0; 
    virtual ~PatternBase() {}
};

class OptimizationBase {
private:
    std::vector<PatternBase*> patterns;
public:
    Module* ir;
    OptimizationBase(Module* _ir) : ir(_ir) {}
    virtual void run() = 0;
    void addPattern(PatternBase* pattern);
    bool applyPatterns(Operation* op, Module* ir);
    virtual ~OptimizationBase() {}
    
};

class DCEPattern final : public PatternBase {
    public:
        DCEPattern() : PatternBase("Any") {}
        bool matchAndRewrite(Operation* op, Module* ir) override {
            if (!op->hasMemoryEffect()) {
                if (!ir->haveUsers(op->getIndex())) {
                    ir->deleteOp(op);
                    return true;
                }
            }
            return false;
        }
};

class AddConstant final : public PatternBase {
    public:
        AddConstant() : PatternBase("Add") {}
        bool matchAndRewrite(Operation *op, Module* ir) override {
            if (op->getName() == "Add") {
                BinaryOperation* addOperation = dynamic_cast<BinaryOperation*>(op);
                //analyze
                auto a = addOperation->getA();
                auto b = addOperation->getB();
                Operation* op1 = ir->getOperation(a.first, a.second);
                Operation* op2 = ir->getOperation(b.first, b.second);
                if (op1->getName() == "Constant" || op2->getName() == "Constant") {
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
};

struct MulConstantPattern final : public PatternBase {
    MulConstantPattern() : PatternBase("Mul") {}
    bool matchAndRewrite(Operation *op, Module* ir) override {
            if (op->getName() == "Mul") {
                BinaryOperation* mulOperation = dynamic_cast<BinaryOperation*>(op);
                //analyze
                auto a = mulOperation->getA();
                auto b = mulOperation->getB();
                Operation* op1 = ir->getOperation(a.first, a.second);
                Operation* op2 = ir->getOperation(b.first, b.second);
                if (op1->getName() == "Constant" || op2->getName() == "Constant") {
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
};

struct ShiftConstantPattern final : public PatternBase {
    ShiftConstantPattern() : PatternBase("Shift") {}
    bool matchAndRewrite(Operation *op, Module* ir) override {
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
                if (op1->getName() == "Constant" || op2->getName() == "Constant") {
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
};

class ConstantFoldingOptimization final : public OptimizationBase {
public:
    ConstantFoldingOptimization(Module* _ir) : OptimizationBase(_ir) {
        
    }
    void run() override {
        DCEPattern pattern;
        AddConstant pattern1;
        MulConstantPattern pattern2;
        ShiftConstantPattern pattern3;
        addPattern(&pattern);
        addPattern(&pattern1);
        addPattern(&pattern2);
        addPattern(&pattern3);

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

};
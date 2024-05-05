
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

class addConstant final : public PatternBase {
    public:
        addConstant() : PatternBase("Add") {}
        bool matchAndRewrite(Operation *op, Module* ir) {
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
                    ConstantOperation constResult = ConstantOperation(const1->getValue() + const2->getValue(), const1->getDType());
                    ir->replace(op, &constResult);
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
        addConstant pattern1;
        addPattern(&pattern1);

        bool irWasChanged = true;
        while(irWasChanged) {
            std::cout << __LINE__ << " " << __FILE__ << std::endl;
            irWasChanged = false;
            auto&& tmp_bbs = ir->getBBs();
            for(auto& bb : tmp_bbs) {
                std::cout << __LINE__ << " " << __FILE__ << std::endl;
                auto&& tmp_ops = bb->getOps();
                for(auto& op : tmp_ops) {
                    std::cout << __LINE__ << " " << __FILE__ << std::endl;
                    std::cout << op << std::endl;
                    op->print();
                    std::cout << __LINE__ << " " << __FILE__ << std::endl;
                    irWasChanged |= applyPatterns(op, ir);
                }
            }
            std::cout << "FINISH FIRST LAP" << std::endl;
        }
    }

};
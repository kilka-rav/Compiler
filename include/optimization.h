
#include "module.h"
#include <cmath>

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
    void runOnOperation();
    
};


class DCEPattern final : public PatternBase {
    public:
        DCEPattern() : PatternBase("Any") {}
        bool matchAndRewrite(Operation* op, Module* ir) override;
};

class AddConstant final : public PatternBase {
    public:
        AddConstant() : PatternBase("Add") {}
        bool matchAndRewrite(Operation *op, Module* ir) override;
};

struct MulConstantPattern final : public PatternBase {
    MulConstantPattern() : PatternBase("Mul") {}
    bool matchAndRewrite(Operation *op, Module* ir) override;
};

struct ShiftConstantPattern final : public PatternBase {
    ShiftConstantPattern() : PatternBase("Shift") {}
    bool matchAndRewrite(Operation *op, Module* ir);
};

struct AndConstantPattern final : public PatternBase {
    public:
        AndConstantPattern() : PatternBase("And") {}
        bool matchAndRewrite(Operation *op, Module* ir) override;
};

struct MulZeroPattern final : public PatternBase {
    MulZeroPattern() : PatternBase("Mul") {}
    bool matchAndRewrite(Operation *op, Module* ir) override;
};

struct MulOnePattern final : public PatternBase {
    MulOnePattern() : PatternBase("Mul") {}
    bool matchAndRewrite(Operation *op, Module* ir) override;
};

struct ShiftZeroPattern final : public PatternBase {
    ShiftZeroPattern() : PatternBase("Shift") {}
    bool matchAndRewrite(Operation *op, Module* ir) override;
};

struct MulDecomposeShiftPattern final : public PatternBase {
    MulDecomposeShiftPattern() : PatternBase("Shift") {}
    bool matchAndRewrite(Operation *op, Module* ir) override;
};

struct AndZeroPattern final : public PatternBase {
    AndZeroPattern() : PatternBase("And") {}
    bool matchAndRewrite(Operation *op, Module* ir) override;
};

struct AndEqualPattern final : public PatternBase {
    AndEqualPattern() : PatternBase("And") {}
    bool matchAndRewrite(Operation *op, Module* ir) override;
};

class ConstantFoldingOptimization final : public OptimizationBase {
public:
    ConstantFoldingOptimization(Module* _ir) : OptimizationBase(_ir) {}
    void run() override {
        DCEPattern pattern;
        AddConstant pattern1;
        MulConstantPattern pattern2;
        ShiftConstantPattern pattern3;
        AndConstantPattern pattern4;

        addPattern(&pattern);
        addPattern(&pattern1);
        addPattern(&pattern2);
        addPattern(&pattern3);
        addPattern(&pattern4);

        runOnOperation();
    }

};

struct PeepholesOptimization final : OptimizationBase {
    PeepholesOptimization(Module* _ir) : OptimizationBase(_ir) {}
    void run() override {
        DCEPattern pattern;
        MulZeroPattern pattern1;
        MulOnePattern pattern2;
        ShiftZeroPattern pattern3;
        MulDecomposeShiftPattern pattern4;
        AndZeroPattern pattern5;
        AndEqualPattern pattern6;

        addPattern(&pattern);
        addPattern(&pattern1);
        addPattern(&pattern2);
        addPattern(&pattern3);
        addPattern(&pattern4);
        addPattern(&pattern5);
        addPattern(&pattern6);

        runOnOperation();

    }
        
};

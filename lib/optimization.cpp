#include "../include/optimization.h"

void OptimizationBase::addPattern(PatternBase* pattern) {
    patterns.push_back(pattern);
}

bool OptimizationBase::applyPatterns(Operation* op, Module* ir) {
    bool irWasChanged = false;
    std::pair<bool, Module*> res;
    for(auto pattern : patterns) {
        irWasChanged |= pattern->matchAndRewrite(op, ir);
    }
    return irWasChanged;
}

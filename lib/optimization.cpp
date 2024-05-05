#include "../include/optimization.h"

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

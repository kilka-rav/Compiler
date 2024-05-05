#include <iostream>
#include "module.h"
#include "type.h"
#include "optimization.h"

int main() {
    std::string name = "Constant folding";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto const1 = application.create<ConstantOperation>(0, 1, "I32");
    auto const2 = application.create<ConstantOperation>(1, 10, "I32");
    auto shift = application.create<ShiftOperation>(2, "Left", std::make_pair(b0, const1->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, shift->getIndex()));

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(shift);
    b0->insert(ret);
    application.print();
    ConstantFoldingOptimization opt1(&application);
    opt1.run();
    application.print();

}
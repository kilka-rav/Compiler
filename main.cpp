#include <iostream>
#include "module.h"
#include "type.h"
#include "optimization.h"

int main() {
    std::string name = "Peepholes";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto move = application.create<MoveOperation>(0, "arg0");
    auto const2 = application.create<ConstantOperation>(1, 0, "I32");
    auto mul = application.create<BinaryOperation>(2, "And", std::make_pair(b0, move->getIndex()), std::make_pair(b0, move->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, mul->getIndex()));

    b0->insert(move);
    b0->insert(const2);
    b0->insert(mul);
    b0->insert(ret);
    application.print();
    PeepholesOptimization opt1(&application);
    opt1.run();
    application.print();

}
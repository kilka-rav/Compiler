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
    auto b1 = application.create<BasicBlock>();

    b0->addSucessor({b1});
    b1->addPredessor({b0});

    auto move = application.create<MoveOperation>(0, "arg0");
    auto len = application.create<ConstantOperation>(1, 10, "I32");
    auto zeroCheck1 = application.create<ZeroCheckOperation>(2, move->getIndex());
    auto boundsCheck1 = application.create<BoundsCheckOperation>(3, move->getIndex(), len->getIndex());
    auto jump = application.create<JumpOperation>(4, b1);
    b0->insert(move);
    b0->insert(len);
    b0->insert(zeroCheck1);
    b0->insert(boundsCheck1);
    b0->insert(jump);

    auto zeroCheck2 = application.create<ZeroCheckOperation>(5, move->getIndex());
    auto boundsCheck2 = application.create<BoundsCheckOperation>(6, move->getIndex(), len->getIndex());
    auto sub = application.create<BinaryOperation>(7, "Add", std::make_pair(b0, move->getIndex()), std::make_pair(b0, len->getIndex()));
    auto ret = application.create<ReturnOperation>(8, std::make_pair(b1, sub->getIndex()));

    b1->insert(zeroCheck2);
    b1->insert(boundsCheck2);
    b1->insert(sub);
    b1->insert(ret);

    application.print();
    auto opt = CheckElimination(&application);
    opt.run();

    application.print();

}
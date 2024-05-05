#include <iostream>
#include "module.h"
#include "type.h"
#include "optimization.h"

int main() {
/*
    std::string name = "Constant folding";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto const1 = application.create<ConstantOperation>(0, 1, "I32");
    auto const2 = application.create<ConstantOperation>(1, 10, "I32");
    auto add = application.create<BinaryOperation>(2, "Add", std::make_pair(b0, const1->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, add->getIndex()));

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(add);
    b0->insert(ret);
    application.print();
    ConstantFoldingOptimization opt1(&application);
    opt1.run();
    application.print();
*/

    std::string name = "regAlloc";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("void");
    
    auto b0 = application.create<BasicBlock>();
    auto b1 = application.create<BasicBlock>();
    auto b2 = application.create<BasicBlock>();
    auto b3 = application.create<BasicBlock>();

    b0->addSucessor({b1});

    b1->addPredessor({b0, b2});
    b1->addSucessor({b2, b3});

    b2->addPredessor({b1});
    b2->addSucessor({b1});

    b3->addPredessor({b1});

    auto const1 = application.create<ConstantOperation>(0, 1, "I32");
    auto const2 = application.create<ConstantOperation>(1, 10, "I32");
    auto const3 = application.create<ConstantOperation>(2, 20, "I32");

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(const3);

    auto phi1 = application.create<PhiOperation>(3);
    phi1->addInputs({b0, b2}, {const1->getIndex(), 7});
    auto phi2 = application.create<PhiOperation>(4);
    phi2->addInputs({b0, b2}, {const2->getIndex(), 8});
    auto equal1 = application.create<CompareOperation>(5, "eq", std::make_pair(b1, phi2->getIndex()), std::make_pair(b0, const1->getIndex()));
    auto if1 = application.create<IfOperation>(6, b2, b3, std::make_pair(b1, equal1->getIndex()));

    b1->insert(phi1);
    b1->insert(phi2);
    b1->insert(equal1);
    b1->insert(if1);

    auto mul1 = application.create<BinaryOperation>(7, "Mul", std::make_pair(b1, phi1->getIndex()), std::make_pair(b1, phi2->getIndex()));
    auto sub1 = application.create<BinaryOperation>(8, "Sub", std::make_pair(b1, phi2->getIndex()), std::make_pair(b0, const1->getIndex()));
    
    b2->insert(mul1);
    b2->insert(sub1);

    auto add1 = application.create<BinaryOperation>(9, "Add", std::make_pair(b0, const3->getIndex()), std::make_pair(b1, phi1->getIndex()));
    auto ret1 = application.create<ReturnOperation>(10);

    b3->insert(add1);
    b3->insert(ret1);
    
    auto regAllocs = application.linearScanRegAlloc(2);
    regAllocs.print();
    std::cout << __LINE__ << std::endl;
}
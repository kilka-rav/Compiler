#include <iostream>
#include "module.h"
#include "type.h"


int main() {

    std::string name = "First task";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I64");
    
    /*
    auto b1 = BasicBlock(0, {0}, {1});
    auto b2 = BasicBlock(1, {0, 4}, {2, 3});
    auto b3 = BasicBlock(2, {1}, {2});
    auto b4 = BasicBlock(3, {1}, {4});
    auto b5 = BasicBlock(4, {3}, {1});

    application.insert(&b1);
    application.insert(&b2);
    application.insert(&b3);
    application.insert(&b4);
    application.insert(&b5);
    
    application.loopAnalyzer();
    application.printLoops();
*/
    auto* b1 = application.create<BasicBlock>();
    auto* b2 = application.create<BasicBlock>();
    auto* b3 = application.create<BasicBlock>();
    auto* b4 = application.create<BasicBlock>();
    b1->addSucessor({b2});
    b2->addPredessor({b1});
    b2->addSucessor({b3, b4});
    b3->addPredessor({b2});
    b4->addPredessor({b2});

    ConstantOperation c1 = ConstantOperation(0, 1, "I32");
    ConstantOperation c2 = ConstantOperation(1, 2, "I32");
    MoveOperation c13 = MoveOperation(2, "arg0");
    JumpOperation c3 = JumpOperation(3, b2);

    b1->insert(&c1);
    b1->insert(&c2);
    b1->insert(&c13);
    b1->insert(&c3);

    
    PhiOperation c4 = PhiOperation(4, {b1, b3}, {c2.getIndex(), 7});
    CompareOperation c5 = CompareOperation(5, "le", std::make_pair(b2, c4.getIndex()), std::make_pair(b1, c13.getIndex()));
    PhiOperation c6 = PhiOperation(6, {b1, b3}, {c1.getIndex(), 9});
    IfOperation c7 = IfOperation(7, b3, b4, std::make_pair(b2, c5.getIndex()));
    b2->insert(&c4);
    b2->insert(&c5);
    b2->insert(&c6);
    b2->insert(&c7);

    ConstantOperation c8 = ConstantOperation(8, 1, "I32");
    BinaryOperation c9 = BinaryOperation(9, "Add", std::make_pair(b2, c4.getIndex()), std::make_pair(b3, c7.getIndex()));
    CastOperation c10 = CastOperation(10, std::make_pair(b3, c9.getIndex()), "I64");
    BinaryOperation c11 = BinaryOperation(11, "Mul", std::make_pair(b2, c6.getIndex()), std::make_pair(b3, c10.getIndex()));
    auto c12 = JumpOperation(12, b2);

    b3->insert(&c8);
    b3->insert(&c9);
    b3->insert(&c10);
    b3->insert(&c11);
    b3->insert(&c12);

    ReturnOperation c14 = ReturnOperation(13, std::make_pair(b2, c6.getIndex()));

    b4->insert(&c14);
    application.print();
    delete b1;
    delete b2;
    delete b3;
    delete b4;
}
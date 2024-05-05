#include <gtest/gtest.h>
#include "./../include/module.h"
#include "./../include/type.h"

TEST(Factorial, IRTest) {
    /*
Module: Factorial(%arg0 : I32, ) -> I64 {
        Basic Block: id: 0 prev ids:  next ids: 1  {
          %0.I32 Constant 1
          %1.I32 Constant 2
          %2 Move %arg0
          %3 Jump -> Basic Block 1
        }
        Basic Block: id: 1 prev ids: 0  next ids: 2 3  {
          %4 Phi [0 : %1], [2 : %7], 
          %5 Cmp le : [1 : %4], [0 : %2]
          %6 Phi [0 : %0], [2 : %9], 
          %7 If [1 : %5] -> 2, 3
        }
        Basic Block: id: 2 prev ids: 1  next ids:  {
          %8.I32 Constant 1
          %9 Add  : [1 : %4], [2 : %7]
          %10 Cast %[2: 9] -> I64
          %11 Mul  : [1 : %6], [2 : %10]
          %12 Jump -> Basic Block 1
        }
        Basic Block: id: 3 prev ids: 1  next ids:  {
          %13 Return [1: %6]
        }
}

    */

    std::string name = "Factorial";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I64");

    auto* b1 = application.create<BasicBlock>();
    auto* b2 = application.create<BasicBlock>();
    auto* b3 = application.create<BasicBlock>();
    auto* b4 = application.create<BasicBlock>();
    b1->addSucessor({b2});
    b2->addPredessor({b1});
    b2->addSucessor({b3, b4});
    b3->addPredessor({b2});
    b4->addPredessor({b2});

    auto c1 = application.create<ConstantOperation>(0, 1, "I32");
    auto c2 = application.create<ConstantOperation>(1, 2, "I32");
    auto c13 = application.create<MoveOperation>(2, "arg0");
    auto c3 = application.create<JumpOperation>(3, b2);

    b1->insert(c1);
    b1->insert(c2);
    b1->insert(c13);
    b1->insert(c3);

    
    auto c4 = application.create<PhiOperation>(4);
    c4->addInputs({b1, b3}, {c2->getIndex(), 7});
    auto c5 = application.create<CompareOperation>(5, "le", std::make_pair(b2, c4->getIndex()), std::make_pair(b1, c13->getIndex()));
    auto c6 = application.create<PhiOperation>(6);
    c6->addInputs({b1, b3}, {c1->getIndex(), 9});
    auto c7 = application.create<IfOperation>(7, b3, b4, std::make_pair(b2, c5->getIndex()));
    b2->insert(c4);
    b2->insert(c5);
    b2->insert(c6);
    b2->insert(c7);

    auto c8 = application.create<ConstantOperation>(8, 1, "I32");
    auto c9 = application.create<BinaryOperation>(9, "Add", std::make_pair(b2, c4->getIndex()), std::make_pair(b3, c7->getIndex()));
    auto c10 = application.create<CastOperation>(10, std::make_pair(b3, c9->getIndex()), "I64");
    auto c11 = application.create<BinaryOperation>(11, "Mul", std::make_pair(b2, c6->getIndex()), std::make_pair(b3, c10->getIndex()));
    auto c12 = application.create<JumpOperation>(12, b2);

    b3->insert(c8);
    b3->insert(c9);
    b3->insert(c10);
    b3->insert(c11);
    b3->insert(c12);

    auto c14 = application.create<ReturnOperation>(13, std::make_pair(b2, c6->getIndex()));

    b4->insert(c14);
    //application.print();
    
}
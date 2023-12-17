#include <gtest/gtest.h>
#include "./../include/module.h"
#include "./../include/type.h"
#include "./../include/operation.h"

TEST(Factorial, IRTest) {
    /*
Module: Factorial(%arg0 : I32, ) -> I64 {
        Basic Block: id: 0 prev ids: 0  next ids: 1  {
          %0.I32 Constant 1
          %1.I32 Constant 2
          %2 Move %arg0
          %3 Jump -> Basic Block 1
        }
        Basic Block: id: 1 prev ids: 1  next ids: 2 3  {
          %4 Phi 0->%1, 2->%9
          %5 Cmp le : %4, %2
          %6 Phi 0->%0, 2->%11
          %7 If %5 : BasicBlock 2, BasicBlock 3
        }
        Basic Block: id: 2 prev ids: 1  next ids: 1  {
          %8.I32 Constant 1
          %9 Add  : %4, %8
          %10 Cast %9 -> I64
          %11 Mul  : %6, %10
          %12 Jump -> Basic Block 1
        }
        Basic Block: id: 3 prev ids: 3  next ids: 3  {
          %13 Return %6
        }
}

    */

   /*
    std::string name = "Factorial";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I64");
    EXPECT_EQ("Factorial", application.getName());
    auto b1 = BasicBlock(0, {0}, {1});
    auto b2 = BasicBlock(1, {0}, {2, 3});
    auto b3 = BasicBlock(2, {1}, {1});
    auto b4 = BasicBlock(3, {3}, {3});
    
    EXPECT_EQ(b1.getNext()[0], b2.getID());
    EXPECT_EQ(b2.getPrev()[0], b1.getID());
    EXPECT_EQ(b2.getNext()[0], b3.getID());
    EXPECT_EQ(b2.getNext()[1], b4.getID());

    application.insert(&b1);
    application.insert(&b2);
    application.insert(&b3);
    application.insert(&b4);

    
    ConstantOperation c1 = ConstantOperation(0, 1, "I32");
    ConstantOperation c2 = ConstantOperation(1, 2, "I32");
    MoveOperation c13 = MoveOperation(2, "arg0");
    JumpOperation c3 = JumpOperation(3, b2.getID());
    b1.insert(&c1);
    b1.insert(&c2);
    b1.insert(&c13);
    b1.insert(&c3);
    
    PhiOperation c4 = PhiOperation(4, b1.getID(), c2.getIndex(), b3.getID(), 7);
    CompareOperation c5 = CompareOperation(5, "le", 4, 2);
    PhiOperation c6 = PhiOperation(6, b1.getID(), c1.getIndex(), b3.getID(), 9);
    IfOperation c7 = IfOperation(7, 2, 3, 5);
    b2.insert(&c4);
    b2.insert(&c5);
    b2.insert(&c6);
    b2.insert(&c7);
    
    ConstantOperation c8 = ConstantOperation(8, 1, "I32");
    BinaryOperation c9 = BinaryOperation(9, "Add", c4.getIndex(), c8.getIndex());
    CastOperation c10 = CastOperation(10, c9.getIndex(), "I64");
    BinaryOperation c11 = BinaryOperation(11, "Mul", c6.getIndex(), c10.getIndex());
    b3.insert(&c8);
    b3.insert(&c9);
    b3.insert(&c10);
    b3.insert(&c11);
    
    ReturnOperation c12 = ReturnOperation(12, c6.getIndex());
    b4.insert(&c12);
    //application.print();
    */
}
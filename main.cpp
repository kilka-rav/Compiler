#include <iostream>
#include "module.h"
#include "type.h"
#include "operation.h"

int main() {
    
    std::string name = "Factorial";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I64");
    std::cout << application.getName() << std::endl;
    auto b1 = BasicBlock(0, {0}, {1});
    auto b2 = BasicBlock(1, {1}, {2, 3});
    auto b3 = BasicBlock(2, {1}, {1});
    auto b4 = BasicBlock(3, {3}, {3});
    
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
    
    PhiOperation c4 = PhiOperation(4, b1.getID(), c2.getIndex(), b3.getID(), 9);
    CompareOperation c5 = CompareOperation(5, "le", 4, 2);
    PhiOperation c6 = PhiOperation(6, b1.getID(), c1.getIndex(), b3.getID(), 11);
    IfOperation c7 = IfOperation(7, 2, 3, 5);
    b2.insert(&c4);
    b2.insert(&c5);
    b2.insert(&c6);
    b2.insert(&c7);
    
    ConstantOperation c8 = ConstantOperation(8, 1, "I32");
    BinaryOperation c9 = BinaryOperation(9, "Add", c4.getIndex(), c8.getIndex());
    CastOperation c10 = CastOperation(10, c9.getIndex(), "I64");
    BinaryOperation c11 = BinaryOperation(11, "Mul", c6.getIndex(), c10.getIndex());
    JumpOperation c12 = JumpOperation(12, b2.getID());
    b3.insert(&c8);
    b3.insert(&c9);
    b3.insert(&c10);
    b3.insert(&c11);
    b3.insert(&c12);
    
    ReturnOperation c14 = ReturnOperation(13, c6.getIndex());
    b4.insert(&c14);
    
    application.print();

    application.printDFS();
    application.DFS();
    std::cout << "After DFS" << std::endl;
    application.printDFS();

}
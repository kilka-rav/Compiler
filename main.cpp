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
    auto* b1 = new BasicBlock(0, 0, 1);
    auto* b2 = new BasicBlock(0, 1, {2, 3});
    auto* b3 = new BasicBlock(1, 2, 1);
    auto* b4 = new BasicBlock(1, 3, 3);
    application.insert(b1);
    application.insert(b2);
    application.insert(b3);
    application.insert(b4);
    ConstantOperation* c1 = new ConstantOperation(0, 1, "I32");
    ConstantOperation* c2 = new ConstantOperation(1, 2, "I32");
    MoveOperation* c13 = new MoveOperation(2, "arg0");
    JumpOperation* c3 = new JumpOperation(3, b2->getID());
    b1->insert(c1);
    b1->insert(c2);
    b1->insert(c13);
    b1->insert(c3);
    PhiOperation* c4 = new PhiOperation(4, 0, 1, 2, 9);
    CompareOperation* c5 = new CompareOperation(5, "le", 3, 2);
    PhiOperation* c6 = new PhiOperation(6, 0, 0, 0, 0);
    IfOperation* c7 = new IfOperation(7, 2, 3, 5);
    b2->insert(c4);
    b2->insert(c5);
    b2->insert(c6);
    b2->insert(c7);

    ConstantOperation* c8 = new ConstantOperation(8, 1, "I32");
    BinaryOperation* c9 = new BinaryOperation(9, "Add", 0, 0);
    CastOperation* c10 = new CastOperation(10, 0, "I64");
    BinaryOperation* c11 = new BinaryOperation(11, "Mul", 0, 0);
    b3->insert(c8);
    b3->insert(c9);
    b3->insert(c10);
    b3->insert(c11);

    ReturnOperation* c12 = new ReturnOperation(12, 0);
    b4->insert(c12);
    application.print();
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    delete c5;
    delete c6;
    delete c7;
    delete c8;
    delete c9;
    delete c10;
    delete c11;
    delete c12;
    delete c13;
    delete b1;
    delete b2;
    delete b3;
    delete b4;

    /*
    std::cout << __LINE__ << " " << __FILE__ << std::endl;
    ConstantOperation c3(1, 321);
    c3.print();
    std::cout << __LINE__ << " " << __FILE__ << std::endl;
    application.insert(c3);
    std::cout << __LINE__ << " " << __FILE__ << std::endl;
    application.print();
    */
}
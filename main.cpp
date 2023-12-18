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

    



    application.print();
    delete b1;
    delete b2;
    delete b3;
    delete b4;
}
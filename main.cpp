#include <iostream>
#include "module.h"
#include "type.h"


int main() {

    std::string name = "First task";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I64");
    
    auto b1 = application.create<BasicBlock>();
    auto b2 = application.create<BasicBlock>();
    auto b3 = application.create<BasicBlock>();
    auto b4 = application.create<BasicBlock>();
    auto b5 = application.create<BasicBlock>();
    auto b6 = application.create<BasicBlock>();
    auto b7 = application.create<BasicBlock>();
    auto b8 = application.create<BasicBlock>();

    
    b1->addSucessor({b2});
    b1->addPredessor({b8});

    b2->addPredessor({b1, b7});
    b2->addSucessor({b3, b4});

    b3->addPredessor({b2});
    b3->addSucessor({b5, b6});

    b4->addPredessor({b2});
    b4->addSucessor({b6});

    b5->addPredessor({b3});

    b6->addPredessor({b3, b4});
    b6->addSucessor({b7});

    b7->addPredessor({b6});
    b7->addSucessor({b8, b2});

    b8->addPredessor({b7});
    b8->addSucessor({b1});

    application.loopAnalyzer();
    application.printLoops();
    
}
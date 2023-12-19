#include <iostream>
#include "module.h"
#include "type.h"


int main() {

   std::string name = "DFS";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I64");
    std::cout << application.getName() << std::endl;
    
    auto b1 = application.create<BasicBlock>();

    auto b2 = application.create<BasicBlock>();
    auto b3 = application.create<BasicBlock>();
    auto b4 = application.create<BasicBlock>();

    auto b5 = application.create<BasicBlock>();
    auto b6 = application.create<BasicBlock>();

    auto b7 = application.create<BasicBlock>();
    auto b8 = application.create<BasicBlock>();

    auto b9 = application.create<BasicBlock>();
    auto b10 = application.create<BasicBlock>();

    auto b11 = application.create<BasicBlock>();
    auto b12 = application.create<BasicBlock>();
    
    b1->addSucessor({b2, b3, b4});

    b2->addPredessor({b1});
    b2->addSucessor({b5, b6});
    b3->addPredessor({b1});
    b4->addPredessor({b1});
    b4->addSucessor({b7, b8});
    
    b5->addPredessor({b2});
    b5->addSucessor({b9, b10});

    b6->addPredessor({b2});

    b7->addPredessor({b4});
    b7->addSucessor({b11, b12});

    b8->addPredessor({b4});

    b9->addPredessor({b5});

    b10->addPredessor({b5});
    b11->addPredessor({b7});
    b12->addPredessor({b7});



    application.printDFS();
    application.print();
    std::cout << __LINE__ << std::endl;
    application.DFS();
    std::cout << "After DFS" << std::endl;
    application.printDFS();
    application.buildDomtree();

}
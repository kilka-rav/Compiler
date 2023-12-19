#include <gtest/gtest.h>
#include "./../include/module.h"
#include "./../include/type.h"


/*
id: 0 numDFS: -1 prev ids: 0  next ids: 1 2 3 
id: 1 numDFS: -1 prev ids: 0  next ids: 4 5 
id: 2 numDFS: -1 prev ids: 0  next ids: 2 
id: 3 numDFS: -1 prev ids: 0  next ids: 6 7 
id: 4 numDFS: -1 prev ids: 1  next ids: 8 9 
id: 5 numDFS: -1 prev ids: 1  next ids: 5 
id: 6 numDFS: -1 prev ids: 3  next ids: 10 11 
id: 7 numDFS: -1 prev ids: 3  next ids: 7 
id: 8 numDFS: -1 prev ids: 4  next ids: 8 
id: 9 numDFS: -1 prev ids: 4  next ids: 9 
id: 10 numDFS: -1 prev ids: 6  next ids: 10 
id: 11 numDFS: -1 prev ids: 6  next ids: 11 
After DFS
id: 0 numDFS: 1 prev ids: 0  next ids: 1 2 3 
id: 1 numDFS: 2 prev ids: 0  next ids: 4 5 
id: 2 numDFS: 7 prev ids: 0  next ids: 2 
id: 3 numDFS: 8 prev ids: 0  next ids: 6 7 
id: 4 numDFS: 3 prev ids: 1  next ids: 8 9 
id: 5 numDFS: 6 prev ids: 1  next ids: 5 
id: 6 numDFS: 9 prev ids: 3  next ids: 10 11 
id: 7 numDFS: 12 prev ids: 3  next ids: 7 
id: 8 numDFS: 4 prev ids: 4  next ids: 8 
id: 9 numDFS: 5 prev ids: 4  next ids: 9 
id: 10 numDFS: 10 prev ids: 6  next ids: 10 
id: 11 numDFS: 11 prev ids: 6  next ids: 11

*/

TEST(Dominators, DFS) {
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

    //application.printDFS();
    application.DFS();
    //std::cout << "After DFS" << std::endl;
    //application.printDFS();
}
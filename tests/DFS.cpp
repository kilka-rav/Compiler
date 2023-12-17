#include <gtest/gtest.h>
#include "./../include/module.h"
#include "./../include/type.h"
#include "./../include/operation.h"


TEST(Marked, DFSTest) {
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

/*
    std::string name = "DFS";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I64");
    std::cout << application.getName() << std::endl;
    
    auto b1 = BasicBlock(0, {0}, {1, 2, 3});
    auto b2 = BasicBlock(1, {0}, {4, 5});
    auto b3 = BasicBlock(2, {0}, {2});
    auto b4 = BasicBlock(3, {0}, {6, 7});

    auto b5 = BasicBlock(4, {1}, {8, 9});
    auto b6 = BasicBlock(5, {1}, {5});

    auto b7 = BasicBlock(6, {3}, {10, 11});
    auto b8 = BasicBlock(7, {3}, {7});

    auto b9 = BasicBlock(8, {4}, {8});
    auto b10 = BasicBlock(9, {4}, {9});

    auto b11 = BasicBlock(10, {6}, {10});
    auto b12 = BasicBlock(11, {6}, {11});
    
    application.insert(&b1);
    application.insert(&b2);
    application.insert(&b3);
    application.insert(&b4);

    application.insert(&b5);
    application.insert(&b6);
    application.insert(&b7);
    application.insert(&b8);

    application.insert(&b9);
    application.insert(&b10);
    application.insert(&b11);
    application.insert(&b12);

    //application.printDFS();
    application.DFS();
    std::cout << "After DFS" << std::endl;
    //application.printDFS();
    */
}
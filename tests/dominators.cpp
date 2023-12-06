#include <gtest/gtest.h>

#include "./../include/module.h"
#include "./../include/type.h"
#include "./../include/operation.h"


int main() {

    std::string name = "CheckDominators";

    Module application(name);
    std::cout << application.getName() << std::endl;
    application.setArgument("I32");
    application.setReturnType("I64");
    
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

    for(int i = 1; i < 12; ++i) {
        EXPECT_TRUE(application.isDominator(b1.getID(), i));
    }
    EXPECT_TRUE(application.isDominator(b1.getID(), b2.getID()));
    EXPECT_FALSE(application.isDominator(b2.getID(), b12.getID()));
    EXPECT_FALSE(application.isDominator(b2.getID(), b3.getID()));
    EXPECT_TRUE(application.isDominator(b2.getID(), b9.getID()));
    EXPECT_FALSE(application.isDominator(b6.getID(), b9.getID()));
    EXPECT_FALSE(application.isDominator(b9.getID(), b6.getID()));
    EXPECT_FALSE(application.isDominator(b9.getID(), b9.getID()));
    
}
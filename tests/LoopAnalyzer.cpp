#include <gtest/gtest.h>
#include "./../include/module.h"
#include "./../include/type.h"



TEST(LoopAnalyzer, TEST1) {

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
    
    b1->addSucessor({b2});

    b2->addPredessor({b1, b5});
    b2->addSucessor({b3, b4});

    b3->addPredessor({b2});

    b4->addPredessor({b2});
    b4->addSucessor({b5});

    b5->addPredessor({b4});
    b5->addSucessor({b2});

    application.loopAnalyzer();
    
}

TEST(LoopAnalyzer, TEST2) {
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
    
    b1->addSucessor({b2});

    b2->addPredessor({b1, b5});
    b2->addSucessor({b3});

    b3->addPredessor({b2});
    b3->addSucessor({b4, b6});

    b4->addPredessor({b3});
    b4->addSucessor({b5, b6});

    b5->addPredessor({b4});
    b5->addSucessor({b2});

    b6->addPredessor({b3, b4});

    application.loopAnalyzer();
}
#include <gtest/gtest.h>
#include "module.h"
#include "type.h"
#include "optimization.h"

TEST(inlineTest, test1) {
        std::string name = "Caller_graph";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";

    std::string name2 = "Callee_graph";
    Module callee(name2);
    callee.setArgument("I32");
    callee.setArgument("I32");
    callee.setReturnType("I32");

    auto b0 = application.create<BasicBlock>(0);
    auto b1 = application.create<BasicBlock>(1);

    b0->addSucessor({b1});
    b1->addPredessor({b0});


    auto c1 = application.create<ConstantOperation>(0, 10, "I32");
    auto c2 = application.create<ConstantOperation>(1, 5, "I32");

    b0->insert(c1);
    b0->insert(c2);

    auto move1 = application.create<MoveOperation>(2, "arg0");
    auto move2 = application.create<MoveOperation>(3, "arg1");
    auto add1 = application.create<BinaryOperation>(4, "Add", std::make_pair(b0, c1->getIndex()), std::make_pair(b1, move1->getIndex()));
    auto add2 = application.create<BinaryOperation>(5, "Add", std::make_pair(b0, c2->getIndex()), std::make_pair(b1, move2->getIndex()));
    auto call = application.create<CallStaticOperation>(6, &callee);
    call->setOperands({add1->getIndex(), add2->getIndex()});

    auto ret = application.create<ReturnOperation>(7, std::make_pair(b1, call->getIndex()));

    b1->insert(move1);
    b1->insert(move2);
    b1->insert(add1);
    b1->insert(add2);
    b1->insert(call);
    b1->insert(ret);
    
    auto b2 = callee.create<BasicBlock>(2);
    auto b3 = callee.create<BasicBlock>(3);
    auto b4 = callee.create<BasicBlock>(4);
    auto b5 = callee.create<BasicBlock>(5);

    b2->addSucessor({b3});

    b3->addPredessor({b2});
    b3->addSucessor({b4, b5});

    b4->addPredessor({b3});

    b5->addPredessor({b3});

    auto move3 = application.create<MoveOperation>(11, 0);
    auto move4 = application.create<MoveOperation>(12, 1);
    auto const3 = application.create<ConstantOperation>(13, 1, "I32");

    b2->insert(move3);
    b2->insert(move4);
    b2->insert(const3);

    auto use2 = application.create<BinaryOperation>(14, "Mul", std::make_pair(b2, move3->getIndex()), std::make_pair(b2, const3->getIndex()));
    auto use3 = application.create<BinaryOperation>(15, "Mul", std::make_pair(b2, move4->getIndex()), std::make_pair(b2, const3->getIndex()));
    auto cmp = application.create<CompareOperation>(16, "GEQ", std::make_pair(b3, use2->getIndex()), std::make_pair(b3, use3->getIndex()));
    auto ifOp = application.create<IfOperation>(17, b4, b5, std::make_pair(b3, cmp->getIndex()));

    b3->insert(use2);
    b3->insert(use3);
    b3->insert(cmp);
    b3->insert(ifOp);

    auto ret1 = application.create<ReturnOperation>(18, std::make_pair(b3, use3->getIndex()));
    auto ret2 = application.create<ReturnOperation>(19, std::make_pair(b3, use2->getIndex()));

    b4->insert(ret1);
    b5->insert(ret2);

    StaticInline opt(&application);
    opt.run();

    application.print();

    EXPECT_EQ(application.getBBs().size(), 7);
    EXPECT_EQ(application.getBBs()[6]->getOps()[0]->getName(), "Phi");
    EXPECT_EQ(application.getBBs()[1]->getOps()[4]->getName(), "Jump");
}
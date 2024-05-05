#include <gtest/gtest.h>
#include "module.h"
#include "type.h"
#include "optimization.h"

TEST(CheckElimination, test) {
    std::string name = "CheckElimination";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto b1 = application.create<BasicBlock>();

    b0->addSucessor({b1});
    b1->addPredessor({b0});

    auto move = application.create<MoveOperation>(0, "arg0");
    auto len = application.create<ConstantOperation>(1, 10, "I32");
    auto zeroCheck1 = application.create<ZeroCheckOperation>(2, move->getIndex());
    auto boundsCheck1 = application.create<BoundsCheckOperation>(3, move->getIndex(), len->getIndex());
    auto jump = application.create<JumpOperation>(4, b1);
    b0->insert(move);
    b0->insert(len);
    b0->insert(zeroCheck1);
    b0->insert(boundsCheck1);
    b0->insert(jump);

    auto zeroCheck2 = application.create<ZeroCheckOperation>(5, move->getIndex());
    auto boundsCheck2 = application.create<BoundsCheckOperation>(6, move->getIndex(), len->getIndex());
    auto sub = application.create<BinaryOperation>(7, "Add", std::make_pair(b0, move->getIndex()), std::make_pair(b0, len->getIndex()));
    auto ret = application.create<ReturnOperation>(8, std::make_pair(b1, sub->getIndex()));

    b1->insert(zeroCheck2);
    b1->insert(boundsCheck2);
    b1->insert(sub);
    b1->insert(ret);

    application.print();

    auto different_elimination = application.getBBs()[1]->getOps().size();
    auto opt = CheckElimination(&application);
    opt.run();

    auto opResult = application.getBBs()[0]->getOps()[2];
    auto opResult2 = application.getBBs()[0]->getOps()[3];

    auto zeroCheck = dynamic_cast<ZeroCheckOperation*>(opResult);
    auto boundsCheck = dynamic_cast<BoundsCheckOperation*>(opResult2);
    EXPECT_NE(zeroCheck, nullptr);
    EXPECT_EQ(zeroCheck->getName(), "ZeroCheck");
    EXPECT_EQ(zeroCheck->getOperands()[0], 0);

    EXPECT_NE(boundsCheck, nullptr);
    EXPECT_EQ(boundsCheck->getName(), "BoundsCheck");
    EXPECT_EQ(boundsCheck->getOperands()[0], 0);
    EXPECT_EQ(boundsCheck->getOperands()[1], 1);    

    different_elimination -= application.getBBs()[1]->getOps().size();

    EXPECT_EQ(different_elimination, 2);

    application.print();

}
#include <gtest/gtest.h>
#include "module.h"
#include "type.h"
#include "optimization.h"

TEST(AddFoldingDCE, test) {
    std::string name = "Constant folding";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto const1 = application.create<ConstantOperation>(0, 1, "I32");
    auto const2 = application.create<ConstantOperation>(1, 10, "I32");
    auto add = application.create<BinaryOperation>(2, "Add", std::make_pair(b0, const1->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, add->getIndex()));

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(add);
    b0->insert(ret);
    ConstantFoldingOptimization opt1(&application);
    opt1.run();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto const5 = dynamic_cast<ConstantOperation*>(opResult);
    EXPECT_NE(const5, nullptr);
    EXPECT_EQ(const5->getValue(), 11);
    EXPECT_EQ(const5->getDType(), "I32");
}

TEST(MulFoldingDCE, test2) {
    std::string name = "Constant folding";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto const1 = application.create<ConstantOperation>(0, 2, "I32");
    auto const2 = application.create<ConstantOperation>(1, 7, "I32");
    auto add = application.create<BinaryOperation>(2, "Mul", std::make_pair(b0, const1->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, add->getIndex()));

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(add);
    b0->insert(ret);
    ConstantFoldingOptimization opt1(&application);
    opt1.run();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto const5 = dynamic_cast<ConstantOperation*>(opResult);
    EXPECT_NE(const5, nullptr);
    EXPECT_EQ(const5->getValue(), 14);
    EXPECT_EQ(const5->getDType(), "I32");
}
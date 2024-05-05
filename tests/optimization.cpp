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

TEST(ShiftLeftFoldingDCE, test3) {
        std::string name = "Constant folding";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto const1 = application.create<ConstantOperation>(0, 1, "I32");
    auto const2 = application.create<ConstantOperation>(1, 10, "I32");
    auto shift = application.create<ShiftOperation>(2, "Left", std::make_pair(b0, const1->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, shift->getIndex()));

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(shift);
    b0->insert(ret);
    application.print();
    ConstantFoldingOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto const5 = dynamic_cast<ConstantOperation*>(opResult);
    EXPECT_NE(const5, nullptr);
    EXPECT_EQ(const5->getValue(), 1024);
    EXPECT_EQ(const5->getDType(), "I32");
}

TEST(ShiftRightFoldingDCE, test3) {
        std::string name = "Constant folding";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto const1 = application.create<ConstantOperation>(0, 1024, "I32");
    auto const2 = application.create<ConstantOperation>(1, 10, "I32");
    auto shift = application.create<ShiftOperation>(2, "Right", std::make_pair(b0, const1->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, shift->getIndex()));

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(shift);
    b0->insert(ret);
    application.print();
    ConstantFoldingOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto const5 = dynamic_cast<ConstantOperation*>(opResult);
    EXPECT_NE(const5, nullptr);
    EXPECT_EQ(const5->getValue(), 1);
    EXPECT_EQ(const5->getDType(), "I32");
}

TEST(AndFoldersPattern, test4) {
    std::string name = "Constant folding";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto const1 = application.create<ConstantOperation>(0, 15, "I32");
    auto const2 = application.create<ConstantOperation>(1, 19, "I32");
    auto And = application.create<BinaryOperation>(2, "And", std::make_pair(b0, const1->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, And->getIndex()));

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(And);
    b0->insert(ret);
    application.print();
    ConstantFoldingOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto const5 = dynamic_cast<ConstantOperation*>(opResult);
    EXPECT_NE(const5, nullptr);
    EXPECT_EQ(const5->getValue(), 3);
    EXPECT_EQ(const5->getDType(), "I32");
}

TEST(PeepholesOptimization, MulZero) {
    std::string name = "Peepholes";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto move = application.create<MoveOperation>(0, "arg0");
    auto const2 = application.create<ConstantOperation>(1, 0, "I32");
    auto mul = application.create<BinaryOperation>(2, "Mul", std::make_pair(b0, move->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, mul->getIndex()));

    b0->insert(move);
    b0->insert(const2);
    b0->insert(mul);
    b0->insert(ret);
    application.print();
    PeepholesOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto const5 = dynamic_cast<ConstantOperation*>(opResult);
    EXPECT_NE(const5, nullptr);
    EXPECT_EQ(const5->getValue(), 0);
    EXPECT_EQ(const5->getDType(), "I32");
}

TEST(PeepholesOptimization, MulOne) {
    std::string name = "Peepholes";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto move = application.create<MoveOperation>(0, "arg0");
    auto const2 = application.create<ConstantOperation>(1, 1, "I32");
    auto mul = application.create<BinaryOperation>(2, "Mul", std::make_pair(b0, move->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, mul->getIndex()));

    b0->insert(move);
    b0->insert(const2);
    b0->insert(mul);
    b0->insert(ret);
    application.print();
    PeepholesOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto opResult1 = application.getBBs()[0]->getOps()[1];
    auto c1 = dynamic_cast<MoveOperation*>(opResult);
    auto c2 = dynamic_cast<ReturnOperation*>(opResult1);
    EXPECT_NE(c1, nullptr);
    EXPECT_NE(c2, nullptr);
    EXPECT_EQ(c2->getOperands()[0], c1->getIndex());   
}

TEST(PeepholesOptimization, ShiftZero) {
    std::string name = "Peepholes";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto move = application.create<MoveOperation>(0, "arg0");
    auto const2 = application.create<ConstantOperation>(1, 0, "I32");
    auto shift = application.create<ShiftOperation>(2, "Left", std::make_pair(b0, move->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, shift->getIndex()));

    b0->insert(move);
    b0->insert(const2);
    b0->insert(shift);
    b0->insert(ret);
    application.print();
    PeepholesOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto opResult1 = application.getBBs()[0]->getOps()[1];
    auto c1 = dynamic_cast<MoveOperation*>(opResult);
    auto c2 = dynamic_cast<ReturnOperation*>(opResult1);
    EXPECT_NE(c1, nullptr);
    EXPECT_NE(c2, nullptr);
    EXPECT_EQ(c2->getOperands()[0], c1->getIndex());
}

TEST(PeepholesOptimization, MulDecomposeShift) {
        std::string name = "Peepholes";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto move = application.create<MoveOperation>(0, "arg0");
    auto const2 = application.create<ConstantOperation>(1, 4, "I32");
    auto mul = application.create<BinaryOperation>(2, "Mul", std::make_pair(b0, move->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, mul->getIndex()));

    b0->insert(move);
    b0->insert(const2);
    b0->insert(mul);
    b0->insert(ret);
    application.print();
    PeepholesOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto opResult1 = application.getBBs()[0]->getOps()[1];
    auto opResult2 = application.getBBs()[0]->getOps()[2];
    auto c1 = dynamic_cast<MoveOperation*>(opResult);
    auto c2 = dynamic_cast<ConstantOperation*>(opResult1);
    auto c3 = dynamic_cast<ShiftOperation*>(opResult2);
    EXPECT_NE(c1, nullptr);
    EXPECT_NE(c2, nullptr);
    EXPECT_NE(c3, nullptr);
    EXPECT_EQ(c2->getValue(), 2);
    EXPECT_EQ(c3->getDirection(), "Left");
    EXPECT_EQ(c3->getA().second, c1->getIndex());
}

TEST(PeepholesOptimization, AndZeroPattern) {
    std::string name = "Peepholes";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto move = application.create<MoveOperation>(0, "arg0");
    auto const2 = application.create<ConstantOperation>(1, 0, "I32");
    auto mul = application.create<BinaryOperation>(2, "And", std::make_pair(b0, move->getIndex()), std::make_pair(b0, const2->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, mul->getIndex()));

    b0->insert(move);
    b0->insert(const2);
    b0->insert(mul);
    b0->insert(ret);
    application.print();
    PeepholesOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto c2 = dynamic_cast<ConstantOperation*>(opResult);
    EXPECT_NE(c2, nullptr);
    EXPECT_EQ(c2->getValue(), 0);
}

TEST(PeepholesOptimization, AndEqualPattern) {
    std::string name = "Peepholes";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("I32");
    std::string dtype = "I32";
    auto b0 = application.create<BasicBlock>();
    auto move = application.create<MoveOperation>(0, "arg0");
    auto const2 = application.create<ConstantOperation>(1, 0, "I32");
    auto mul = application.create<BinaryOperation>(2, "And", std::make_pair(b0, move->getIndex()), std::make_pair(b0, move->getIndex()));
    auto ret = application.create<ReturnOperation>(3, std::make_pair(b0, mul->getIndex()));

    b0->insert(move);
    b0->insert(const2);
    b0->insert(mul);
    b0->insert(ret);
    application.print();
    PeepholesOptimization opt1(&application);
    opt1.run();
    application.print();
    auto opResult = application.getBBs()[0]->getOps()[0];
    auto opResult1 = application.getBBs()[0]->getOps()[1];
    auto c1 = dynamic_cast<MoveOperation*>(opResult);
    auto c2 = dynamic_cast<ReturnOperation*>(opResult1);
    EXPECT_NE(c1, nullptr);
    EXPECT_NE(c2, nullptr);
    EXPECT_EQ(c2->getOperands()[0], c1->getIndex());   
}
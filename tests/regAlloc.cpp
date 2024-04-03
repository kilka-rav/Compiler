#include <gtest/gtest.h>
#include "./../include/module.h"
#include "./../include/type.h"

static bool isRegister(Operation* op, AllocInfo& info) {
    return info.getReg(op) >= 0;
}

static bool isStack(Operation* op, AllocInfo& info) {
    return info.getReg(op) < 0;
}

TEST(regAlloc, test3) {
    
    std::string name = "regAlloc";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("void");
    
    auto b0 = application.create<BasicBlock>();
    auto b1 = application.create<BasicBlock>();
    auto b2 = application.create<BasicBlock>();
    auto b3 = application.create<BasicBlock>();

    b0->addSucessor({b1});

    b1->addPredessor({b0, b2});
    b1->addSucessor({b2, b3});

    b2->addPredessor({b1});
    b2->addSucessor({b1});

    b3->addPredessor({b1});

    ConstantOperation const1 = ConstantOperation(0, 1, "I32");
    ConstantOperation const2 = ConstantOperation(1, 10, "I32");
    ConstantOperation const3 = ConstantOperation(2, 20, "I32");

    b0->insert(&const1);
    b0->insert(&const2);
    b0->insert(&const3);

    PhiOperation phi1 = PhiOperation(3, {b0, b2}, {const1.getIndex(), 7});
    PhiOperation phi2 = PhiOperation(4, {b0, b2}, {const2.getIndex(), 8});
    CompareOperation equal1 = CompareOperation(5, "eq", std::make_pair(b1, phi2.getIndex()), std::make_pair(b0, const1.getIndex()));
    IfOperation if1 = IfOperation(6, b2, b3, std::make_pair(b1, equal1.getIndex()));

    b1->insert(&phi1);
    b1->insert(&phi2);
    b1->insert(&equal1);
    b1->insert(&if1);

    BinaryOperation mul1 = BinaryOperation(7, "Mul", std::make_pair(b1, phi1.getIndex()), std::make_pair(b1, phi2.getIndex()));
    BinaryOperation sub1 = BinaryOperation(8, "Sub", std::make_pair(b1, phi2.getIndex()), std::make_pair(b0, const1.getIndex()));
    
    b2->insert(&mul1);
    b2->insert(&sub1);

    BinaryOperation add1 = BinaryOperation(9, "Add", std::make_pair(b0, const3.getIndex()), std::make_pair(b1, phi1.getIndex()));
    ReturnOperation ret1 = ReturnOperation(10);

    b3->insert(&add1);
    b3->insert(&ret1);
    
    auto regAllocs = application.linearScanRegAlloc(3);
    regAllocs.print();

    EXPECT_TRUE(isRegister(&const1, regAllocs));
    EXPECT_FALSE(isStack(&const1, regAllocs));

    EXPECT_TRUE(isRegister(&const2, regAllocs));
    EXPECT_FALSE(isStack(&const2, regAllocs));

    EXPECT_FALSE(isRegister(&const3, regAllocs));
    EXPECT_TRUE(isStack(&const3, regAllocs));

    EXPECT_FALSE(isRegister(&phi1, regAllocs));
    EXPECT_TRUE(isStack(&phi1, regAllocs));

    EXPECT_TRUE(isRegister(&phi2, regAllocs));
    EXPECT_FALSE(isStack(&phi2, regAllocs));

    EXPECT_TRUE(isRegister(&equal1, regAllocs));
    EXPECT_FALSE(isStack(&equal1, regAllocs));

    EXPECT_TRUE(isRegister(&mul1, regAllocs));
    EXPECT_FALSE(isStack(&mul1, regAllocs));

    EXPECT_TRUE(isRegister(&sub1, regAllocs));
    EXPECT_FALSE(isStack(&sub1, regAllocs));

    EXPECT_TRUE(isRegister(&add1, regAllocs));
    EXPECT_FALSE(isStack(&add1, regAllocs));

}

TEST(regAlloc, test4) {
    
    std::string name = "regAlloc";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("void");
    
    auto b0 = application.create<BasicBlock>();
    auto b1 = application.create<BasicBlock>();
    auto b2 = application.create<BasicBlock>();
    auto b3 = application.create<BasicBlock>();

    b0->addSucessor({b1});

    b1->addPredessor({b0, b2});
    b1->addSucessor({b2, b3});

    b2->addPredessor({b1});
    b2->addSucessor({b1});

    b3->addPredessor({b1});

    ConstantOperation const1 = ConstantOperation(0, 1, "I32");
    ConstantOperation const2 = ConstantOperation(1, 10, "I32");
    ConstantOperation const3 = ConstantOperation(2, 20, "I32");

    b0->insert(&const1);
    b0->insert(&const2);
    b0->insert(&const3);

    PhiOperation phi1 = PhiOperation(3, {b0, b2}, {const1.getIndex(), 7});
    PhiOperation phi2 = PhiOperation(4, {b0, b2}, {const2.getIndex(), 8});
    CompareOperation equal1 = CompareOperation(5, "eq", std::make_pair(b1, phi2.getIndex()), std::make_pair(b0, const1.getIndex()));
    IfOperation if1 = IfOperation(6, b2, b3, std::make_pair(b1, equal1.getIndex()));

    b1->insert(&phi1);
    b1->insert(&phi2);
    b1->insert(&equal1);
    b1->insert(&if1);

    BinaryOperation mul1 = BinaryOperation(7, "Mul", std::make_pair(b1, phi1.getIndex()), std::make_pair(b1, phi2.getIndex()));
    BinaryOperation sub1 = BinaryOperation(8, "Sub", std::make_pair(b1, phi2.getIndex()), std::make_pair(b0, const1.getIndex()));
    
    b2->insert(&mul1);
    b2->insert(&sub1);

    BinaryOperation add1 = BinaryOperation(9, "Add", std::make_pair(b0, const3.getIndex()), std::make_pair(b1, phi1.getIndex()));
    ReturnOperation ret1 = ReturnOperation(10);

    b3->insert(&add1);
    b3->insert(&ret1);
    
    auto regAllocs = application.linearScanRegAlloc(4);
    regAllocs.print();

    EXPECT_TRUE(isRegister(&const1, regAllocs));
    EXPECT_FALSE(isStack(&const1, regAllocs));

    EXPECT_TRUE(isRegister(&const2, regAllocs));
    EXPECT_FALSE(isStack(&const2, regAllocs));

    EXPECT_TRUE(isRegister(&const3, regAllocs));
    EXPECT_FALSE(isStack(&const3, regAllocs));

    EXPECT_FALSE(isRegister(&phi1, regAllocs));
    EXPECT_TRUE(isStack(&phi1, regAllocs));

    EXPECT_TRUE(isRegister(&phi2, regAllocs));
    EXPECT_FALSE(isStack(&phi2, regAllocs));

    EXPECT_TRUE(isRegister(&equal1, regAllocs));
    EXPECT_FALSE(isStack(&equal1, regAllocs));

    EXPECT_TRUE(isRegister(&mul1, regAllocs));
    EXPECT_FALSE(isStack(&mul1, regAllocs));

    EXPECT_TRUE(isRegister(&sub1, regAllocs));
    EXPECT_FALSE(isStack(&sub1, regAllocs));

    EXPECT_TRUE(isRegister(&add1, regAllocs));
    EXPECT_FALSE(isStack(&add1, regAllocs));

}

TEST(regAlloc, test5) {
    
    std::string name = "regAlloc";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("void");
    
    auto b0 = application.create<BasicBlock>();
    auto b1 = application.create<BasicBlock>();
    auto b2 = application.create<BasicBlock>();
    auto b3 = application.create<BasicBlock>();

    b0->addSucessor({b1});

    b1->addPredessor({b0, b2});
    b1->addSucessor({b2, b3});

    b2->addPredessor({b1});
    b2->addSucessor({b1});

    b3->addPredessor({b1});

    ConstantOperation const1 = ConstantOperation(0, 1, "I32");
    ConstantOperation const2 = ConstantOperation(1, 10, "I32");
    ConstantOperation const3 = ConstantOperation(2, 20, "I32");

    b0->insert(&const1);
    b0->insert(&const2);
    b0->insert(&const3);

    PhiOperation phi1 = PhiOperation(3, {b0, b2}, {const1.getIndex(), 7});
    PhiOperation phi2 = PhiOperation(4, {b0, b2}, {const2.getIndex(), 8});
    CompareOperation equal1 = CompareOperation(5, "eq", std::make_pair(b1, phi2.getIndex()), std::make_pair(b0, const1.getIndex()));
    IfOperation if1 = IfOperation(6, b2, b3, std::make_pair(b1, equal1.getIndex()));

    b1->insert(&phi1);
    b1->insert(&phi2);
    b1->insert(&equal1);
    b1->insert(&if1);

    BinaryOperation mul1 = BinaryOperation(7, "Mul", std::make_pair(b1, phi1.getIndex()), std::make_pair(b1, phi2.getIndex()));
    BinaryOperation sub1 = BinaryOperation(8, "Sub", std::make_pair(b1, phi2.getIndex()), std::make_pair(b0, const1.getIndex()));
    
    b2->insert(&mul1);
    b2->insert(&sub1);

    BinaryOperation add1 = BinaryOperation(9, "Add", std::make_pair(b0, const3.getIndex()), std::make_pair(b1, phi1.getIndex()));
    ReturnOperation ret1 = ReturnOperation(10);

    b3->insert(&add1);
    b3->insert(&ret1);
    
    auto regAllocs = application.linearScanRegAlloc(5);
    regAllocs.print();

    EXPECT_TRUE(isRegister(&const1, regAllocs));
    EXPECT_FALSE(isStack(&const1, regAllocs));

    EXPECT_TRUE(isRegister(&const2, regAllocs));
    EXPECT_FALSE(isStack(&const2, regAllocs));

    EXPECT_TRUE(isRegister(&const3, regAllocs));
    EXPECT_FALSE(isStack(&const3, regAllocs));

    EXPECT_TRUE(isRegister(&phi1, regAllocs));
    EXPECT_FALSE(isStack(&phi1, regAllocs));

    EXPECT_TRUE(isRegister(&phi2, regAllocs));
    EXPECT_FALSE(isStack(&phi2, regAllocs));

    EXPECT_TRUE(isRegister(&equal1, regAllocs));
    EXPECT_FALSE(isStack(&equal1, regAllocs));

    EXPECT_TRUE(isRegister(&mul1, regAllocs));
    EXPECT_FALSE(isStack(&mul1, regAllocs));

    EXPECT_TRUE(isRegister(&sub1, regAllocs));
    EXPECT_FALSE(isStack(&sub1, regAllocs));

    EXPECT_TRUE(isRegister(&add1, regAllocs));
    EXPECT_FALSE(isStack(&add1, regAllocs));

}

TEST(regAlloc, test2) {
    
    std::string name = "regAlloc";
    Module application(name);
    auto s = application.getName();
    application.setArgument("I32");
    application.setReturnType("void");
    
    auto b0 = application.create<BasicBlock>();
    auto b1 = application.create<BasicBlock>();
    auto b2 = application.create<BasicBlock>();
    auto b3 = application.create<BasicBlock>();

    b0->addSucessor({b1});

    b1->addPredessor({b0, b2});
    b1->addSucessor({b2, b3});

    b2->addPredessor({b1});
    b2->addSucessor({b1});

    b3->addPredessor({b1});

    ConstantOperation const1 = ConstantOperation(0, 1, "I32");
    ConstantOperation const2 = ConstantOperation(1, 10, "I32");
    ConstantOperation const3 = ConstantOperation(2, 20, "I32");

    b0->insert(&const1);
    b0->insert(&const2);
    b0->insert(&const3);

    PhiOperation phi1 = PhiOperation(3, {b0, b2}, {const1.getIndex(), 7});
    PhiOperation phi2 = PhiOperation(4, {b0, b2}, {const2.getIndex(), 8});
    CompareOperation equal1 = CompareOperation(5, "eq", std::make_pair(b1, phi2.getIndex()), std::make_pair(b0, const1.getIndex()));
    IfOperation if1 = IfOperation(6, b2, b3, std::make_pair(b1, equal1.getIndex()));

    b1->insert(&phi1);
    b1->insert(&phi2);
    b1->insert(&equal1);
    b1->insert(&if1);

    BinaryOperation mul1 = BinaryOperation(7, "Mul", std::make_pair(b1, phi1.getIndex()), std::make_pair(b1, phi2.getIndex()));
    BinaryOperation sub1 = BinaryOperation(8, "Sub", std::make_pair(b1, phi2.getIndex()), std::make_pair(b0, const1.getIndex()));
    
    b2->insert(&mul1);
    b2->insert(&sub1);

    BinaryOperation add1 = BinaryOperation(9, "Add", std::make_pair(b0, const3.getIndex()), std::make_pair(b1, phi1.getIndex()));
    ReturnOperation ret1 = ReturnOperation(10);

    b3->insert(&add1);
    b3->insert(&ret1);
    
    auto regAllocs = application.linearScanRegAlloc(2);
    regAllocs.print();

    EXPECT_FALSE(isRegister(&const1, regAllocs));
    EXPECT_TRUE(isStack(&const1, regAllocs));

    EXPECT_TRUE(isRegister(&const2, regAllocs));
    EXPECT_FALSE(isStack(&const2, regAllocs));

    EXPECT_FALSE(isRegister(&const3, regAllocs));
    EXPECT_TRUE(isStack(&const3, regAllocs));

    EXPECT_FALSE(isRegister(&phi1, regAllocs));
    EXPECT_TRUE(isStack(&phi1, regAllocs));

    EXPECT_TRUE(isRegister(&phi2, regAllocs));
    EXPECT_FALSE(isStack(&phi2, regAllocs));

    EXPECT_TRUE(isRegister(&equal1, regAllocs));
    EXPECT_FALSE(isStack(&equal1, regAllocs));

    EXPECT_TRUE(isRegister(&mul1, regAllocs));
    EXPECT_FALSE(isStack(&mul1, regAllocs));

    EXPECT_TRUE(isRegister(&sub1, regAllocs));
    EXPECT_FALSE(isStack(&sub1, regAllocs));

    EXPECT_TRUE(isRegister(&add1, regAllocs));
    EXPECT_FALSE(isStack(&add1, regAllocs));

}
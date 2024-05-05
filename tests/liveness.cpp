#include <gtest/gtest.h>
#include "./../include/module.h"
#include "./../include/type.h"


struct LiveNumberTest {
    std::vector<std::pair<Operation *, int>> liveNumberCorrect;
    LiveNumberTest(std::vector<std::pair<Operation *, int>>& _liveNumberCorrect) : liveNumberCorrect(_liveNumberCorrect) {}
    bool checkNumber(std::vector<std::pair<Operation *, int>>& b) const { return liveNumberCorrect.size() == b.size(); }
    bool checkLiveNumbers(std::vector<std::pair<Operation *, int>>& b) const {
        for(auto& el : b) {
            if (std::find(liveNumberCorrect.cbegin(), liveNumberCorrect.cend(), el) == liveNumberCorrect.cend()) {
                std::cout << "Error live Number: " << std::endl;
                el.first->print();
                std::cout << "Live number: " << el.second << std::endl;
                return false;
            }
        }
        return true;
    }

};

struct LifeIntervalTest {
    std::unordered_map<Operation *, std::pair<int, int>> lifeIntervalCorrect;
    LifeIntervalTest(std::unordered_map<Operation *, std::pair<int, int>>& _lifeIntervalCorrect) : lifeIntervalCorrect(_lifeIntervalCorrect) {}
    bool checkLiveIntervals(std::unordered_map<Operation *, std::pair<int, int>>& lifeIntervals) const {
        for(auto& el : lifeIntervals) {
            if (std::find(lifeIntervalCorrect.cbegin(), lifeIntervalCorrect.cend(), el) == lifeIntervalCorrect.cend()) {
                std::cout << "Error live interval for: " << std::endl;
                el.first->print();
                std::cout << "Live interval: " << el.second.first << " " << el.second.second << std::endl;
                return false;
            }
        }
        return true;

    }
};


TEST(TestLiveNumber, test1) {
    std::string name = "Liveness";
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

    auto const1 = application.create<ConstantOperation>(0, 1, "I32");
    auto const2 = application.create<ConstantOperation>(1, 10, "I32");
    auto const3 = application.create<ConstantOperation>(2, 20, "I32");

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(const3);

    auto phi1 = application.create<PhiOperation>(3);
    phi1->addInputs({b0, b2}, {const1->getIndex(), 7});
    auto phi2 = application.create<PhiOperation>(4);
    phi2->addInputs({b0, b2}, {const2->getIndex(), 8});
    auto equal1 = application.create<CompareOperation>(5, "eq", std::make_pair(b1, phi2->getIndex()), std::make_pair(b0, const1->getIndex()));
    auto if1 = application.create<IfOperation>(6, b2, b3, std::make_pair(b1, equal1->getIndex()));

    b1->insert(phi1);
    b1->insert(phi2);
    b1->insert(equal1);
    b1->insert(if1);

    auto mul1 = application.create<BinaryOperation>(7, "Mul", std::make_pair(b1, phi1->getIndex()), std::make_pair(b1, phi2->getIndex()));
    auto sub1 = application.create<BinaryOperation>(8, "Sub", std::make_pair(b1, phi2->getIndex()), std::make_pair(b0, const1->getIndex()));
    
    b2->insert(mul1);
    b2->insert(sub1);

    auto add1 = application.create<BinaryOperation>(9, "Add", std::make_pair(b0, const3->getIndex()), std::make_pair(b1, phi1->getIndex()));
    auto ret1 = application.create<ReturnOperation>(10);

    b3->insert(add1);
    b3->insert(ret1);

    auto liveNumbers = application.createLiveNumbers();
    std::vector<std::pair<Operation*, int>> realLiveNumbers = {
        std::make_pair(const1, 2),
        std::make_pair(const2, 4),
        std::make_pair(const3, 6),
        std::make_pair(phi1, 8),
        std::make_pair(phi2, 8),
        std::make_pair(equal1, 10),
        std::make_pair(if1, 12),
        std::make_pair(mul1, 16),
        std::make_pair(sub1, 18),
        std::make_pair(add1, 22),
        std::make_pair(ret1, 24)
    };

    LiveNumberTest liveNumberTest(realLiveNumbers);
    EXPECT_TRUE(liveNumberTest.checkNumber(liveNumbers));
    EXPECT_TRUE(liveNumberTest.checkLiveNumbers(liveNumbers));
    
}

TEST(TestLiveness, test2) {
    std::string name = "Liveness";
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

    auto const1 = application.create<ConstantOperation>(0, 1, "I32");
    auto const2 = application.create<ConstantOperation>(1, 10, "I32");
    auto const3 = application.create<ConstantOperation>(2, 20, "I32");

    b0->insert(const1);
    b0->insert(const2);
    b0->insert(const3);

    auto phi1 = application.create<PhiOperation>(3);
    phi1->addInputs({b0, b2}, {const1->getIndex(), 7});
    auto phi2 = application.create<PhiOperation>(4);
    phi2->addInputs({b0, b2}, {const2->getIndex(), 8});
    auto equal1 = application.create<CompareOperation>(5, "eq", std::make_pair(b1, phi2->getIndex()), std::make_pair(b0, const1->getIndex()));
    auto if1 = application.create<IfOperation>(6, b2, b3, std::make_pair(b1, equal1->getIndex()));

    b1->insert(phi1);
    b1->insert(phi2);
    b1->insert(equal1);
    b1->insert(if1);

    auto mul1 = application.create<BinaryOperation>(7, "Mul", std::make_pair(b1, phi1->getIndex()), std::make_pair(b1, phi2->getIndex()));
    auto sub1 = application.create<BinaryOperation>(8, "Sub", std::make_pair(b1, phi2->getIndex()), std::make_pair(b0, const1->getIndex()));
    
    b2->insert(mul1);
    b2->insert(sub1);

    auto add1 = application.create<BinaryOperation>(9, "Add", std::make_pair(b0, const3->getIndex()), std::make_pair(b1, phi1->getIndex()));
    auto ret1 = application.create<ReturnOperation>(10);

    b3->insert(add1);
    b3->insert(ret1);

    auto liveNumbers = application.createLiveNumbers();
    auto lifeIntervals = application.lifeInterval(liveNumbers);

    LifeIntervalTest test(lifeIntervals);
    std::unordered_map<Operation*, std::pair<int, int>> m_intervals;
    m_intervals[const1] = {2, 20};
    m_intervals[const2] = {4, 8};
    m_intervals[const3] = {6, 22};
    m_intervals[phi1] = {8, 22};
    m_intervals[phi2] = {8, 18};
    m_intervals[equal1] = {10, 12};
    m_intervals[mul1] = {16, 20};
    m_intervals[sub1] = {18, 20};
    m_intervals[add1] = {22, 24};
    EXPECT_TRUE(test.checkLiveIntervals(m_intervals));
}
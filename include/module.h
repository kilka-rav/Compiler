#pragma once

#include <iostream>
#include <vector>
#include "operation.h"
#include <memory>
#include <list>
#include <set>


class BasicBlock {
private:
    int id = 0;
    std::vector<BasicBlock*> prev_id;
    std::vector<BasicBlock*> next_id;
    int number_dfs = -1;
    
    std::vector<BasicBlock*> getParentsID() const { return prev_id; }
    std::list<Operation*> ops;
    
public:
    BasicBlock() = default;
    BasicBlock(int _id, std::initializer_list<BasicBlock*> _prev, std::initializer_list<BasicBlock*> _next);
    void print() const;
    int getID() const { return id; }
    void insert(Operation* op);
    std::vector<BasicBlock*> getPrev() const;
    std::vector<BasicBlock*> getNext() const;
    void resettingDFS();
    void resettingDFSCondition();
    void setNumberDFS(int numDFS);
    void setNumberDFSCondition(int numDFS);
    bool isMarked() const;
    bool isMarkedCondition() const;
    int getNumDFS() const;
    void print_ids() const;
    int number_dfs_condition = -1;
};

struct LoopInfo {
    int enteringNode;
    int header;
    int possibleHeader;
    std::vector<int> latches;
    std::set<int> vertex;
public:
    LoopInfo(int _possibleHeader) : possibleHeader(_possibleHeader) {}
};

//template <typename ConstantType>
class Module {
private:
    std::string& name;
    std::vector<BasicBlock*> basicBlocks;
    std::string returnType = "void";
    std::vector<std::string> inputs;
    std::vector<int> immediateDominators;
    std::vector<LoopInfo> loops;
    std::string printInputs() const;
    bool needDFS = true;
    void markDFS(BasicBlock* bb, int& numDFS);
    void markDFSCondition(BasicBlock* bb, int& numDFS, int dominator);
    void DFSCondition(int indexOfDominator);
    void writeToImmediateDominators(int idx, std::vector<int>& dfsNotMarked);
    void resettingDFSCondition();
    void reverseFilling(LoopInfo& loop, int latch);
    void checkIrreducibleLoop();
    void fillLoopNodes();
    void findLatches();
public:
    Module(std::string& _name) : name(_name) {}
    std::string& getName() const;
    void setReturnType(const std::string& _returnType);
    void setName(const std::string& _name);
    void setArgument(const std::string& inputType);
    void setArguments(const std::vector<std::string>& inputs);
    void print() const;
    void insert(BasicBlock* bb);
    void DFS();
    void printDFS() const;
    void printDFSCondition() const;
    void buildImmediateDominators();
    bool isDominator(int idxA, int idxB);
    void printLoops() const;
    virtual ~Module() {}
    void loopAnalyzer();
    std::vector<LoopInfo> getLoops() const;
    template <typename Type, typename ...Args>
    Type* create(Args... args) {
        std::cout << "Cannot find constructor" << std::endl;
        return nullptr;
    }
};


template <>
inline BasicBlock* Module::create<BasicBlock>(std::initializer_list<BasicBlock*> _prev, std::initializer_list<BasicBlock*> _next) {
    std::cout << "start creating" << std::endl;
    BasicBlock* bb = new BasicBlock(basicBlocks.size(), {}, {});
    return bb;
}


template <>
inline BasicBlock* Module::create<BasicBlock>() {
    std::cout << "start creating 2" << std::endl;
    BasicBlock* bb = new BasicBlock(basicBlocks.size(), {}, {});
    basicBlocks.push_back(bb);
    return bb;
}
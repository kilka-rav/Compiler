#pragma once

#include <iostream>
#include <vector>
#include "operation.h"
#include <memory>
#include <list>


class BasicBlock {
private:
    int id = 0;
    std::vector<int> prev_id;
    std::vector<int> next_id;
    int number_dfs = -1;
    std::vector<int> getParentsID() const { return prev_id; }
    std::list<Operation*> ops;
    
public:
    BasicBlock() = default;
    BasicBlock(int _id, std::initializer_list<int> _prev, std::initializer_list<int> _next);
    void print() const;
    int getID() const { return id; }
    void insert(Operation* op);
    std::vector<int> getPrev() const;
    std::vector<int> getNext() const;
    void resettingDFS();
    void setNumberDFS(int numDFS);
    bool isMarked() const;
    int getNumDFS() const;
    void print_ids() const;
};

//template <typename ConstantType>
class Module {
private:
    std::string& name;
    std::vector<BasicBlock*> basicBlocks;
    std::string returnType = "void";
    std::vector<std::string> inputs;
    std::vector<int> immediateDominators;
    std::string printInputs() const;
    bool needDFS = true;
    void markDFS(BasicBlock* bb, int& numDFS);
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
    void buildImmediateDominators();
    virtual ~Module() {}
};

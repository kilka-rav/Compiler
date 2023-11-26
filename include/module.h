#pragma once

#include <iostream>
#include <vector>
#include "operation.h"
#include <memory>



class BasicBlock {
private:
    int id = 0;
    int prev_id = 0;
    std::vector<int> next_id;
    
    int getParentID() const { return prev_id; }
    std::vector<Operation*> ops;
public:
    BasicBlock() = default;
    BasicBlock(int _prev, int _id, int _next) : id(_id), prev_id(_prev), next_id{_next} {}
    BasicBlock(int _prev, int _id, std::vector<int> _next) : id(_id), prev_id(_prev) { next_id = _next;}
    void print() const;
    int getID() const { return id; }
    void insert(Operation* op);
};

//template <typename ConstantType>
class Module {
private:
    std::string& name;
    std::vector<BasicBlock*> basicBlocks;
    std::string returnType = "void";
    std::vector<std::string> inputs;
    std::string printInputs() const;
 //   std::vector<ConstantType> constants;
public:
    Module(std::string& _name) : name(_name) {}
    std::string& getName() const;
    void setReturnType(const std::string& _returnType);
    void setName(const std::string& _name);
    void setArgument(const std::string& inputType);
    void setArguments(const std::vector<std::string>& inputs);
    void print() const;
    void insert(BasicBlock* bb);
    virtual ~Module() {}
    
};

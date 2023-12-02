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
    
    std::vector<int> getParentsID() const { return prev_id; }
    std::list<Operation*> ops;
    void print_ids() const {
        std::cout << " prev ids: ";
        for(auto id : prev_id)
            std::cout << id << " ";
        std::cout << " next ids: ";
        for(auto id : next_id)
            std::cout << id << " ";
    }
public:
    BasicBlock() = default;
    //BasicBlock(int _id, int _prev, int _next) : id(_id), prev_id(_prev), next_id{_next} {}
    BasicBlock(int _id, std::initializer_list<int> _prev, std::initializer_list<int> _next) {
        id = _id;
        prev_id = _prev;
        next_id = _next;
    }
    void print() const;
    int getID() const { return id; }
    void insert(Operation* op);
    std::vector<int> getPrev() const {
        return prev_id;
    }
    std::vector<int> getNext() const {
        return next_id;
    }
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

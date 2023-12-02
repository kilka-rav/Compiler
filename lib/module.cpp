#include "../include/module.h"


void Module::setName(const std::string& _name) {
    name = _name;
}

std::string& Module::getName() const {
    return name;
}

void Module::print() const{
    std::cout << "Module: " << name << "(" << printInputs() << ") -> " << returnType <<" {" << std::endl;
    for(auto& bb : basicBlocks) {
        bb->print();
    }
    std::cout << "}" << std::endl;
}

std::string Module::printInputs() const {
    if (inputs.size() == 0) {
        return "";
    }
    std::string s;
    for(int i = 0; i < inputs.size(); ++i) {
        s = s + "%arg" + std::to_string(i) + " : " + inputs[i] + ", ";
    }
    return s;
}

void Module::insert(BasicBlock* b) {
    basicBlocks.push_back(b);
}

void Module::setArgument(const std::string& inputType) {
    inputs.push_back(inputType);
}

void Module::setReturnType(const std::string& outType) {
    returnType = outType;
}

void BasicBlock::print() const {
    std::cout << "\tBasic Block: id: " << this->getID();
    print_ids();
    std::cout  << " {" << std::endl;
    for(auto&& t : ops) {
        t->print();
    }
    std::cout << "\t}" << std::endl;
}

void BasicBlock::insert(Operation* op) {
    ops.push_back(op);
}
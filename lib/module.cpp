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
    needDFS = true;
}

void Module::setArgument(const std::string& inputType) {
    inputs.push_back(inputType);
}

void Module::setReturnType(const std::string& outType) {
    returnType = outType;
}

void Module::DFS() {
    for(auto bb : basicBlocks) {
        bb->resettingDFS();
    }
    int start = 1;
    markDFS(basicBlocks[0], start);
    needDFS = false;
}

void Module::markDFS(BasicBlock* bb, int& numDFS) {
    if (!bb->isMarked()) {
        bb->setNumberDFS(numDFS);
        numDFS++;
        for(auto child : bb->getNext()) {
            markDFS(basicBlocks[child], numDFS);
        }
    }
}

void Module::markDFSCondition(BasicBlock* bb, int& numDFS, int dominator) {
    if (bb->getID() == dominator) {
        return;
    }
    if (!bb->isMarkedCondition()) {
        bb->setNumberDFSCondition(numDFS);
        numDFS++;
        for(auto child : bb->getNext()) {
            markDFSCondition(basicBlocks[child], numDFS, dominator);
            
        }
    }
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

int BasicBlock::getNumDFS() const {
    return number_dfs;
}

void Module::printDFS() const {
    for(auto bb : basicBlocks) {
        std::cout << "id: " << bb->getID() << " numDFS: " << bb->getNumDFS();
        bb->print_ids();
        std::cout << std::endl;
    }
}

void Module::printDFSCondition() const {
    for(auto bb : basicBlocks) {
        std::cout << "id: " << bb->getID() << " numDFS Cond: " << bb->number_dfs_condition;
        bb->print_ids();
        std::cout << std::endl;
    }
}

bool BasicBlock::isMarked() const {
    if (number_dfs == -1){
        return false;
    }
    return true;
}

bool BasicBlock::isMarkedCondition() const {
    if (number_dfs_condition == -1){
        return false;
    }
    return true;
}

void BasicBlock::print_ids() const {
    std::cout << " prev ids: ";
    for(auto id : prev_id)
        std::cout << id << " ";
    std::cout << " next ids: ";
    for(auto id : next_id)
        std::cout << id << " ";
}

std::vector<int> BasicBlock::getPrev() const {
    return prev_id;
}
std::vector<int> BasicBlock::getNext() const {
    return next_id;
}
void BasicBlock::resettingDFS() {
    number_dfs = -1;
}

void BasicBlock::resettingDFSCondition() {
    number_dfs_condition = -1;
}

void BasicBlock::setNumberDFS(int numDFS) {
    number_dfs = numDFS;
}

void BasicBlock::setNumberDFSCondition(int numDFS) {
    number_dfs_condition = numDFS;
}

BasicBlock::BasicBlock(int _id, std::initializer_list<int> _prev, std::initializer_list<int> _next) {
    id = _id;
    prev_id = _prev;
    next_id = _next;
}

void Module::DFSCondition(int indexOfDominator) {
    int start = 1;
    markDFSCondition(basicBlocks[0], start, indexOfDominator);
}

bool Module::isDominator(int idxA, int idxB) {
    if (idxA == idxB) {
        return false;
    }
    if (needDFS) {
        DFS();
    }
    resettingDFSCondition();
    DFSCondition(idxA);
    return !basicBlocks[idxB]->isMarkedCondition();
}

void Module::resettingDFSCondition() {
    for(auto bb : basicBlocks) {
        bb->resettingDFSCondition();
    }
}
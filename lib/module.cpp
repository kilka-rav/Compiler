#include "../include/module.h"


void Module::setName(const std::string& _name) {
    name = _name;
}

std::string& Module::getName() const {
    return name;
}

void Module::print() const{
    std::cout << "Module: " << name << "(" << printInputs() << ") -> " << returnType <<" {" << std::endl;
    for(auto&& bb : basicBlocks) {
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
    vertexDFS.clear();
    vertexDFS.resize(basicBlocks.size());
    int start = 0;
    markDFS(basicBlocks[0], start);
    needDFS = false;
}

void Module::markDFS(BasicBlock* bb, int& numDFS) {
    if (!bb->isMarked()) {
        bb->setNumberDFS(numDFS);
        vertexDFS[numDFS] = bb->getID();
        numDFS++;
        for(auto child : bb->getNext()) {
            markDFS(child, numDFS);
        }
    } else if (bb->getNext().size() > 0){
        if (bb->getID() != bb->getNext()[0]->getID()) {
            LoopInfo possibleLoop(bb->getID());
            loops.push_back(possibleLoop);
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
            markDFSCondition(child, numDFS, dominator);
            
        }
    }
}

void BasicBlock::print() const {
    std::cout << "\tBasic Block: id: " << getID();
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
    for(auto&& id : prev_id)
        std::cout << id->getID() << " ";
    std::cout << " next ids: ";
    for(auto&& id : next_id)
        std::cout << id->getID() << " ";
}

std::vector<BasicBlock*> BasicBlock::getPrev() const {
    return prev_id;
}
std::vector<BasicBlock*> BasicBlock::getNext() const {
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

void BasicBlock::addPredessor(std::initializer_list<BasicBlock*> bbs) {
    for(auto&& bb : bbs) {
        prev_id.push_back(bb);
    }
}

void BasicBlock::addSucessor(std::initializer_list<BasicBlock*> bbs) {
    for(auto&& bb : bbs) {
        next_id.push_back(bb);
    }
}

BasicBlock::BasicBlock(int _id, std::initializer_list<BasicBlock*> _prev, std::initializer_list<BasicBlock*> _next) {
    id = _id;
    prev_id = _prev;
    next_id = _next;
}

void Module::DFSCondition(int indexOfDominator) {
    int start = 1;
    markDFSCondition(basicBlocks[0], start, indexOfDominator);
}

int Module::getImmediateDominator(BasicBlock* bb) {
    int numDFS = bb->getNumDFS();
    auto idx = bb->getID();
    if (numDFS == 0) {
        return vertexDFS[0];
    }
    for(int i = numDFS - 1; i > 0; i--) {
        if (isDominator(vertexDFS[i], idx)) {
            return vertexDFS[i];
        }
    }
    return vertexDFS[0];
}

void Module::buildDomtree() {
    if (needDFS) {
        DFS();
    }
    immediateDominators.clear();
    immediateDominators.resize(basicBlocks.size());
    for(auto&& bb : basicBlocks) {
        auto idx = getImmediateDominator(bb);
        immediateDominators[bb->getID()] = idx;
    }
    std::cout << "Print: " << immediateDominators.size() <<  std::endl;
    for(int i = 0; i < immediateDominators.size(); ++i) {
        std::cout << i << " " <<  immediateDominators[i]  << std::endl;
    }
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

void Module::printLoops() const {
    for(auto&& loop : loops) {
        std::cout << "Possible header: " <<  loop.possibleHeader << " Latches: ";
        for(auto l : loop.latches) {
            std::cout << l << " ";
        }
        std::cout << "header: " << loop.header << " Entering node: " << loop.enteringNode << std::endl;
        std::cout << "Vertexs: ";
        for(auto l : loop.vertex) {
            std::cout << l << " ";
        }
        if (loop.nestedLoop != -1) {
            std::cout << "Header of nested loop: " << loop.nestedLoop << std::endl;
        }
        std::cout << std::endl;
    }
}

void Module::findLatches() {
    for(auto&& loop : loops) {
        auto curDFS = basicBlocks[loop.possibleHeader]->getNumDFS();
        for(auto&& prev : basicBlocks[loop.possibleHeader]->getPrev()) {
            if (curDFS < basicBlocks[prev->getID()]->getNumDFS()) {
                loop.latches.push_back(prev->getID());
            }
        }
    }
}

void Module::checkIrreducibleLoop() {
    for(auto&& loop : loops) {
        bool irreducibleLoop = false;
        auto possibleHeader = loop.possibleHeader;
        for(auto&& latch : loop.latches) {
            if (!isDominator(possibleHeader, latch)) {
                irreducibleLoop = true;
            }
        }
        if (!irreducibleLoop && checkConditions(loop.possibleHeader)) {
            loop.header = loop.possibleHeader;
            loop.enteringNode = basicBlocks[possibleHeader]->getPrev()[0]->getID();
        }
    }
}

bool Module::checkConditions(int possibleHeader) {
    for(auto&& prev : basicBlocks[possibleHeader]->getPrev()) {
        if (isDominator(possibleHeader, prev->getID())) {
            return true;
        }
    }
    return false;
}

void Module::reverseFilling(int loopIndex, int latchIdx) {
    if (latchIdx == loops[loopIndex].header) {
        return;
    }
    if (loopIndex != loops.size() - 1) {
        if (latchIdx == loops[loopIndex + 1].latches[0]) {
            loops[loopIndex].nestedLoop = loops[loopIndex + 1].header;
            latchIdx = loops[loopIndex + 1].enteringNode;
            if (latchIdx == loops[loopIndex].header) {
                return;
            }
        }
    }
    auto prevs = basicBlocks[latchIdx]->getPrev();
    loops[loopIndex].vertex.insert(latchIdx);
    for(auto idx : prevs) {
        reverseFilling(loopIndex, idx->getID());
    }
    
}

void Module::fillLoopNodes() {
    for(int i = loops.size() - 1; i >= 0; i--) {
        for(auto&& latch : loops[i].latches) {
            reverseFilling(i, latch);
        }
        loops[i].vertex.insert(loops[i].header);
    }
}

void Module::deletePossibleHeader() {
    for(int i = 0; i < loops.size(); ++i) {
        if (loops[i].header != loops[i].possibleHeader) {
            loops.erase(loops.begin() + i);
        }
    }
}

void Module::loopAnalyzer() {
    DFS();
    findLatches();
    checkIrreducibleLoop();
    deletePossibleHeader();
    fillLoopNodes();
}

std::vector<LoopInfo> Module::getLoops() const {
    return loops;
}

int Operation::getIndex() const {
    return idx;
}

std::string Operation::getName() const {
    return name;
}
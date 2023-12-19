#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <set>

class Operation;


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
    void addSucessor(std::initializer_list<BasicBlock*> bbs);
    void addPredessor(std::initializer_list<BasicBlock*> bbs);
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
    std::vector<int> vertexDFS;
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
    int getImmediateDominator(BasicBlock* bb);
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
    virtual ~Module() {
        for(auto&& b : basicBlocks) {
            delete b;
        }
    }
    void loopAnalyzer();
    void buildDomtree();
    std::vector<LoopInfo> getLoops() const;
    template <typename Type, typename ...Args>
    Type* create(Args... args) {
        std::cout << "Cannot find constructor" << std::endl;
        return nullptr;
    }
};


template <>
inline BasicBlock* Module::create<BasicBlock>(std::initializer_list<BasicBlock*> _prev, std::initializer_list<BasicBlock*> _next) {
    BasicBlock* bb = new BasicBlock(basicBlocks.size(), {}, {});
    return bb;
}


template <>
inline BasicBlock* Module::create<BasicBlock>() {
    BasicBlock* bb = new BasicBlock(basicBlocks.size(), {}, {});
    basicBlocks.push_back(bb);
    return bb;
}

class Operation {
private:
    std::string name;
    int idx;
public:
    int getIndex() const;
    virtual void print() const = 0;
    Operation(std::string _name, int _idx) : name(_name), idx(_idx) {}
    std::string getName() const;
};

class ConstantOperation : public Operation {
private:
    uint64_t value;
    std::string DType;
public:
    ConstantOperation(int _idx, uint64_t _value, std::string dt) : value(_value), Operation("Constant", _idx), DType(dt) {}
    ConstantOperation(uint64_t _value, std::string dt) : value(_value), Operation("Constant", 0), DType(dt) {}
    ConstantOperation() = default;
    void print() const {
        std::cout << "\t  %" <<  getIndex() << "." << DType << " Constant " << value << std::endl;
    }    
};

class JumpOperation : public Operation {
    BasicBlock* toBasicBlock;
public:
    JumpOperation(int idx, BasicBlock* id) : toBasicBlock(id), Operation("Jump", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() <<  " -> Basic Block " << toBasicBlock->getID() << std::endl;
    }
};

class PhiOperation : public Operation {
private:
    std::vector<BasicBlock*> inputBB;
    std::vector<int> ops;
public:
    PhiOperation(int idx, std::initializer_list<BasicBlock*> _inputs, std::initializer_list<int> _ops) : inputBB(_inputs), ops(_ops), Operation("Phi", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " ";
        for(size_t i = 0; i < inputBB.size(); ++i) {
            std::cout << "[" << inputBB[i]->getID() << " : %" << ops[i] << "], ";
        }
        std::cout << std::endl;
    }
};

class CompareOperation : public Operation {
    std::string typeCompare;//replace enum
    std::pair<BasicBlock*, int> left;
    std::pair<BasicBlock*, int> right;
public:
    CompareOperation(int idx, std::string _typeCompare, std::pair<BasicBlock*, int> l, std::pair<BasicBlock*, int>r) : typeCompare(_typeCompare), left(l), right(r), Operation("Cmp", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " " << typeCompare << " : [" << left.first->getID() << " : %" << left.second << "], [" 
            << right.first->getID() << " : %" << right.second << "]" << std::endl;
    }
};

class IfOperation : public Operation {
    BasicBlock* trueIdx;
    BasicBlock* falseIdx;
    std::pair<BasicBlock*, int> idxBool;
public:
    IfOperation(int idx, BasicBlock* t, BasicBlock* f, std::pair<BasicBlock*, int> sign) : trueIdx(t), falseIdx(f), idxBool(sign), Operation("If", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " [" << idxBool.first->getID() << " : %" << idxBool.second << "] -> " 
            << trueIdx->getID() << ", " << falseIdx->getID() << std::endl;
    }
};

class BinaryOperation : public Operation {
    std::pair<BasicBlock*, int> left;
    std::pair<BasicBlock*, int> right;
public:
    BinaryOperation(int idx, std::string _type, std::pair<BasicBlock*, int> l, std::pair<BasicBlock*, int> r) : left(l), right(r), Operation(_type, idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " " << " : [" << left.first->getID() << " : %" << left.second << "], [" 
            << right.first->getID() << " : %" << right.second << "]" << std::endl;
    }
};

class CastOperation : public Operation {
    std::string toType;
    std::pair<BasicBlock*, int> prev;
public:
    CastOperation(int idx, std::pair<BasicBlock*, int> p, std::string _type) : prev(p), toType(_type), Operation("Cast", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " %[" << prev.first->getID() << ": " << prev.second << "] -> " << toType << std::endl;
    }
};

class ReturnOperation : public Operation {
    std::pair<BasicBlock*, int> prev;
public:
    ReturnOperation(int idx, std::pair<BasicBlock*, int> p) : prev(p), Operation("Return", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " [" << prev.first->getID() << ": %" << prev.second << "]" << std::endl;
    }
};

class MoveOperation : public Operation {
    std::string prev;
public:
    MoveOperation(int idx, std::string p) : prev(p), Operation("Move", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " %" << prev << std::endl;
    }
};
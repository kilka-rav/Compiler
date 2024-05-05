#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <stack>

class Operation;


class BasicBlock {
private:
    int id = 0;
    std::vector<BasicBlock*> prev_id;
    std::vector<BasicBlock*> next_id;
    int number_dfs = -1;
    std::vector<BasicBlock*> getParentsID() const { return prev_id; }
    std::vector<Operation*> ops;
    
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
    bool replace(Operation* dst, Operation* src);
    bool eraseWithoutResult(Operation* op);
    bool replaceAllUses(Operation* oldOp, Operation* newOp);
    void deleteOp(int idx);
    std::unordered_set<Operation*> liveSet;
    std::vector<Operation*> getOps() const {
        return ops;
    }
    int number_dfs_condition = -1;
    virtual ~BasicBlock();
};

struct LoopInfo {
    int enteringNode = -1;
    int header = -1;
    int possibleHeader = -1;
    std::vector<int> latches;
    std::set<int> vertex;
    int nestedLoop = -1;//if >-1, then nested loop = header of nested loop
public:
    LoopInfo(int _possibleHeader) : possibleHeader(_possibleHeader) {}
};

struct AllocInfo {
    int stackShift;
    std::vector<std::pair<Operation*, int>> regs;
    void print() const;
    void insert(Operation* op, int reg);
    int getReg(Operation* op) const;
    void moveToStack(Operation* op);
    AllocInfo() : stackShift(-1) {};
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
    void deletePossibleHeader();
    bool needDFS = true;
    void markDFS(BasicBlock* bb, int& numDFS);
    void markDFSCondition(BasicBlock* bb, int& numDFS, int dominator);
    void DFSCondition(int indexOfDominator);
    void writeToImmediateDominators(int idx, std::vector<int>& dfsNotMarked);
    void resettingDFSCondition();
    void reverseFilling(int loopIndex, int latch);
    void checkIrreducibleLoop();
    void fillLoopNodes();
    void findLatches();
    int getImmediateDominator(BasicBlock* bb);
    bool checkConditions(int possibleHeader);
    void expireOldIntervals(Operation* op, std::vector<std::pair<Operation*, std::pair<int, int>>>& active, std::stack<int>& freeReg, int idx, AllocInfo& info);
    void spillAtInterval(Operation* op, std::vector<std::pair<Operation*, std::pair<int, int>>>& active, int idx, int idx2, AllocInfo& info);
    std::vector<BasicBlock*> linearOrder();
public:
    Module(std::string& _name) : name(_name) {}
    std::string& getName() const;
    std::vector<BasicBlock*> rpo();
    void eraseWithoutResult(Operation* op);
    void replace(Operation* dest, Operation* src);
    void replaceAllUses(Operation* oldOp, Operation* newOp);
    Operation* getOperation(int bb, int index) const;
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
    int getBBFromOp(Operation* op) const;
    bool haveUsers(int idx) const;
    int getNumUsers(int idx) const;
    void deleteOp(Operation* op);
    std::vector<BasicBlock*> getBBs() const { return basicBlocks; }
    virtual ~Module() {
        for(auto&& b : basicBlocks) {
            delete b;
        }
    }
    void loopAnalyzer();
    void buildDomtree();
    std::vector<std::pair<Operation*, int>> createLiveNumbers();
    std::pair<int, int> getIntervalNumberForBB(BasicBlock* b, std::vector<std::pair<Operation*, int>>& liveNumbers);
    std::unordered_map<Operation*, std::pair<int, int>> lifeInterval(std::vector<std::pair<Operation*, int>>& linear);
    std::vector<LoopInfo> getLoops() const;
    AllocInfo linearScanRegAlloc(int numRegisters);
    template <typename Type, typename ...Args>
    Type* create(Args... args) {
        Type* op = new Type(args...);
        return op;
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
    std::vector<int> operands;
public:
    int getIndex() const;
    virtual void print() const = 0;
    Operation(std::string _name, int _idx) : name(_name), idx(_idx) {}
    Operation(std::string _name, int _idx, std::initializer_list<int> indexOfOperands) : name(_name), idx(_idx), operands(indexOfOperands) {}
    std::vector<int> getOperands() const { return operands; }
    std::string getName() const;
    void setIndex(int newIndex);
    void changeOperand(int oldIndex, int newIndex);
    bool hasMemoryEffect() const;
    virtual ~Operation() {}
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
    uint64_t getValue() const { return value; }
    std::string getDType() const { return DType; }
    virtual ~ConstantOperation() {}
};

class JumpOperation : public Operation {
    BasicBlock* toBasicBlock;
public:
    JumpOperation(int idx, BasicBlock* id) : toBasicBlock(id), Operation("Jump", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " << getName() <<  " -> Basic Block " << toBasicBlock->getID() << std::endl;
    }
};

class PhiOperation : public Operation {
private:
    std::vector<BasicBlock*> inputBB;
    std::vector<int> ops;
public:
    PhiOperation(int idx, std::initializer_list<BasicBlock*> _inputs, std::initializer_list<int> _ops) : inputBB(_inputs), ops(_ops), Operation("Phi", idx, _ops) {}
    PhiOperation(int idx) : Operation("Phi", idx) {}
    void addInputs(std::initializer_list<BasicBlock*> _inputs, std::initializer_list<int> _ops) {
        inputBB = _inputs;
        ops = _ops;
    }
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " ";
        for(size_t i = 0; i < inputBB.size(); ++i) {
            std::cout << "[" << inputBB[i]->getID() << " : %" << ops[i] << "], ";
        }
        std::cout << std::endl;
    }
    Operation* getInstrFromBB(BasicBlock* bb) {
        assert(inputBB.size() == ops.size());
        for(int i = 0; i < inputBB.size(); ++i) {
            if (bb == inputBB[i]) {
                int indexOp = ops[i];
                auto &&ops_tmp = bb->getOps();
                for(auto possibleOp : ops_tmp) {
                    if (possibleOp->getIndex() == indexOp) {
                        return possibleOp;
                    }
                }
            }
        }
        return nullptr;
    }
};

class CompareOperation : public Operation {
    std::string typeCompare;//replace enum
    std::pair<BasicBlock*, int> left;
    std::pair<BasicBlock*, int> right;
public:
    CompareOperation(int idx, std::string _typeCompare, std::pair<BasicBlock*, int> l, std::pair<BasicBlock*, int>r) : typeCompare(_typeCompare), left(l), right(r), Operation("Cmp", idx, {l.second, r.second}) {}
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
    IfOperation(int idx, BasicBlock* t, BasicBlock* f, std::pair<BasicBlock*, int> sign) : trueIdx(t), falseIdx(f), idxBool(sign), Operation("If", idx, {sign.second}) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " [" << idxBool.first->getID() << " : %" << idxBool.second << "] -> " 
            << trueIdx->getID() << ", " << falseIdx->getID() << std::endl;
    }
};

class BinaryOperation : public Operation {
    std::pair<BasicBlock*, int> left;
    std::pair<BasicBlock*, int> right;
public:
    BinaryOperation(int idx, std::string _type, std::pair<BasicBlock*, int> l, std::pair<BasicBlock*, int> r) : left(l), right(r), Operation(_type, idx, {l.second, r.second}) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " " << " : [" << left.first->getID() << " : %" << left.second << "], [" 
            << right.first->getID() << " : %" << right.second << "]" << std::endl;
    }
    std::pair<int, int> getA() const { return std::make_pair(left.first->getID(), left.second); }
    std::pair<int, int> getB() const { return std::make_pair(right.first->getID(), right.second); }
};

class CastOperation : public Operation {
    std::string toType;
    std::pair<BasicBlock*, int> prev;
public:
    CastOperation(int idx, std::pair<BasicBlock*, int> p, std::string _type) : prev(p), toType(_type), Operation("Cast", idx, {p.second}) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " %[" << prev.first->getID() << ": " << prev.second << "] -> " << toType << std::endl;
    }
};

class ReturnOperation : public Operation {
    std::pair<BasicBlock*, int> prev;
    bool use_void = false;
public:
    ReturnOperation(int idx, std::pair<BasicBlock*, int> p) : prev(p), Operation("Return", idx, {p.second}) {}
    ReturnOperation(int idx) : use_void(true), Operation("Return", idx) {}
    void print() const {
        if (!use_void) {
            std::cout << "\t  %" << getIndex() << " " <<  getName() << " %" << getOperands()[0] << std::endl;
        } else {
            std::cout << "\t  %" << getIndex() << " " <<  getName() << " void" << std::endl;
        }

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

class ShiftOperation : public Operation {
    std::string direction;
    std::pair<BasicBlock*, int> left;
    std::pair<BasicBlock*, int> right;
public:
    ShiftOperation(int idx, std::string _direction, std::pair<BasicBlock*, int> _left, std::pair<BasicBlock*, int> _right) : direction(_direction), left(_left), right(_right), Operation("Shift", idx, {_left.second, _right.second}) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << direction << " : [" << left.first->getID() << " : %" << left.second << "], [" 
            << right.first->getID() << " : %" << right.second << "]" << std::endl;
    }
    std::string getDirection() const { return direction; }
    std::pair<int, int> getA() const { return std::make_pair(left.first->getID(), left.second); }
    std::pair<int, int> getB() const { return std::make_pair(right.first->getID(), right.second); }
};

class ZeroCheckOperation : public Operation {
public:
    ZeroCheckOperation(int idx, int prev) : Operation("ZeroCheck", idx, {prev}) {}
    void print() const {
        std::cout << " \t  %" << getIndex() << " " << getName() << " %" << getOperands()[0] << std::endl;
    }
};

class BoundsCheckOperation : public Operation {
public:
    BoundsCheckOperation(int idx, int enter, int len) : Operation("BoundsCheck", idx, {enter, len}) {}
    void print() const {
        std::cout << " \t  %" << getIndex() << " " << getName() << " %" << getOperands()[0] << ", %" << getOperands()[1] << std::endl;
    }
};

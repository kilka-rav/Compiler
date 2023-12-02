#pragma once

#include "type.h"

#include <string>
#include <vector>
#include <iostream>

class Operation {
private:
    std::string name;
    size_t idx;
public:
    size_t getIndex() const;
    virtual void print() const = 0;
    Operation(std::string _name, size_t _idx) : name(_name), idx(_idx) {}
    std::string getName() const;
};

class ConstantOperation : public Operation {
private:
    uint64_t value;
    std::string DType;
public:
    ConstantOperation(size_t _idx, uint64_t _value, std::string dt) : value(_value), Operation("Constant", _idx), DType(dt) {}
    ConstantOperation(uint64_t _value, std::string dt) : value(_value), Operation("Constant", 0), DType(dt) {}
    ConstantOperation() = default;
    void print() const {
        std::cout << "\t  %" <<  getIndex() << "." << DType << " Constant " << value << std::endl;
    }    
};

class JumpOperation : public Operation {
    int toBasicBlock;
public:
    JumpOperation(size_t idx, int id) : toBasicBlock(id), Operation("Jump", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() <<  " -> Basic Block " << toBasicBlock << std::endl;
    }
};

class PhiOperation : public Operation {
private:
    size_t leftIdx;
    size_t leftBB;
    size_t rightIdx;
    size_t rightBB;
public:
    PhiOperation(size_t idx, size_t l, size_t _leftIdx, size_t r, size_t _rightIdx) : leftBB(l), leftIdx(_leftIdx), rightBB(r), rightIdx(_rightIdx), Operation("Phi", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " " << leftBB << "->%" << leftIdx << ", " << rightBB << "->%" << rightIdx << std::endl;
    }
};

class CompareOperation : public Operation {
    std::string typeCompare;
    size_t leftIdx;
    size_t rightIdx;
public:
    CompareOperation(size_t idx, std::string _typeCompare, size_t l, size_t r) : typeCompare(_typeCompare), leftIdx(l), rightIdx(r), Operation("Cmp", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " " << typeCompare << " : %" << leftIdx << ", %" << rightIdx << std::endl;
    }
};

class IfOperation : public Operation {
    size_t trueIdx;
    size_t falseIdx;
    size_t idxBool;
public:
    IfOperation(size_t idx, size_t t, size_t f, size_t sign) : trueIdx(t), falseIdx(f), idxBool(sign), Operation("If", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " %" << idxBool << " : BasicBlock " << trueIdx << ", BasicBlock " << falseIdx << std::endl;
    }
};

class BinaryOperation : public Operation {
    size_t leftIdx;
    size_t rightIdx;
public:
    BinaryOperation(size_t idx, std::string _type, size_t l, size_t r) : leftIdx(l), rightIdx(r), Operation(_type, idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " " << " : %" << leftIdx << ", %" << rightIdx << std::endl;
    }
};

class CastOperation : public Operation {
    std::string toType;
    size_t prev;
public:
    CastOperation(size_t idx, size_t p, std::string _type) : prev(p), toType(_type), Operation("Cast", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " %" << prev << " -> " << toType << std::endl;
    }
};

class ReturnOperation : public Operation {
    size_t prev;
public:
    ReturnOperation(size_t idx, size_t p) : prev(p), Operation("Return", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " %" << prev << std::endl;
    }
};

class MoveOperation : public Operation {
    std::string prev;
public:
    MoveOperation(size_t idx, std::string p) : prev(p), Operation("Move", idx) {}
    void print() const {
        std::cout << "\t  %" << getIndex() << " " <<  getName() << " %" << prev << std::endl;
    }
};

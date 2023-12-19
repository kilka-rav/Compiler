#include <iostream>
#include "module.h"
#include "type.h"


int main() {

   std::string name = "CheckDominators";

    Module application(name);
    std::cout << application.getName() << std::endl;
    application.setArgument("I32");
    application.setReturnType("I64");
    
    auto b1 = application.create<BasicBlock>();
    auto b2 = application.create<BasicBlock>();
    auto b3 = application.create<BasicBlock>();
    auto b4 = application.create<BasicBlock>();

    auto b5 = application.create<BasicBlock>();
    auto b6 = application.create<BasicBlock>();

    auto b7 = application.create<BasicBlock>();
    auto b8 = application.create<BasicBlock>();

    auto b9 = application.create<BasicBlock>();
    auto b10 = application.create<BasicBlock>();

    auto b11 = application.create<BasicBlock>();
    auto b12 = application.create<BasicBlock>();


}
#include <iostream>
#include "module.h"
#include "type.h"


int main() {

   std::string name = "CheckDominators";

    Module application(name);
    application.setArgument("I32");
    application.setReturnType("I64");
    
    auto a = application.create<BasicBlock>();
    auto b = application.create<BasicBlock>();
    auto c = application.create<BasicBlock>();
    auto d = application.create<BasicBlock>();

    auto e = application.create<BasicBlock>();
    auto f = application.create<BasicBlock>();

    auto g = application.create<BasicBlock>();
    
    a->addSucessor({b});

    b->addPredessor({a});
    b->addSucessor({c, f});

    c->addPredessor({b});
    c->addSucessor({d});

    d->addPredessor({c, e, g});

    e->addPredessor({f});
    e->addSucessor({d});

    f->addPredessor({b});
    f->addSucessor({e, g});

    g->addPredessor({f});
    g->addSucessor({d});

    application.buildDomtree();

}
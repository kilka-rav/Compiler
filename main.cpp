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
    auto h = application.create<BasicBlock>();
    auto i = application.create<BasicBlock>();

    
    a->addSucessor({b});

    b->addPredessor({a, f});
    b->addSucessor({e, c});

    c->addPredessor({b, g});
    c->addSucessor({d});

    d->addPredessor({c, e});
    d->addSucessor({g});

    e->addPredessor({b});
    e->addSucessor({f, d});

    f->addPredessor({e});
    f->addSucessor({b, h});

    g->addPredessor({h, d});
    g->addSucessor({c, i});

    i->addPredessor({h, g});

    h->addPredessor({e});
    h->addSucessor({g, i});

    application.buildDomtree();
    std::cout << application.isDominator(b->getID(), c->getID()) << std::endl;
}
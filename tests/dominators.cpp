#include <gtest/gtest.h>

#include "./../include/module.h"
#include "./../include/type.h"



TEST(Dominators, example1) {
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

TEST(Dominators, Example2) {
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
    auto k = application.create<BasicBlock>();
    auto j = application.create<BasicBlock>();
    
    a->addSucessor({b});

    b->addPredessor({a, h});
    b->addSucessor({c, j});

    c->addPredessor({b, j, d});
    c->addSucessor({d});

    d->addPredessor({c});
    d->addSucessor({c, e});

    e->addPredessor({d, f});
    e->addSucessor({f});

    f->addPredessor({e});
    f->addSucessor({e, g});

    g->addPredessor({f});
    g->addSucessor({h, i});

    i->addPredessor({g});
    i->addSucessor({k});

    k->addPredessor({i});

    j->addPredessor({b});
    j->addSucessor({c});

    h->addPredessor({g});
    h->addSucessor({b});
    
    application.buildDomtree();
}

TEST(Dominators, Example3) {
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
}
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "ra/map.hpp"

using value_type = std::pair<std::string, int>;
using tree_type = avl_tree<value_type, std::less<std::string>>;
using iterator = tree_type::avl_iterator;//, tree_type>;

TEST_CASE("basic sanity check"){
    map<std::string, int> q;
    map<std::string, int> b;
    map<std::string, int> v;

    b = std::move(q);
    b = v;

    std::pair<iterator, bool> x = b.insert({"hello",37});
    
    CHECK(x.first->data_.first == "hello");
    CHECK(x.first->data_.second == 37);
    CHECK(b.size() == 1);
    
    q = std::move(b);
    
    CHECK(b.size() == 0);
    CHECK(q.size() == 1);

    b = q;

    CHECK(b.size() == 1);
    CHECK(b.at("hello") == 37);
    REQUIRE_THROWS_AS(b.at("chess"), std::out_of_range);

    x = b.insert({"cheese", 43});
    CHECK(x.first->data_.first == "cheese");
    CHECK(x.first->data_.second == 43);

    x = b.insert({"c", 4});
    CHECK(x.first->data_.first == "c");
    CHECK(x.first->data_.second == 4);

    iterator a = b.begin();
    CHECK(a->data_.first == "c");
    CHECK(a->data_.second == 4);

    CHECK(b.size() == 3);

    CHECK(b.empty() == false);
    CHECK(v.empty() == true);

    a = b.begin();
    a++;
    CHECK(a->data_.first == "cheese");
    CHECK(a->data_.second == 43);

    a--;
    CHECK(a->data_.first == "c");
    CHECK(a->data_.second == 4);

    iterator c = b.erase(a);
    CHECK(c->data_.first == "cheese");
    CHECK(c->data_.second == 43);
    CHECK(b.size() == 2);

    map<std::string, int> g;
    map<std::string, int> h;

    g.insert({"hey",1});
    g.swap(h);

    c = h.begin();
    CHECK(c->data_.first == "hey");
    CHECK(c->data_.second == 1);
    
    c = g.begin();
    CHECK(c.equals_end() == true);

    map<std::string, int> t;
    map<std::string, int> k;

    map<std::string, int>* tt = &t;
    map<std::string, int>* kk = &k;

    bool res = kk->insert({"hey",1}).second;

    CHECK(res == true);

    CHECK(kk->at("hey") == 1);

    *tt = std::move(*kk);

    CHECK(tt->at("hey") == 1);
}
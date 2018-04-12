#include "Factory.hpp"
#include <stdio.h>

class Base {
protected:
    virtual ~Base(){}
};

class Descendant1: public Base{
public:
    static constexpr char name[256] = {"Descendant1"};
    int id;
    Descendant1(int id=1)
        :id(id)
    {}
};

class Descendant2: public Base{
public:
    static constexpr char name[256] = {"Descendant2"};
    int id;
    Descendant2(int id=2)
        :id(id)
    {}
};

//--------------------------------------------------------------------------------
int main(int argc, char** argv){
    Factory factory;
    //factory.registerClass<Base>("Cls1");
    factory.registerClass<Descendant1>("Descendant1");
    factory.registerClass<Descendant2>("Descendant2");

    Base* d1 = factory.create("Descendant1");
    printf("d1: %s id=%d\n", ((Descendant1*)d1)->name, ((Descendant1*)d1)->id);
    Base* d2 = factory.create("Descendant2");
    printf("d2: %s id=%d\n", ((Descendant2*)d2)->name, ((Descendant2*)d2)->id);
    return 0;
}

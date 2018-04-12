#include "Factory.hpp"
#include <stdio.h>

//--------------------------------------------------------------------------------
class Base {
public:
    virtual ~Base(){
        printf("%s() this=%p\n", __FUNCTION__, this);
    }
};

//--------------------------------------------------------------------------------
class Derived1: public Base{
public:
    static constexpr char name[256] = {"Derived1"};
    int id;
    Derived1(int id=1)
        :id(id)
    {printf("%s()\n", __FUNCTION__);}
};

//--------------------------------------------------------------------------------
class Derived2: public Base{
public:
    static constexpr char name[256] = {"Derived2"};
    int id;
    Derived2(int id=2)
        :id(id)
    {printf("%s()\n", __FUNCTION__);}

    static Base* create(){return new Derived2(1973);}
};

//--------------------------------------------------------------------------------
int main(int argc, char** argv){
    Factory<Base> factory;
    //factory.registerClass<Base>("Base");
    factory.registerClass<Derived1>("Derived1");
    factory.registerClass<Derived2>("Derived2");
    factory.registerClass<Derived2>("Derived2_2", &Derived2::create);


    Base* b = factory.create<Base>("Base");
    printf("b=%p\n", b);
    delete b;

    Derived1* d1 = factory.create<Derived1>("Derived1");
    printf("d1: %s id=%d\n", d1->name, d1->id);//d1: Derived1 id=1
    delete d1;

    Derived2* d2 = factory.create<Derived2>("Derived2");
    printf("d2: %s id=%d\n", d2->name, d2->id);//d2: Derived2 id=2
    delete d2;

    Derived2* d3 = factory.create<Derived2>("Derived2_2");
    printf("d3: %s id=%d\n", d3->name, d3->id);//d3: Derived2 id=1973
    delete d3;

    return 0;
}

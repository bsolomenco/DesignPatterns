#include "Factory.hpp"
#include <stdio.h>

//--------------------------------------------------------------------------------
class Base{
public:
    int nr;
    Base(int nr=0): nr(nr){}
    virtual ~Base(){printf("%s() this=%p nr=%d\n", __FUNCTION__, this, nr);}
    virtual const char* cls(){return nullptr;}//class ID
};

//--------------------------------------------------------------------------------
class Derived: public Base{
public:
    Derived(int nr=1): Base(nr){printf("%s(%d)\n", __FUNCTION__, nr);}
    const char* cls() override {return "Derived";}
    static Base* create(){return new Derived(1973);}//creator function
};

//--------------------------------------------------------------------------------
class Derived2: public Base{
public:
    Derived2(int nr=2): Base(nr){printf("%s(%d)\n", __FUNCTION__, nr);}
    const char* cls() override {return "Derived2";}
};

//--------------------------------------------------------------------------------
class DerivedCreator{//could be a life cycle manager for Derived objects
public:
    DerivedCreator(){}
    Derived* create(){
        static Derived singleton(2020);
        return &singleton;
    }
};

//--------------------------------------------------------------------------------
int main(int /*argc*/, char** /*argv*/){
    Factory<std::string, Base> factory;//factory for descendants of Base

    //factory.registerClass<Base>("Base");
    factory.registerType<Derived>("Derived");
    factory.registerType<Derived>("Derived_function", &Derived::create);//same class as above but different name and creator
    factory.registerType<Derived>("Derived_lambda"  , [](){return new Derived(2018);});//same class as above but different name and lambda creator
    DerivedCreator derivedCreator;
    factory.registerType<Derived>("Derived_object"  , [&derivedCreator](){return derivedCreator.create();});//lambda + object creator
    factory.registerType<Derived>("Derived_bind"    , std::bind(&DerivedCreator::create, derivedCreator));//using std::bind works but is discouraged since lambda
    factory.registerType<Derived2>("Derived2");

    auto b = factory.create<Base>("Base");//"Base" not registered
    if(!b){printf("[%s()] \"%s\" not registered\n", __FUNCTION__, "Base");}

    struct{
        bool  deletable;
        Base* ptr;
    } arr[] = {//array of objects derived from Base created via factory
        {true , factory.create<Derived >("Derived"          )},
        {true , factory.create<Derived2>("Derived_function" )}, 
        {true , factory.create<Derived2>("Derived_lambda"   )}, 
        {false, factory.create<Derived2>("Derived_object"   )},//no delete because is a singleton 
        {false, factory.create<Derived2>("Derived_bind"     )},//no delete because is a singleton 
        {true , factory.create<Derived2>("Derived2"         )}, 
    };

    for(size_t i=0; i< sizeof(arr)/sizeof(arr[0]); ++i){
        if(arr[i].ptr){
            printf("%zd: %s nr=%d\n", i, arr[i].ptr->cls(), arr[i].ptr->nr);
        }else{
            printf("%zd: nullptr\n", i);
        }
        if(arr[i].deletable){
            delete arr[i].ptr;
        }
    }

    printf("[%s()].\n", __FUNCTION__);
    return 0;
}

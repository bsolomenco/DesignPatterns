#include "Factory.hpp"
#include <stdio.h>

//--------------------------------------------------------------------------------
class T0{
public:
    int nr;
    T0(int nr=0): nr(nr){}
    virtual ~T0(){printf("%s() this=%p nr=%4d\n", __FUNCTION__, this, nr);}
    virtual const char* cls(){return "T0";}//class ID
};

//--------------------------------------------------------------------------------
class T1: public T0{
public:
    T1(int nr=1): T0(nr){printf("%s(%4d) this=%p\n", __FUNCTION__, nr, this);}
    const char* cls() override {return "T1";}
};

//--------------------------------------------------------------------------------
class T2: public T0{
public:
    T2(int nr=2): T0(nr){printf("%s(%4d) this=%p\n", __FUNCTION__, nr, this);}
    const char* cls() override {return "T2";}
    static T0* create(){return new T2(1973);}//creator function
};

//--------------------------------------------------------------------------------
class T2Creator{//could be a life cycle manager for derived objects
public:
    T2* create(){
        static T2 singleton(2020);//destroyed @ exit (uncontrolled order)
        return &singleton;
    }
};

//--------------------------------------------------------------------------------
int main(int /*argc*/, char** /*argv*/){
    Factory<std::string, T0> factory;//factory for descendants of T0

    //factory.registerClass<T0>("T0");
    factory.registerType<T1>("T1");//using default creator for T1
    factory.registerType<T2>("T2");//using default creator for T2
    factory.registerType<T2>("T2_func", &T2::create);//function creator
    factory.registerType<T2>("T2_lambda"  , [](){return new T2(2018);});//lambda creator
    T2Creator t2Creator;
    factory.registerType<T2>("T2_object"  , [&t2Creator](){return t2Creator.create();});//lambda + object creator
    factory.registerType<T2>("T2_bind"    , std::bind(&T2Creator::create, t2Creator));//std::bind works but is discouraged since lambda

    T0* b = factory("T0");
    if(!b){printf("[%s()] \"%s\" not registered\n", __FUNCTION__, "T0");}

    {//simple usage
        printf("======== simple usage:\n");
        T1* t1 = factory("T1");//no need to cast explicitly, it happens automagically
        delete t1;
        T2* t2 = factory("T2");
        delete t2;
    }
    {//array
        printf("======== array:\n");
        struct{
            bool deletable;
            T0*  ptr;
        } arr[] = {//array of objects derived from T0 created via factory
            {true , factory("T1"            )},
            {true , factory("T2"            )}, 
            {true , factory("T2_func"       )}, 
            {true , factory("T2_lambda"     )}, 
            {false, factory("T2_object"     )},//no delete because it is a singleton 
            {false, factory("T2_bind"       )},//no delete because it is a singleton 
        };

        for(size_t i=0; i< sizeof(arr)/sizeof(arr[0]); ++i){
            if(arr[i].ptr){
                printf("%zd: this=%p %s nr=%d\n", i, arr[i].ptr, arr[i].ptr->cls(), arr[i].ptr->nr);
            }else{
                printf("%zd: nullptr\n", i);
            }
            if(arr[i].deletable){
                delete arr[i].ptr;
            }
        }
    }

    printf("[%s()].\n", __FUNCTION__);
    return 0;
}

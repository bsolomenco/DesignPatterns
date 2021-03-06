#if 0 //Factory for objects derived from same Base class
- maintains association between a key and a creator for a type
- returns a pointer to an instance of type associated with a key
- can be used for Abstract Factory Pattern by passing AbstractFactory interface as Base
- difference from traditional approach:
1. T0* t = factory("T1");
2. T1* t = dynamic_cast<T1>(factory("T1"));//explicit upcast (compiler cannot detect/help)
3. T2* t = dynamic_cast<T1>(factory("T1"));//detected at compile time
4. T2* t = dynamic_cast<T2>(factory("T1"));//not detected at compile time (compiler cannot detect/help)

1. T0* t = factory("T1");
2. T1* t = factory("T1");//explicit (behind the scene) upcast (compiler cannot detect/help)
3. T2* t = factory("T1");//explicit (behind the scene) cast; not detectable at compile time, only at run time
4. T2* t = factory("T1");//same as 3.

The only difference is at 3. but it is a small trade-off for easier usage
#endif
#pragma once
#include <functional>
#include<map>

template<typename Key, typename Base> class Factory{
public:
    template<typename T> void registerType(
        Key                    key, //ToDo: can deduce the key from T? then what about different keys for same type?
        std::function<Base*()> creator=_creator<T>//function or lambda (bind works but discouraged since lambdas)
    ){
        _creatorsMap[key] = creator;
    }

    Factory& operator()(Key key){
        auto f = _creatorsMap[key];
        _base = (f ? f() : nullptr);
        return *this;
    }

    //NOTE: this allows automatic casts but prevents compiler to detect wrong casts at compile time (but it cannot help also when explicit wrong casts are used)
    template<typename T> operator T(){return dynamic_cast<T>(_base);}//cast to whatever (derived) type you need

private:
    std::map<Key, std::function<Base*()>> _creatorsMap;
    Base*                                 _base;

    template<typename T> static Base* _creator(){return new T;}//default creator
};

#if 0 // USAGE
class T0{...}; //base class
class Ti: public T0{...}; //derived classes; i>0
Factory<std::string, T0> factory;

factory.registerType<T1>("T2");//using default creator
factory.registerType<T2>("T2");
factory.registerType<T2>("T2_create", &T2::create);//assume there is a static T0* T2::create()
factory.registerType<T2>("T2_lambda", [](){return new T2(...);}]);//lambda creator
//...

T1* t1 = factory("T1");//NOTE: no explicit cast needed!
T2* t2 = factory("T2");
T2* t3 = factory("T2_create");
T2* t4 = factory("T2_lambda");

//destroy instances (where necessary)
#endif

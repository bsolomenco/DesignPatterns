#if 0 //Factory for objects derived from same Base class
- maintains association between a key and a creator for a type
- returns a pointer to an instance of type associated with a key
- can be used for Abstract Factory Pattern by passing AbstractFactory interface as Base
#endif
#pragma once
#include <functional>
#include<map>

template<typename Key, typename Base> class Factory{
public:
    template<typename T> void registerType(
        Key                    key, //ToDo: can deduce the key from T?
        std::function<Base*()> creator=_creator<T>//function or lambda (bind works but discouraged since lambdas)
    ){
        _creatorsMap[key] = creator;
    }

    Factory& operator()(Key key){
        auto f = _creatorsMap[key];
        _base = (f ? f() : nullptr);
        return *this;
    }

    template<typename T> operator T(){return (T)_base;}

private:
    std::map<Key, std::function<Base*()>> _creatorsMap;
    Base*                                 _base;

    template<typename T> static Base* _creator(){return new T;}//default creator
};

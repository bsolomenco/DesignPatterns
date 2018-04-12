#if 0 //Factory for objects derived from same Base class
- maintains association between a key and a creator for a type
- returns a pointer to an instance of type associated with a key
- can be used for Abstract Factory Pattern by using AbstractFactory interface as Base
#endif
#pragma once
#include <functional>
#include<map>

template<typename Key, typename Base>class Factory{
public:
    template<typename T> void registerType(
        Key                    key, 
        std::function<Base*()> creator=_creator<T>//could be function or lambda (bind works but discouraged since lambdas)
    )
    {
        _creatorsMap[key] = creator;
    }

    template<typename T> T* create(Key key){
        if(!_creatorsMap[key]){
            return nullptr;
        }
        return (T*)_creatorsMap[key]();
    }

private:
    std::map<Key, std::function<Base*()>> _creatorsMap;

    template<typename T> static Base* _creator(){return new T;}//default creator
};

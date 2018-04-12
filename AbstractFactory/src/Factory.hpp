#pragma once
#include<map>
#include<string>
class Base;

class Factory{
private:
    typedef Base* (CreateFunc)();
    std::map<std::string, CreateFunc*> _createFuncMap;
public:
    template<typename T> void registerClass(const char* name){
        _createFuncMap[name] = &_createFunc<T>;
    }

    Base* create(const char* name){//ToDo: call ctor with variable number of args
        return _createFuncMap[name]();
    }
private:
    template<typename T> static Base* _createFunc(){return new T;}
};

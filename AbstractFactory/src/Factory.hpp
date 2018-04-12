#pragma once
#include<map>
#include<string>
class Base;

template<typename Base>class Factory{
public:
    typedef Base* Create();

    ~Factory(){
        _createFuncMap.clear();
    }

    template<typename T> void registerClass(const char* name, Create* createFunc=&_create<T>){
        _createFuncMap[name] = createFunc;
    }

    template<typename T> T* create(const char* name){//ToDo: call ctor with variable number of args
        if(!_createFuncMap[name]){
            printf("[%s()] \"%s\" not registered\n", __FUNCTION__, name);
            return nullptr;
        }
        return (T*)_createFuncMap[name]();
    }

private:
    template<typename T> static Base* _create(){return new T;}
    std::map<std::string, Create*> _createFuncMap;
};

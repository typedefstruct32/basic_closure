#include <iostream>

class Closure {
public:
    Closure() {}
    ~Closure() {}
    virtual void Run() = 0;
};

//ordinary function closure with no arguments
class FunctionClosure0 : public Closure {
public:
    typedef void (*FunctionType)();
    
    FunctionClosure0(FunctionType f) : _f(f) {}
    ~FunctionClosure0() {}
    //the object of this class is create by NewCallback(), So we call delete this in Run().
    virtual void Run() {
        _f();
        delete this;
    }
private:
    FunctionType _f;
};

//member function closure with no arguments
template <typename Class> 
class MethodClosure0 : public Closure { 
public:
    typedef void (Class::*MethodType)();  //function pointer to member function of Class object

    MethodClosure0(Class *object, MethodType m) :
        _object(object),
        _m(m) {
        }
    ~MethodClosure0() {    
    }

    virtual void Run() {
        std::cout<< "address of object in Run : " << _object << std::endl;
        (_object->*_m)();  //because function is no argument, so it is ().
        delete this;       //delete object after run this closure;
    }
private:
    Class* _object;
    MethodType _m;
};

Closure* NewCallback(void (*function)()) {
    return new FunctionClosure0(function);
}

template <typename Class>
Closure* NewCallback(Class* object, void (Class::*method)()) {    //2nd argument simply means a member function pointer which names "method".
    return new MethodClosure0<Class>(object, method);
}

class A {
public:
    void Method() {
        std::cout << "This is Method of Class A" << std::endl;
    }
};

int main() {
    
    //test for ordinary function closure with no arguments
    Closure* closure = NewCallback([]{ std::cout<< "a lambda function" << std::endl; });
    closure->Run();

    //test for member function closure with no arguments
    A* a = new A;   
    closure = NewCallback(a, &A::Method);
    closure->Run();
    //see details in https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/stubs/callback.h
}
#if 1

#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base::Base() constructor called." << std::endl;
        print(); // 在构造函数中调用虚函数
    }

    virtual ~Base() {
        std::cout << "Base::~Base() destructor called." << std::endl;
        print();
    }

    virtual void print() const {
        std::cout << "Base::print() called." << std::endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived::Derived() constructor called." << std::endl;
    }

    ~Derived() {
        std::cout << "Derived::~Derived() destructor called." << std::endl;
    }

    void print() const override {
        std::cout << "Derived::print() called." << std::endl;
    }
};

int main() {
    Derived d;
    return 0;
}

#endif
#include <iostream>
#include<memory>

using namespace std;

// Curiously recurring template pattern - f-bound polymorphism - static polymorphism technique
// The outwardly visible abstract interface class defines a set of common methods
// (and potentially other members) that are universally available to the component
// user, but independent of the underlying implementation.
// Interface classes contains no functionality themselves, but instead forward
// all method calls to an associated implementation class using a zero-cost abstraction
// technique. Interface classes serve to formalize the functional interface of a component,
// as well as provide a place on which to host implementation-neutral API documentation.

// Example 1) Fixed relationship between the interface and the implementation class
class InterfaceImpl;

class Interface
{
    // let's assume we only have one implementation of the Interface
    using ImplClass = InterfaceImpl;

    public:
    void DoSth();
    static void Init();
};

class InterfaceImpl: public Interface
{
    friend Interface;
    void _DoSth() {cout << "Doing sth concrete" << endl; }
    static void _Init() {cout << "Initialized" << endl; }
};

inline void Interface::DoSth() {return static_cast<ImplClass*>(this)->_DoSth();}
inline void Interface::Init() {return ImplClass::_Init();}

//////////////////////////////////////////////////////////////////////////////////////////////

// Example 2) Flexible relationship between the interface and the implementation class

// now let's assume there might be multiple implementations of the generic interface
template<class T>
class Animal
{
    public:
    void Sound() { static_cast<T*>(this)->_Sound(); }
};

class Dog: public Animal<Dog>
{
    friend Animal;
    void _Sound() { cout << "Wrah wrah!" << endl; }
};

class Cat: public Animal<Cat>
{
    friend Animal;
    void _Sound() { cout << "Miew miew!" << endl; }
};

int main()
{
    Interface * generic_iface = new InterfaceImpl;
    generic_iface->Init();
    generic_iface->DoSth();
    delete generic_iface;

    unique_ptr<Animal<Dog>> animal_dog = make_unique<Dog>();
    animal_dog->Sound();

    auto animal_cat = make_unique<Cat>();
    animal_cat->Sound();
} 


// Example 3) ConnectivityManager from Matter SDK
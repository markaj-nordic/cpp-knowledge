#include <iostream>
#include<memory>

using namespace std;

// std::unique_ptr
// lightweight smart pointer that implements exclusive ownership of the object
// when the uniqe_ptr goes out of scope or starts to own another object, the owned object is deleted automatically
// additional overhead may come with the default deleter

class MyObject
{
public:
   MyObject(const string& name) : mName(name){cout << "MyObject created" << endl;}
    ~MyObject() {cout << "MyObject deleted" << endl;}
   void Print() {cout << mName << endl;}

private:
      std::string mName;
};

void dummyFunc()
{
    unique_ptr<MyObject> obj_ptr(new MyObject("abc"));
    obj_ptr->Print();

    // unique_ptr API usage examples

    // MyObject* rawObj =  obj_ptr.release(); // will not be managed by uniqe_ptr and will not be deleted automatically
    // obj_ptr.reset(new MyObject("def")); // will destroy the previously owned object first and then own a new one
    // obj_ptr.reset(); // will destroy the previously owned object and remain empty, the same as obj_ptr = nullptr
    //delete rawObj;


    // exchanging ownership between two unique_ptrs
    unique_ptr<MyObject> one (new MyObject("Nordic"));
    unique_ptr<MyObject> two (new MyObject("Silabs"));
    two->Print();
    two = std::move(one); // 1.Silabs is destroyed, 2. one becomes empty, 3.two owns Nordic
}

// passing unique_ptr to the function and from the function
void Foo(unique_ptr<MyObject> ptr)
{
    ptr->Print();
}

unique_ptr<MyObject> Get()
{
    unique_ptr<MyObject> obj(new MyObject("object"));
    return obj; // automatically calls std::move under the hood
}

int main()
{
    dummyFunc();

    // unique_ptr as an input
    auto obj1 = std::make_unique<MyObject>("some name"); // an example of using make_unique -> a rule of thumb applies here
    Foo(std::move(obj1)); // won't work without std::move

    // unique_ptr as an output
    unique_ptr<MyObject> obj2 = Get();
}


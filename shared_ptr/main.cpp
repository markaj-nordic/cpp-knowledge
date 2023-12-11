#include <iostream>
#include<memory>

using namespace std;

// std::shared_ptr
// Keeps track of how many pointers are pointing to the given object.
// When the number equals 0, the resource is deleted.
// An object should be assigned to the shared_ptr as soon as it's created, do not use raw pointer.
// Automatic memory management, shared ownership. Increases the overhead due to the reference counting.

// It may be accompanied by the weak_ptr - the class that acts as an observer of managed shared resource
// without increasing its reference (without claiming ownership)

class MyObject
{
public:
   MyObject(const string& name) : mName(name){cout << "MyObject " << mName << " created" << endl;}
   MyObject() : mName(""){cout << "MyObject created" << endl;}
    ~MyObject() {cout << "MyObject " << mName << " deleted" << endl;}
   void Print() {cout << mName << endl;}

private:
      std::string mName;
};

void test1()
{
    shared_ptr<MyObject> o (new MyObject("test1")); // count = 1
    cout << "Count = " << o.use_count() << endl;

    {
        shared_ptr<MyObject> o1 = o; // count = 2
        cout << "Count = " << o.use_count() << endl;
        o1->Print();
    } // count = 1

    o->Print();
} // count = 0;

using Ptr = shared_ptr<MyObject>;

void test2()
{
    MyObject *o = new MyObject("test2");

    Ptr p1(o);
    cout << "Count = " << p1.use_count() << endl;
    Ptr p2(o);
    cout << "Count = " << p2.use_count() << endl;
    // undefined behavior
}

void test3()
{
    Ptr p(new MyObject("NotSafe")); // 1. MyObject is created 2. p is created
    // what in case the p cannot be created for any reason? MyObject will not be managed.
    Ptr p1 = make_shared<MyObject>("Safe"); // exception safe, single allocation for both the control block and managed object, but no custom deleter supported
} 

void test4()
{
    // shared_ptr API is similar to the unique_ptr (excluding c'tors and move semantics)
    Ptr p1 = make_shared<MyObject>("Bob");
    Ptr p2 = make_shared<MyObject>("Chris");
    //p1 = p2; // Bob is deleted (no more referenced)
    //p1 = nullptr; // Bob is deleted (no more referenced)
    p1.reset(); // Bob is deleted (no more referenced)

    MyObject* rawPtr = p1.get(); // returns a raw pointer

    p2.reset(new MyObject);
} 

void test5()
{
    Ptr p1 = shared_ptr<MyObject>(new MyObject("P1")); // default deleter
    Ptr p2 = shared_ptr<MyObject>(new MyObject("P2"), [](MyObject* o){
            cout << "Custom deleter!" << endl;
            delete o; 
        }); // custom deleter
    Ptr p3 (new MyObject[3]); // p3 only manages fist element, the rest has memory leaks
    Ptr p4 (new MyObject[3], [](MyObject* o){
        delete[] o;
    }); // all 3 are deletet when the p4 goes out of scope
    // custom deleter is handy when managing more complex objects
    // (e.g. requiring any additional cleanup like closing file handle, sockets etc.)
}

void test6()
{
    // the weak_ptr does not guarantee that the underlying object exists
    // hence the sanity check is needed everytime one wants to access the managed object
    shared_ptr<MyObject> p1 = shared_ptr<MyObject>(new MyObject("P1"));
    cout << "Count " << p1.use_count() << endl;
    weak_ptr<MyObject> wp = p1;
    cout << "Count " << p1.use_count() << endl;

    if (shared_ptr<MyObject> locked = wp.lock()) {
        cout << "Count " << p1.use_count() << endl;
        locked->Print();
    }

    p1.reset();

    if (wp.expired()) {
        cout << "The weak_ptr is expired!" << endl;
    }
}

void test7()
{
    // circular dependency
    struct Member;

    struct MatterTeam {
        shared_ptr<Member> qa;
        ~MatterTeam() {cout << "Matter Team destructed! Nordic Semi can be closed." << endl; }
    };

    struct Member {
        shared_ptr<MatterTeam> team;
        ~Member() {cout << "Member destroyed!" << endl; }
    };

    auto matter = make_shared<MatterTeam>();
    auto ms = make_shared<Member>();

    matter->qa = ms;
    ms->team = matter;
}

int main()
{
    // MyObject* o = new MyObject("abc");

    // delete o;

    // o->Print();

    test7();
} 


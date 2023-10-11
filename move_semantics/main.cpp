#include <iostream>
#include <cstring>

using namespace std;
using Type = unsigned char;

class Vector {
  public:
    Vector(): mBuffer(nullptr), mSize(0) {} // default constructor
    Vector(Type* buffer, size_t size) : mBuffer(buffer), mSize(size){}
    //~Vector() {delete[] mBuffer;}

    // copy constructor
    Vector(const Vector& other)
    {
        cout << "Copy constructor" << endl;
        mSize = other.mSize;
        mBuffer = new Type[mSize];
        memcpy(mBuffer, other.mBuffer, mSize);
    }

    // copy assignment operator
    Vector& operator=(const Vector& other)
    {
        cout << "Copy assignement operator" << endl;
        mSize = other.mSize;
        if (mBuffer) delete[] mBuffer;
        mBuffer = new Type[mSize];
        memcpy(mBuffer, other.mBuffer, mSize);
        return *this;
    }

// Move semantics is a C++ feature introduced in C++11 that allows for more efficient copying of objects.
// Instead of copying all the data from one object to another, move semantics allows for the transfer
// of ownership of the data from one object to another.
// This can be much faster and more efficient than making a copy, particularly when dealing with large objects.

// In move semantics, objects are moved instead of copied. This is done using a move constructor
// and a move assignment operator. The move constructor takes an rvalue reference to another
// object and “steals” its resources, leaving the original object in a valid but unspecified state.
// The move assignment operator is similar but takes an rvalue reference to another object and
// moves its resources into the target object.

    // move constructor
    Vector(Vector&& other)
    {
        cout << "Move constructor" << endl;
        mSize = other.mSize;
        mBuffer = other.mBuffer;
        // cleanup the source object
        other.mBuffer = nullptr;
        other.mSize = 0;
    }

    // move assignment operator
    Vector& operator=(Vector&& other)
    {
        cout << "Move assignement operator" << endl;
        mSize = other.mSize;
        mBuffer = other.mBuffer;
        // cleanup the source object
        other.mBuffer = nullptr;
        other.mSize = 0;
        return *this;
    }

  private:
    Type* mBuffer;
    size_t mSize;
};

//void PrintInt(int value) { cout << "copy: " << value << endl; } // results in an ambiguous declaration
void PrintInt(int& value) { cout << "lvalue reference: " << value << endl; }
void PrintInt(int&& value) { cout << "rvalue reference: " << value << endl; }

int main(){

// rvalue reference
    int var = 123;
    int& var_ref = var;
    int&& var_rvref = std::move(var_ref); //std::move doesn't actually move anything out of it's own. It's just a syntactic sugar for a cast to a T&&

    PrintInt(var);
    PrintInt(6);

// move semantics
    static Type sArr[123];
    Vector a(sArr, sizeof(sArr)/sizeof(Type));
    Vector b = a; // copy constructor
    Vector c(b); // copy constructor
    Vector d;
    d = a; // copy assignment operator
    Vector m = std::move(d);
    d = std::move(m);

    // In the above code, std::move is used to cast a to an rvalue reference,
    // allowing it to be moved instead of copied. When b is initialized with a,
    // a move constructor is called to transfer ownership of a’s resources to b.
    // Similarly, when c is assigned a, a move assignment operator is called to
    // transfer ownership of a’s resources to c.
}


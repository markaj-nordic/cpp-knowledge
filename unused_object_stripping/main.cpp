#include <iostream>
#include <cstdint>

using namespace std;

/////////////////////////////// STACK ///////////////////////////////
struct AIface;
struct BIface;

class Stack
{
public:
    static Stack & Instance()
    {
        static Stack sInstance;
        return sInstance;
    }
    void SetA(AIface *a) { mAIface = a; }
    void SetB(BIface *b) { mBIface = b; }
private:
    AIface *mAIface{nullptr};
    BIface *mBIface{nullptr};
};

struct AIface
{
    virtual void DoA() = 0;
};

struct BIface
{
    virtual void DoB() = 0;
};
////////////////////////////////////////////////////////////////////


/////////////////////////////// APP ///////////////////////////////
struct AImplDefault : public AIface
{
    void DoA() override
    {
        cout << "Doing A default implementation";
    }
};

struct BImplDefault : public BIface
{
    void DoB() override 
    {
        cout << "Doing B default implementation";
    }
    int id{};
};

struct InitData
{
    static AImplDefault sADefault;
    static BImplDefault sBDefault;

    AIface *mA {&sADefault};
    BIface *mB {&sBDefault};
};

//AImplDefault InitData::sADefault{}; // by commenting this out we emulate linker's optimization that strips off the unused object
// NOTE: stripping of the unused objects is possible thanks to -fdata-sections linker option:
// https://developer.arm.com/documentation/101754/0621/armclang-Reference/armclang-Command-line-Options/-ffunction-sections---fno-function-sections
// Similarly for unused functions the following one must be used -ffunction-sections
BImplDefault InitData::sBDefault{};

void InitStack(const InitData& init)
{
    Stack::Instance().SetA(init.mA);
    Stack::Instance().SetB(init.mB);
}

////////////////////////////////////////////////////////////////////


struct AImplCustom : public AIface
{
    void DoA() override
    {
        cout << "Doing A CUSTOM implementation";
    }
};

static AImplCustom ACustom;

int main()
{
    InitData init{.mA = &ACustom};
    InitStack(init);
}

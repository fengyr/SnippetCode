#include <iostream>

using namespace std;

class A {
    public :
        virtual void hello() { cout << "A::hello" << endl; }
        virtual void hi() { cout << "A::hi" << endl; }
};

class B {
    public :
        virtual void hello() { cout << "B::hello" << endl; }
        virtual void hi() { cout << "B::hi" << endl; }
};

class C {
    public :
        virtual void hello() { cout << "C::hello" << endl; }
        virtual void hi() { cout << "C::hi" << endl; }
};

class ABC : public A, public B, public C {
    public :
        virtual void hello() { cout << "ABC::hello" << endl; }
        virtual void say_hi() { cout << "ABC::say_hi" << endl; }
};

typedef void (*Fun)(void);

int main()
{
    Fun pFun = NULL;
    ABC abc;
    int **pVtab = (int **)&abc;

    /************************  A的虚表 ************************/
    std::cout << "=========== A类虚表 ===========" << std::endl;
    pFun = (Fun)*((int*)(*((int*)&abc+0)+0));
    // pFun = (Fun)pVtab[0][0];
    pFun();

    pFun = (Fun)*((int*)(*(int*)((int*)&abc+0)+1));
    // pFun = (Fun)pVtab[0][1];
    pFun();

    pFun = (Fun)*((int*)(*((int*)&abc+0)+2));
    // pFun = (Fun)pVtab[0][2];
    pFun();

    //The tail of the vtable
    pFun = (Fun)pVtab[0][3];
    cout<<pFun<<endl;
    cout<<"\n";

    /************************  B类虚表 ************************/
    std::cout << "=========== B类虚表 ===========" << std::endl;
    //pFun = (Fun)*((int*)*(int*)((int*)&abc+1)+0);
    pFun = (Fun)pVtab[1][0];
    pFun();

    //pFun = (Fun)*((int*)*(int*)((int*)&abc+1)+1);
    pFun = (Fun)pVtab[1][1];
    pFun();

    //The tail of the vtable
    pFun = (Fun)pVtab[1][2];
    cout<<pFun<<endl;
    cout<<"\n";

    /************************  C类虚表 ************************/
    std::cout << "=========== C类虚表 ===========" << std::endl;
    //pFun = (Fun)*((int*)*(int*)((int*)&abc+1)+0);
    pFun = (Fun)pVtab[2][0];
    pFun();

    //pFun = (Fun)*((int*)*(int*)((int*)&abc+1)+1);
    pFun = (Fun)pVtab[2][1];
    pFun();

    //The tail of the vtable
    pFun = (Fun)pVtab[2][2];
    cout<<pFun<<endl;

    return 0 ;
} 

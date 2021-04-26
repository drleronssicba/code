#include <iostream>
#include <string>
using namespace std;

//ARII资源分配即初始化，定义一个类来封装资源的分配和初始化

//Scoped_Ptr 管理权的转移，不建议使用
template<class T>
class Scoped_Ptr{
public:
    Scoped_Ptr(T* ptr) 
        :_ptr(ptr)
    {
        cout<<"Scoped_Ptr(T* ptr)"<<endl;
    }

    
    T& operator*() {
        return *_ptr;
    }

    T* operator->() {
        return _ptr;
    }

    ~Scoped_Ptr() {
        cout<<"~Scoped_Ptr()"<<endl;
        delete _ptr;
    }
private:
    Scoped_Ptr(Scoped_Ptr<T>& ap);   //之声明，不实现是为了防拷贝，  设置为私有是为了防止继承重写
    T& operator=(Scoped_Ptr<T>& ap);

private:
    T* _ptr;
};

void Test_Scoped_Ptr() {
    int* p = new int(1);
    Scoped_Ptr<int> sp1(p);
    cout<<*p<<endl;
    *sp1 = 10;
    cout<<*p<<endl;
}

int main()
{
    Test_Scoped_Ptr();

    return 0;
}

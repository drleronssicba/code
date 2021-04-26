#include <iostream>
#include <string>
using namespace std;

//ARII资源分配即初始化，定义一个类来封装资源的分配和初始化

//Auto_Ptr 管理权的转移，不建议使用
template<class T>
class Auto_Ptr{
public:
    Auto_Ptr(T* ptr) 
        :_ptr(ptr)
    {
        cout<<"Auto_Ptr(T* ptr)"<<endl;
    }

    Auto_Ptr(Auto_Ptr<T>& ap) 
        :_ptr(ap._ptr)
    {
        ap._ptr = NULL;
    }
    
    T& operator*() {
        return *_ptr;
    }

    T* operator->() {
        return _ptr;
    }

    T& operator=(Auto_Ptr<T>& ap) {
        if(this != &ap) {
            if(_ptr != NULL) {
                delete _ptr;
            }
            _ptr = ap._ptr;
            ap._ptr = NULL;
        }
    }

    ~Auto_Ptr() {
        cout<<"~Auto_Ptr()"<<endl;
        delete _ptr;
    }
//private:
    T* _ptr;
};

void Test_Auto_Ptr() {
    int* p = new int(1);
    Auto_Ptr<int> ap1(p);
    Auto_Ptr<int> ap2(ap1);
    cout<<"ap1:"<<ap1._ptr<<"  ap2:"<<ap2._ptr<<endl;

    ap1 = ap2;
    cout<<"ap1:"<<ap1._ptr<<"  ap2:"<<ap2._ptr<<endl;
    
    *ap1 = 10;
    cout<<*ap1<<endl;
}

int main()
{
    Test_Auto_Ptr();

    return 0;
}

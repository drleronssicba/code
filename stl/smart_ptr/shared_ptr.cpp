#include <iostream>
using namespace std;

template<class T> 
class Shared_Ptr{
public:
    Shared_Ptr(T* str = NULL) 
        :_ptr(str)
        ,_count(new int(1))
    {
        cout<<"str:"<<_ptr<<endl;
        cout<<"count:"<<(*_count)<<endl;
    }

    Shared_Ptr(Shared_Ptr<T> &sp) 
        :_ptr(sp._ptr)
        ,_count(sp._count)
    {
        ++(*_count);
    }

    T& operator=(Shared_Ptr<T> &sp) {
        if(_ptr != NULL) {
            this->~Shared_Ptr();
        }
        if(this != &sp) {
            _ptr = sp._ptr;
            _count = sp._count;
            ++(*_count);
        }
        return *_ptr;
    }

    T& operator*() {
        return *_ptr;
    }

    T* operator->() {
        return _ptr;            
    }

    ~Shared_Ptr() {
        if(*_count == 1) {
            delete  _ptr;
            delete _count;
            cout<<"引用计数为0，对象销毁"<<endl;
        } else {
            --*_count;
            cout<<"str:"<<_ptr<<endl;
            cout<<"count:"<<(*_count)<<endl;
        }
    }

private:
    T* _ptr;
    int* _count;
};

void Test_Shared_Ptr() {
    int* p = new int(1);
    int* q = new int(1);
    Shared_Ptr<int> sp1(p);
    Shared_Ptr<int> sp2(sp1);
    Shared_Ptr<int> sp3(sp1);
    Shared_Ptr<int> sp4(sp1);
    Shared_Ptr<int> sp5(q);
    sp5 = sp1;

}



int main()
{
    Test_Shared_Ptr();
    return 0;
}

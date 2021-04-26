#include <iostream>
#include <vector>
using namespace std;

template<class T>
class Sort{
    T& operator=(vector<T>& v);
public:

//直接插入排序
//在插入环节可优化为二分查找
    void InsertSort(vector<T>& v) {
        for(int i = 1; i < v.size(); ++i) {
            for(int j = i; j > 0; --j) {
                if(v[j] > v[j-1]) {
                    swap(v[j], v[j-1]);
                    continue;
                }
                break;
            }
        }
    }    



//希尔排序
    void ShellSort(vector<int>& v) {

    }

//选择排序
//
    void SelectSort(vector<T>& v) {
        for(int i = 0; i<v.size(); ++i) {
            int Min = i;
            for(int j = i; j<v.size(); ++j) {
                if(v[j] > v[Min]) {
                    Min = j;
                }
            }
            swap(v[Min], v[i]);
        }    
    }


//堆排序

//冒泡排序
    void BubbleSort(vector<T>& v) {
        bool flag = false;
        for(int i = 0; i < v.size() - 1; ++i) {
            for(int j = i; j < v.size() - 1; ++j) {
                if(v[j] < v[j + 1]) {
                    swap(v[j], v[j + 1]);
                    flag = true;
                }
            } 
            if(!flag) {
                break;
            }
            flag = false;
        }
    }


//快速排序
//归并排序
};


template<class T>
void Despaly(vector<T>& v) {
    for(int i = 0; i<v.size(); ++i) {
        cout<<v[i]<<" ";
    }    
    cout<<endl;
}

int main() {
    vector<int> v;
    v.push_back(1);
    v.push_back(4);
    v.push_back(2);
    v.push_back(3);
    
    //Sort<int>().BubbleSort(v);  
    //Sort<int>().SelectSort(v);
    Sort<int>().InsertSort(v);
    Despaly(v);
}

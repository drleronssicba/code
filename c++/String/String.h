// 1.现代写法 
// 2.传统写法 
// 3.字符串增删查改 
#include <iostream>
#include <unistd.h>
#include <string.h>
class String 
{ 
  public: 
    String(const char* str = "");
    // s1.Swap(s2); 
    void Swap(String& s); 

    // String s2(s1) 
    String(const String& s); 
    // s1 = s2 
    String& operator=(String s); 

    ~String(); 

    const char* c_str(); 
    void Expand(size_t n); 
    void PushBack(char ch);
    void PushBack(const char* str); 
    void PopBack(); 
    void Insert(size_t pos, char ch); 
    void Insert(size_t pos, const char* str); 
    void Erase(size_t pos, size_t n = 1); 


    size_t Find(char ch); 
    size_t Find(const char* str); // ? 

    // s1 + 'a' 
    String operator+(char ch); 
    String& operator+=(char ch); 
    String operator+(const char* str); 
    String& operator+=(const char* str);

    bool operator>(const String& s); 
    bool operator>=(const String& s); 
    bool operator<(const String& s); 
    bool operator<=(const String& s); 
    bool operator==(const String& s); 
    bool operator!=(const String& s); 

  private: 
    char* _str; 
    size_t _size; 
    size_t _capacity; 
}; 


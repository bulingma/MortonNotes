
<!-- TOC -->

- [C++11新特性](#c11新特性)
  - [auto & decltype](#auto--decltype)
  - [左值右值](#左值右值)
  - [列表初始化](#列表初始化)
  - [std::function & std::bind & lambda表达式](#stdfunction--stdbind--lambda表达式)
  - [模板的改进](#模板的改进)
  - [并发](#并发)
  - [智能指针](#智能指针)
  - [final & override](#final--override)
  - [default/delete/explicit](#defaultdeleteexplicit)
    - [default](#default)
    - [delete](#delete)
    - [explicit](#explicit)
  - [const & constexpr](#const--constexpr)
    - [const](#const)
    - [constexpr](#constexpr)
  - [内存对齐](#内存对齐)
  - [thread_local](#thread_local)
  - [正则表达式](#正则表达式)
  - [chrono](#chrono)
  - [新增数据结构](#新增数据结构)
  - [新增算法](#新增算法)
  - [](#)
- [参考](#参考)

<!-- /TOC -->


## C++11新特性
### auto & decltype
关于C++11新特性，最先提到的肯定是类型推导，C++11引入了auto和decltype关键字，使用他们可以在编译期就推导出变量或者表达式的类型，方便开发者编码也简化了代码。  
* auto：让编译器在编译器就推导出变量的类型，可以通过=右边的类型推导出变量的类型。  
```cpp
auto a = 10; // 10是int型，可以自动推导出a是int
```

* decltype：相对于auto用于推导变量类型，而decltype则用于推导表达式类型，这里只用于编译器分析表达式的类型，表达式实际不会进行运算。  
```cpp
cont int &i = 1;
int a = 2;
decltype(i) b = 2; // b是const int&
```

### 左值右值
左值是指表达式结束后依然存在的持久对象，右值是指表达式结束就不再存在的临时对象。一个区分左值右值的便捷方法是：看能不能对表达式取地址，如果能，则为左值，否则为右值。  
在C++11中，右值由两个概念构成，一个是`将亡值（xvalue，expiring value）`,另一个则是`纯右值（prvalue, PureRvalue）`。  
C++11中所有的值必属于左值、将亡值、纯右值三者之一。  

* 纯右值  
比如，非引用返回的临时变量、运算表达式产生的临时变量、原始字面量和lambda表达式等都是纯右值。  

* 将亡值  
C++11新增的、与右值引用相关的表达式，比如，将要被移动的对象、T&&函数返回值、std::move返回值和转换为T&&的类型的转换函数的返回值。  


众所周知C++11新增了右值引用，这里涉及到很多概念：

* 左值：
左值是指表达式结束后依然存在的持久对象，可以取地址并且有名字的东西就是左值。  

* 右值：  
不能取地址的没有名字的东西就是右值。在C++11中，右值由两个概念构成，一个是`将亡值（xvalue，expiring value）`,另一个则是`纯右值（prvalue, PureRvalue）`。C++11中所有的值必属于左值、将亡值、纯右值三者之一。     

* 纯右值：
运算表达式产生的临时变量、不和对象关联的原始字面量、非引用返回的临时变量、lambda表达式等都是纯右值。  

* 将亡值：  
C++11新增的、与右值引用相关的表达式，比如，将要被移动的对象、T&&函数返回值、std::move返回值和转换为T&&的类型的转换函数的返回值。可以理解为即将要销毁的值。    

* 左值引用：  
对左值进行引用的类型。  

* 右值引用：    
对右值进行引用的类型。  

* 移动语义：  
转移资源所有权，类似于转让或者资源窃取的意思，对于那块资源，转为自己所拥有，别人不再拥有也不会再使用。  

* 完美转发：  
可以写一个接受任意实参的函数模板，并转发到其它函数，目标函数会收到与转发函数完全相同的实参。  

* 返回值优化：  
当函数需要返回一个对象实例时候，就会创建一个临时对象并通过复制构造函数将目标对象复制到临时对象，这里有复制构造函数和析构函数会被多余的调用到，有代价，而通过返回值优化，C++标准允许省略调用这些复制构造函数。

[左值引用、右值引用、移动语义、完美转发，你知道的不知道的都在这里](https://zhuanlan.zhihu.com/p/137662465)


### 列表初始化  
在C++11中可以直接在变量名后面加上初始化列表来进行对象的初始化。  

### std::function & std::bind & lambda表达式  
c++11新增了std::function、std::bind、lambda表达式等封装使函数调用更加方便  

### 模板的改进
* C++11关于模板有一些细节的改进：  
模板的右尖括号  
模板的别名  
函数模板的默认模板参数    

[c++11新特性之模板的改进](https://zhuanlan.zhihu.com/p/137851516)

### 并发  
c++11关于并发引入了好多好东西，有：  

* std::thread相关
* std::mutex相关
* std::lock相关
* std::atomic相关
* std::call_once相关
* volatile相关
* std::condition_variable相关
* std::future相关
* async相关

[c++11新特性之线程相关所有知识点](https://zhuanlan.zhihu.com/p/137914574)


### 智能指针  
很多人谈到c++，说它特别难，可能有一部分就是因为c++的内存管理吧，不像java那样有虚拟机动态的管理内存，在程序运行过程中可能就会出现内存泄漏，然而这种问题其实都可以通过c++11引入的智能指针来解决，相反我还认为这种内存管理还是c++语言的优势，因为尽在掌握。  

c++11引入了三种智能指针：  

* std::shared_ptr  
* std::weak_ptr  
* std::unique_ptr  

### final & override  
c++11关于继承新增了两个关键字，final用于修饰一个类，表示禁止该类进一步派生和虚函数的进一步重载，override用于修饰派生类中的成员函数，标明该函数重写了基类函数，如果一个函数声明了override但父类却没有这个虚函数，编译报错，使用override关键字可以避免开发者在重写基类函数时无意产生的错误。  

### default/delete/explicit  
#### default  
c++11引入default特性，多数时候用于声明构造函数为默认构造函数，如果类中有了自定义的构造函数，编译器就不会隐式生成默认构造函数。    

```cpp
class A {
    int a;
    A(int i) { a = i; }
};

int main() {
    A a; // 编译出错
    return 0;
}
```
上面代码编译出错，因为没有匹配的构造函数，因为编译器没有生成默认构造函数，而通过default，程序员只需在函数声明后加上“=default;”，就可将该函数声明为 defaulted 函数，编译器将为显式声明的 defaulted 函数自动生成函数体，如下：
```cpp
class A {
    A() = default;
    int a;
    A(int i) { a = i; }
};

int main() {
    A a;
    return 0;
}
```
编译通过。  

#### delete  
c++中，如果开发人员没有定义特殊成员函数，那么编译器在需要特殊成员函数时候会隐式自动生成一个默认的特殊成员函数，例如拷贝构造函数或者拷贝赋值操作符，如下代码：
```cpp
class A {
    A() = default;
    int a;
    A(int i) { a = i; }
};

int main() {
    A a1;
    A a2 = a1;  // 正确，调用编译器隐式生成的默认拷贝构造函数
    A a3;
    a3 = a1;    // 正确，调用编译器隐式生成的默认赋值构造函数
}
```  
而我们有时候想禁止对象的拷贝与赋值，可以使用delete修饰，如下：  
```cpp
class A {
    A() = default;
    A(const A&) = delete;
    A& operator=(const A&) = delete;
    int a;
    A(int i) { a = i; }
};

int main() {
    A a1;
    A a2 = a1;  // 错误，拷贝构造函数被禁用
    A a3;
    a3 = a1;    // 错误，赋值操作符被禁用
}
```
delele函数在c++11中很常用，std::unique_ptr就是通过delete修饰来禁止对象的拷贝的。  

#### explicit  
explicit专用于修饰构造函数，表示只能显式构造，不可以被隐式转换，根据代码看explicit的作用：

不用explicit：
```cpp
class A {
    A(int value) { // 没有explicit关键字
        cout << "value" << endl;
    }
};

int main() {
    A a = 1; // 可以隐式转换
    return 0;
}
```
使用explicit:
```cpp
class A {
    explicit A(int value) {
        cout << "value" << endl;
    }
};

int main() {
    A a = 1; // error，不可以隐式转换
    A aa(2); // ok
    return 0;
}
```

### const & constexpr
#### const
因为要讲后面的constexpr，所以这里简单介绍下const。  
const字面意思为只读，可用于定义变量，表示变量是只读的，不可以更改，如果更改，编译期间就会报错。  

主要用法如下：

1. 用于定义常量，const的修饰的变量不可更改。
```cpp
const int value = 5;
```  

2. 指针也可以使用const，这里有个小技巧，从右向左读，即可知道const究竟修饰的是指针还是指针所指向的内容。  
```cpp
char *const ptr; // 指针本身是常量
const char* ptr; // 指针指向的变量为常量
```

3. 在函数参数中使用const，一般会传递类对象时会传递一个const的引用或者指针，这样可以避免对象的拷贝，也可以防止对象被修改。  
```cpp
class A{};
void func(const A& a);
```

4. const修饰类的成员变量，表示是成员常量，不能被修改，可以在初始化列表中被赋值。
```cpp
class A {
    const int value = 5;
};
class B {
    const int value;
    B(int v) : value(v){}
};
```

5. 修饰类成员函数，表示在该函数内不可以修改该类的成员变量。  
```cpp
class A{
    void func() const;
};
```

6. 修饰类对象，类对象只能调用该对象的const成员函数。  
```cpp
class A {
    void func() const;
};
const A a;
a.func();
```

#### constexpr
constexpr是c++11新引入的关键字，<u>用于编译时的常量和常量函数</u>，这里直接介绍constexpr和const的区别：

两者都代表可读，<u>const只表示read only的语义，只保证了运行时不可以被修改，但它修饰的仍然有可能是个动态变量，而constexpr修饰的才是真正的常量，它会在编译期间就会被计算出来，整个运行过程中都不可以被改变。</u> constexpr可以用于修饰函数，这个函数的返回值会尽可能在编译期间被计算出来当作一个常量，但是如果编译期间此函数不能被计算出来，那它就会当作一个普通函数被处理。如下代码：  
```cpp
#include<iostream>
using namespace std;

constexpr int func(int i) {
    return i + 1;
}

int main() {
    int i = 2;
    func(i);// 普通函数
    func(2);// 编译期间就会被计算出来
}
```

### 内存对齐
* 什么是内存对齐  
理论上计算机对于任何变量的访问都可以从任意位置开始，然而实际上系统会对这些变量的存放地址有限制，通常将变量首地址设为某个数N的倍数，这就是内存对齐。

* 为什么要内存对齐  
硬件平台限制，内存以字节为单位，不同硬件平台不一定支持任何内存地址的存取，一般可能以双字节、4字节等为单位存取内存，为了保证处理器正确存取数据，需要进行内存对齐。
提高CPU内存访问速度，一般处理器的内存存取粒度都是N的整数倍，假如访问N大小的数据，没有进行内存对齐，有可能就需要两次访问才可以读取出数据，而进行内存对齐可以一次性把数据全部读取出来，提高效率。
在c++11之前如果想创建内存对齐需要：
```cpp
void align_cpp11_before()
{
    static char data[sizeof(void *) + sizeof(A)];
    const uintptr_t kAlign = sizeof(void *) - 1;
    char *align_ptr =
        reinterpret_cast<char *>(reinterpret_cast<uintptr_t>(data + kAlign) & ~kAlign);
    A *attr = new (align_ptr) A;
}
```

c++11关于内存对齐新增了一些函数：
```cpp
void align_cpp11_after()
{
    static std::aligned_storage<sizeof(A),
                                alignof(A)>::type data;
    A *attr = new (&data) A;
}
```
还有：alignof()、std::alignment_of()、alignas()，关于内存对齐详情可以看这篇文章：[内存对齐](https://zhuanlan.zhihu.com/p/139520591)

### thread_local  
c++11引入thread_local，用thread_local修饰的变量具有thread周期，每一个线程都拥有并只拥有一个该变量的独立实例，一般用于需要保证线程安全的函数中。  
```cpp
#include <iostream>
#include <thread>

class A {
   public:
    A() {}
    ~A() {}

    void test(const std::string &name) {
        thread_local int count = 0;
        ++count;
        std::cout << name << ": " << count << std::endl;
    }
};

void func(const std::string &name) {
    A a1;
    a1.test(name);
    a1.test(name);
    A a2;
    a2.test(name);
    a2.test(name);
}

int main() {
    std::thread(func, "thread1").join();
    std::thread(func, "thread2").join();
    return 0;
}

// 输出：
thread1: 1
thread1: 2
thread1: 3
thread1: 4
thread2: 1
thread2: 2
thread2: 3
thread2: 4
```

### 正则表达式
c++11引入了regex库更好的支持正则表达式，见代码：
```cpp
#include <iostream>
#include <iterator>
#include <regex>
#include <string>

int main() {
    std::string s = "I know, I'll use2 regular expressions.";
    // 忽略大小写
    std::regex self_regex("REGULAR EXPRESSIONS", std::regex_constants::icase); 
    if (std::regex_search(s, self_regex)) {
        std::cout << "Text contains the phrase 'regular expressions'\n";
    }

    std::regex word_regex("(\\w+)");  // 匹配字母数字等字符
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = std::sregex_iterator();

    std::cout << "Found " << std::distance(words_begin, words_end) << " words\n";

    const int N = 6;
    std::cout << "Words longer than " << N << " characters:\n";
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        if (match_str.size() > N) {
            std::cout << "  " << match_str << '\n';
        }
    }

    std::regex long_word_regex("(\\w{7,})");
    // 超过7个字符的单词用[]包围
    std::string new_s = std::regex_replace(s, long_word_regex, "[$&]");
    std::cout << new_s << '\n';
}
```

### chrono  
c++11关于时间引入了chrono库，源于boost，功能强大，chrono主要有三个点：  
* duration  
* time_point  
* clocks  

### 新增数据结构
* std::forward_list：单向链表，只可以前进，在特定场景下使用，相比于std::list节省了内存，提高了性能
* std::unordered_set：基于hash表实现的set，内部不会排序，使用方法和set类似
* std::unordered_map：基于hash表实现的map，内部不会排序，使用方法和set类似  
* std::array：数组，在越界访问时抛出异常，建议使用std::array替代普通的数组  
* std::tuple：元组类型，类似pair，但比pair扩展性好

### 新增算法
* all_of：检测表达式是否对范围[first, last)中所有元素都返回true，如果都满足，则返回true
* any_of：检测表达式是否对范围[first, last)中至少一个元素返回true，如果满足，则返回true，否则返回false，用法和上面一样  
* none_of：检测表达式是否对范围[first, last)中所有元素都不返回true，如果都不满足，则返回true，否则返回false，用法和上面一样  
* find_if_not：找到第一个不符合要求的元素迭代器，和find_if相反
* copy_if：复制满足条件的元素
* itoa：对容器内的元素按序递增
* minmax_element：返回容器内最大元素和最小元素位置
* is_sorted、is_sorted_until：返回容器内元素是否已经排好序。


### 

## 参考
[c++11新特性，所有知识点都在这了！](https://zhuanlan.zhihu.com/p/139515439)
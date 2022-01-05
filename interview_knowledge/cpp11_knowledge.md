
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

## 参考
[c++11新特性，所有知识点都在这了！](https://zhuanlan.zhihu.com/p/139515439)
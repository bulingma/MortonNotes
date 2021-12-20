<!-- TOC -->

- [C++的编程范式](#c的编程范式)
- [什么是面向对象（OOP）](#什么是面向对象oop)
- [什么是封装、继承、多态](#什么是封装继承多态)
  - [封装](#封装)
  - [继承](#继承)
  - [多态](#多态)
    - [静态多态（编译期/早绑定）](#静态多态编译期早绑定)
    - [动态多态（运行期期/晚绑定）](#动态多态运行期期晚绑定)
- [虚析构函数](#虚析构函数)
- [纯虚函数](#纯虚函数)
- [虚函数、纯虚函数](#虚函数纯虚函数)
- [虚函数指针、虚函数表](#虚函数指针虚函数表)
- [虚继承](#虚继承)
- [虚继承、虚函数](#虚继承虚函数)
- [抽象类、接口类、聚合类](#抽象类接口类聚合类)
- [内存分配和管理](#内存分配和管理)
  - [malloc、calloc、realloc、alloca](#malloccallocreallocalloca)
  - [malloc、free](#mallocfree)
  - [new、delete](#newdelete)
  - [定位 new](#定位-new)
  - [delete this 合法吗？](#delete-this-合法吗)
- [如何定义一个只能在堆上（栈上）生成对象的类？](#如何定义一个只能在堆上栈上生成对象的类)
- [强制类型转换运算符](#强制类型转换运算符)
  - [static_cast](#static_cast)
  - [dynamic_cast](#dynamic_cast)
  - [const_cast](#const_cast)
  - [reinterpret_cast](#reinterpret_cast)
  - [bad_cast](#bad_cast)
- [运行时类型信息 (RTTI)](#运行时类型信息-rtti)
- [C++ 程序的生命周期—编码、预处理、编译、链接、运行](#c-程序的生命周期编码预处理编译链接运行)
  - [Coding](#coding)
  - [Pre-processing](#pre-processing)
  - [Compiling](#compiling)
    - [编译阶段](#编译阶段)
    - [汇编阶段](#汇编阶段)
  - [Link](#link)
  - [Running](#running)
- [常见的STL容器、算法](#常见的stl容器算法)
- [const的作用](#const的作用)
  - [1. const修饰基本数据类型，表示变量值不能改变](#1-const修饰基本数据类型表示变量值不能改变)
  - [2、const初始化引用时的例外(非常规做法，忽略)](#2const初始化引用时的例外非常规做法忽略)
  - [3、顶层const和底层const](#3顶层const和底层const)
  - [4、const与函数](#4const与函数)
  - [5、const与类](#5const与类)
- [static的作用](#static的作用)
- [堆和栈的区别以及生命周期](#堆和栈的区别以及生命周期)
- [指针和引用的区别？](#指针和引用的区别)
- [diffrence between new and malloc](#diffrence-between-new-and-malloc)
- [C++内存划分类型](#c内存划分类型)
- [指针函数和函数指针](#指针函数和函数指针)
  - [pointer to function](#pointer-to-function)
  - [function to pointer](#function-to-pointer)
- [指针常量和常量指针](#指针常量和常量指针)
  - [pointer to const](#pointer-to-const)
  - [const pointer](#const-pointer)
- [指针数组和数组指针](#指针数组和数组指针)
  - [pointer to an array](#pointer-to-an-array)
  - [array of pointers](#array-of-pointers)
- [宏定义](#宏定义)
- [c++堆栈生长方向](#c堆栈生长方向)
- [关键字static的作用](#关键字static的作用)
- [this指针](#this指针)
- [inline 内联函数](#inline-内联函数)
  - [虚函数（virtual）可以是内联函数（inline）吗？](#虚函数virtual可以是内联函数inline吗)
- [volatile](#volatile)
- [assert()](#assert)
- [sizeof()](#sizeof)
- [\#pragma pack(n)](#pragma-packn)
- [位域](#位域)
- [extern "C"](#extern-c)
- [struct 和 typedef struct](#struct-和-typedef-struct)
  - [C 中 struct 和 typedef struct](#c-中-struct-和-typedef-struct)
  - [C++ 中 struct 和 typedef struct](#c-中-struct-和-typedef-struct-1)
- [C++ 中 struct 和 class](#c-中-struct-和-class)
- [union 联合](#union-联合)
- [C 实现 C++ 类（待实现)](#c-实现-c-类待实现)
- [explicit（显式）关键字](#explicit显式关键字)
- [friend 友元类和友元函数(todo:待补充)](#friend-友元类和友元函数todo待补充)
- [`::` 范围解析运算符](#-范围解析运算符)
- [enum 枚举类型](#enum-枚举类型)
- [decltype](#decltype)
- [引用（左右引用 ）](#引用左右引用-)
- [左值右值](#左值右值)
- [成员初始化列表](#成员初始化列表)
- [initializer_list 列表初始化](#initializer_list-列表初始化)

<!-- /TOC -->

### C++的编程范式

**“编程范式”是一种“方法论”，就是指导你编写代码的一些思路、规则、习惯、定式和常用语。**

C++是在C语言的基础上开发的一种面向对象编程语言，应用广泛。C++支持多种编程范式：

- 1、面向过程（ADT+PP）

ADT:abstract data type; 抽象数据类型
PP:procedure programming; 面向过程的编程范式
ADT+PP 就是说面向过程的编程范式+抽象数据类型，你可以理解为c++的前身：带类的C。

 面向过程体现在 C++ 中，就是源自它的前身——C 语言的那部分，比如变量声明、表达式、分支 / 循环 / 跳转语句，等等。

- 2、面向对象（object-oriented programming—OOP）

OOP 这种编程风格起源于 Simula，<u>它依赖于封装（encapsulation）、继承（inheritance）以及多态（polymorphism）。</u> 就C++而言，OOP 的意思是利用类层级（classhierarchies）及虚函数进行编程，从而可以通过精制的接口操作各种类型的对象，并且程序本身也可以通过派生（derivation）进行功能增量扩展。

请注意，OOP并非万能药。不要简单地把“OOP”等同于“好”。如果你的问题的基本要素中没有与生俱来的层级关系，那么类层级和虚函数对你的代码不会有任何帮助。**OOP 的优势在于类层级可以有效地表达很多问题；OOP的主要弱点在于太多人设法强行用层级模式解决问题。**并非所有问题都应该面向对象。也可以考虑使用**[普通类](http://stdcpp.cn/html/24/25/0612/179.htm)**（plain class）、**[泛型编程](http://stdcpp.cn/html/24/25/0701/195.htm)** 和独立的函数（就像数学、C，以及 Fortran 中那样）作为解决问题的方案。

面向对象是 C++ 里一个基本的编程范式。**它的核心思想是“抽象”和“封装”**，倡导的是把任务分解成一些**高内聚低耦合**的对象，这些对象互相通信协作来完成任务。它**强调对象之间的关系和接口，而不是完成任务的具体步骤。**

 在 C++ 里，面向对象范式包括 **class**、**public**、**private**、**virtual**、**this** 等类相关的关键字，还有**构造函数**、**析构函数**、**友元函数**等概念。

- 3、泛型编程（Generic Programming—GP）

**泛型编程（Generic Programming,GP）是一种基于参数化（parameterization）的编程技巧**：可以使用类型参数化另一种类型（例如，vector的元素类型就是通过参数确定的）；算法也可以参数化另一种算法（例如，使用比较函数参数化排序函数）。GP的目的是将有用的算法或者数据结构尽可能地一般化，并使其最优化。

GP 在某些方面比 OOP要灵活得多。特别是，它不依赖于层级。例如，int 和 string 之间没有任何层级关系。总的来说，GP 的结构化程度更甚于OOP。事实上，GP 常被称为“参数多态（parametric polymorphism）”；而 OOP 常被称为“ad hoc 多态”。就C++ 而言，GP 于编译时就解析了所有名称；它不需要任何动态（运行时）调度。因此，GP 在对运行时效率要求很高的领域占据了主导地位。

泛型编程是自 STL（标准模板库）纳入到 C++ 标准以后才逐渐流行起来的新范式，**核心思想是“一切皆为类型”，或者说是“参数化类型” “类型擦除”，使用模板而不是继承的方式来复用代码，所以运行效率更高，代码也更简洁。**

在 C++ 里，泛型的基础就是 **template** 关键字，然后是庞大而复杂的标准库，里面有各种泛型容器和算法，比如 **vector**、**map**、**sort**等等。

- 4、模版元编程

这个词听起来好像很新，其实也有十多年的历史了，不过相对于前三个范式来说，确实“资历浅”。**它的核心思想是“类型运算”，操作的数据是编译时可见的“类型”。**所以也比较特殊，代码只能由编译器执行，而不能被运行时的 CPU 执行。

模板元编程是一种高级、复杂的技术，C++ 语言对它的支持也比较少，更多的是以库的方式来使用，比如**type_traits、enable_if** 等。

- 5、函数式编程

它几乎和“面向过程”一样古老，但却直到近些年才走入主流编程界的视野。所谓的“函数式”并不是 C++ 里写成函数的子程序，而是数学意义上、无副作用的函数，**核心思想是“一切皆可调用”，通过一系列连续或者嵌套的函数调用实现对数据的处理。**

函数式早在 C++98 时就有少量的尝试（bind1st/bind2nd 等函数对象），但直到 C++11 引入了 Lambda 表达式，它才真正获得了可与其他范式并驾齐驱的地位。

[01 | 重新认识C++：生命周期和编程范式](https://blog.csdn.net/GaoJuno23/article/details/106315069)

[C++的编程范式](https://www.cnblogs.com/aademeng/articles/7263505.html)

### 什么是面向对象（OOP）

面向对象是一种对现实世界（需求）理解和抽象的方法、思想、范式，**通过将需求要素转化为对象、进而使用封装、继承、多态进行问题处理的一种思想。**

### 什么是封装、继承、多态

#### 封装

  封装是实现面向对象程序设计的第一步，封装就是将数据或函数等集合在一个个的单元中（我们称之为类）。

  封装的意义在于保护或者防止代码（数据）被我们无意中破坏，从而实现**高内聚**的目的。

  把客观事物封装成抽象的类，并且类可以把自己的数据和方法只让可信的类或者对象操作，对不可信的进行信息隐藏。关键字：public, protected, private。不写默认为 private。

* `public` 成员：可以被任意实体访问
* `protected` 成员：只允许被子类及本类的成员函数访问
* `private` 成员：只允许被本类的成员函数、友元类或友元函数访问


#### 继承

  继承主要实现重用代码，节省开发时间。

  子类可以继承父类的一些东西。

#### 多态

* 多态，即多种状态（形态）。简单来说，我们可以将多态定义为消息以多种形式显示的能力。
* 多态是以封装和继承为基础的。
* C++ 多态分类及实现：
    1. 重载多态（Ad-hoc Polymorphism，编译期）：函数重载、运算符重载
    2. 子类型多态（Subtype Polymorphism，运行期）：虚函数
    3. 参数多态性（Parametric Polymorphism，编译期）：类模板、函数模板
    4. 强制多态（Coercion Polymorphism，编译期/运行期）：基本类型转换、自定义类型转换

[The Four Polymorphisms in C++](https://catonmat.net/cpp-polymorphism)

##### 静态多态（编译期/早绑定）

函数重载

```cpp
class A
{
public:
    void do(int a);
    void do(int a, int b);
};
```

##### 动态多态（运行期期/晚绑定）  
* 虚函数：用 virtual 修饰成员函数，使其成为虚函数
* 动态绑定：当使用基类的引用或指针调用一个虚函数时将发生动态绑定

**注意：**

* 可以将派生类的对象赋值给基类的指针或引用，反之不可
* 普通函数（非类成员函数）不能是虚函数
* <u>静态函数（static）不能是虚函数</u>
* <u>构造函数不能是虚函数</u>（因为在调用构造函数时，虚表指针并没有在对象的内存空间中，必须要构造函数调用完成后才会形成虚表指针）
* 内联函数不能是表现多态性时的虚函数，解释见：  
[虚函数（virtual）可以是内联函数（inline）吗？](https://github.com/huihut/interview#%E8%99%9A%E5%87%BD%E6%95%B0virtual%E5%8F%AF%E4%BB%A5%E6%98%AF%E5%86%85%E8%81%94%E5%87%BD%E6%95%B0inline%E5%90%97)

动态多态使用

```cpp
class Shape                     // 形状类
{
public:
    virtual double calcArea()
    {
        ...
    }
    virtual ~Shape();
};
class Circle : public Shape     // 圆形类
{
public:
    virtual double calcArea();
    ...
};
class Rect : public Shape       // 矩形类
{
public:
    virtual double calcArea();
    ...
};
int main()
{
    Shape * shape1 = new Circle(4.0);
    Shape * shape2 = new Rect(5.0, 6.0);
    shape1->calcArea();         // 调用圆形类里面的方法
    shape2->calcArea();         // 调用矩形类里面的方法
    delete shape1;
    shape1 = nullptr;
    delete shape2;
    shape2 = nullptr;
    return 0;
}
```

### 虚析构函数

<u>**虚析构函数是为了解决基类的指针指向派生类对象，并用基类的指针删除派生类对象。**</u>

虚析构函数使用

```cpp
class Shape
{
public:
    Shape();                    // 构造函数不能是虚函数
    virtual double calcArea();
    virtual ~Shape();           // 虚析构函数
};
class Circle : public Shape     // 圆形类
{
public:
    virtual double calcArea();
    ...
};
int main()
{
    Shape * shape1 = new Circle(4.0);
    shape1->calcArea();    
    delete shape1;  // 因为Shape有虚析构函数，所以delete释放内存时，先调用子类析构函数，再调用基类析构函数，防止内存泄漏。
    shape1 = NULL;
    return 0；
}
```


### 纯虚函数

纯虚函数是一种特殊的虚函数，它是为了实现抽象类。纯虚函数在基类中不能对虚函数给出有意义的实现，而把它声明为纯虚函数，它的实现留给该基类的派生类去做。

```cpp
virtual int A() = 0;
```

### 虚函数、纯虚函数

* 类里如果声明了虚函数，这个函数是实现的，哪怕是空实现，它的作用就是为了能让这个函数在它的子类里面可以被覆盖（override），这样的话，编译器就可以使用后期绑定来达到多态了。纯虚函数只是一个接口，是个函数的声明而已，它要留到子类里去实现。 
* 虚函数在子类里面可以不重写；但纯虚函数必须在子类实现才可以实例化子类。
* 虚函数的类用于 “实作继承”，继承接口的同时也继承了父类的实现。纯虚函数关注的是接口的统一性，实现由子类完成。 
* 带纯虚函数的类叫抽象类，这种类不能直接生成对象，而只有被继承，并重写其虚函数后，才能使用。抽象类被继承后，子类可以继续是抽象类，也可以是普通类。
* 虚基类是虚继承中的基类，具体见下文虚继承。

### 虚函数指针、虚函数表

* 虚函数指针：在含有虚函数类的对象中，指向虚函数表，在运行时确定。
* 虚函数表：在程序只读数据段（`.rodata section`，见：[目标文件存储结构](#%E7%9B%AE%E6%A0%87%E6%96%87%E4%BB%B6%E5%AD%98%E5%82%A8%E7%BB%93%E6%9E%84)），存放虚函数指针，如果派生类实现了基类的某个虚函数，则在虚表中覆盖原本基类的那个虚函数指针，在编译时根据类的声明创建。

> [C++中的虚函数(表)实现机制以及用C语言对其进行的模拟实现](https://blog.twofei.com/496/)

### 虚继承

<u>虚继承用于解决多继承条件下的菱形继承问题（浪费存储空间、存在二义性）。</u>

底层实现原理与编译器相关，一般通过**虚基类指针**和**虚基类表**实现，每个虚继承的子类都有一个虚基类指针（占用一个指针的存储空间，4字节）和虚基类表（不占用类对象的存储空间）（需要强调的是，虚基类依旧会在子类里面存在拷贝，只是仅仅最多存在一份而已，并不是不在子类里面了）；当虚继承的子类被当做父类继承时，虚基类指针也会被继承。

实际上，vbptr 指的是虚基类表指针（virtual base table pointer），该指针指向了一个虚基类表（virtual table），虚表中记录了虚基类与本类的偏移地址；通过偏移地址，这样就找到了虚基类成员，而虚继承也不用像普通多继承那样维持着公共基类（虚基类）的两份同样的拷贝，节省了存储空间。

### 虚继承、虚函数

* 相同之处：都利用了虚指针（均占用类的存储空间）和虚表（均不占用类的存储空间）
* 不同之处：
    * 虚继承
        * 虚基类依旧存在继承类中，只占用存储空间
        * 虚基类表存储的是虚基类相对直接继承类的偏移
    * 虚函数
        * 虚函数不占用存储空间
        * 虚函数表存储的是虚函数地址


### 抽象类、接口类、聚合类

* 抽象类：含有纯虚函数的类
* 接口类：仅含有纯虚函数的抽象类
* 聚合类：用户可以直接访问其成员，并且具有特殊的初始化语法形式。满足如下特点：
    * 所有成员都是 public
    * 没有定义任何构造函数
    * 没有类内初始化
    * 没有基类，也没有 virtual 函数



### 内存分配和管理

#### malloc、calloc、realloc、alloca

1. malloc：申请指定字节数的内存。<u>申请到的内存中的初始值不确定</u>。
2. calloc：为指定长度的对象，分配能容纳其指定个数的内存。<u>申请到的内存的每一位（bit）都初始化为 0</u>。
3. realloc：更改以前分配的内存长度（增加或减少）。当增加长度时，可能需将以前分配区的内容移到另一个足够大的区域，而<u>新增区域内的初始值则不确定</u>。
4. alloca：<u>在栈上申请内存。程序在出栈的时候，会自动释放内存。</u>但是需要注意的是，<u>alloca 不具可移植性, 而且在没有传统堆栈的机器上很难实现。alloca 不宜使用在必须广泛移植的程序中。</u>C99 中支持变长数组 (VLA)，可以用来替代 alloca。

#### malloc、free
用于分配、释放内存

* malloc、free 使用

申请内存，确认是否申请成功
```cpp
char *str = (char*) malloc(100);
assert(str != nullptr);
```

释放内存后指针置空
```cpp
free(p); 
p = nullptr;
```
#### new、delete

1. new / new[]：完成两件事，先底层调用 malloc 分配了内存，然后调用构造函数（创建对象）。
2. delete/delete[]：也完成两件事，先调用析构函数（清理资源），然后底层调用 free 释放空间。
3. new 在申请内存时会自动计算所需字节数，而 malloc 则需我们自己输入申请内存空间的字节数。

* new、delete 使用
申请内存，确认是否申请成功
```cpp
int main()
{
    T* t = new T();     // 先内存分配 ，再构造函数
    delete t;           // 先析构函数，再内存释放
    return 0;
}
```

#### 定位 new

定位 new（placement new）允许我们向 new 传递额外的地址参数，从而在预先指定的内存区域创建对象。

```cpp
new (place_address) type
new (place_address) type (initializers)
new (place_address) type [size]
new (place_address) type [size] { braced initializer list }
```

* `place_address` 是个指针
* `initializers` 提供一个（可能为空的）以逗号分隔的初始值列表


#### delete this 合法吗？
> [Is it legal (and moral) for a member function to say delete this?](https://isocpp.org/wiki/faq/freestore-mgmt#delete-this)

合法，但：
1. 必须保证 this 对象是通过 `new`（不是 `new[]`、不是 placement new、不是栈上、不是全局、不是其他对象成员）分配的
2. 必须保证调用 `delete this` 的成员函数是最后一个调用 this 的成员函数
3. 必须保证成员函数的 `delete this ` 后面没有调用 this 了
4. 必须保证 `delete this` 后没有人使用了


### 如何定义一个只能在堆上（栈上）生成对象的类？
> [如何定义一个只能在堆上（栈上）生成对象的类?](https://www.nowcoder.com/questionTerminal/0a584aa13f804f3ea72b442a065a7618)
* 只能在堆上
  * <u>方法：将析构函数设置为私有</u> 

    原因：C++ 是静态绑定语言，编译器管理栈上对象的生命周期，编译器在为类对象分配栈空间时，会先检查类的析构函数的访问性。若析构函数不可访问，则不能在栈上创建对象。

* 只能在栈上
  * <u>方法：将 new 和 delete 重载为私有</u>  
    
    原因：在堆上生成对象，使用 new 关键词操作，其过程分为两阶段：  
    第一阶段，使用 new 在堆上寻找可用内存，分配给对象；  
    第二阶段，调用构造函数生成对象。  
    将 new 操作设置为私有，那么第一阶段就无法完成，就不能够在堆上生成对象。

### 强制类型转换运算符

> [MSDN . 强制转换运算符](https://msdn.microsoft.com/zh-CN/library/5f6c9f8h.aspx)

#### static_cast

* <u>用于非多态类型的转换</u>
* 不执行运行时类型检查（转换安全性不如 dynamic_cast）
* 通常用于转换数值数据类型（如 float -> int）
* 可以在整个类层次结构中移动指针，子类转化为父类安全（向上转换），父类转化为子类不安全（因为子类可能有不在父类的字段或方法）

> 向上转换是一种隐式转换。

#### dynamic_cast

* <u>用于多态类型的转换</u>
* 执行运行时类型检查
* 只适用于指针或引用
* 对不明确的指针的转换将失败（返回 nullptr），但不引发异常
* 可以在整个类层次结构中移动指针，包括向上转换、向下转换

#### const_cast 

* 用于删除 const、volatile 和 __unaligned 特性（如将 const int 类型转换为 int 类型 ）

#### reinterpret_cast

* <u>用于位的简单重新解释</u>
* 滥用 reinterpret_cast 运算符可能很容易带来风险。 除非所需转换本身是低级别的，否则应使用其他强制转换运算符之一。
* <u>允许将任何指针转换为任何其他指针类型</u>（如 `char*` 到 `int*` 或 `One_class*` 到 `Unrelated_class*` 之类的转换，但其本身并不安全）
* 也允许将任何整数类型转换为任何指针类型以及反向转换。
* reinterpret_cast 运算符不能丢掉 const、volatile 或 __unaligned 特性。 
* reinterpret_cast 的一个实际用途是在哈希函数中，即，通过让两个不同的值几乎不以相同的索引结尾的方式将值映射到索引。

#### bad_cast

* 由于强制转换为引用类型失败，dynamic_cast 运算符引发 bad_cast 异常。

bad_cast 使用

```cpp
try {  
    Circle& ref_circle = dynamic_cast<Circle&>(ref_shape);   
}  
catch (bad_cast b) {  
    cout << "Caught: " << b.what();  
} 
```

### 运行时类型信息 (RTTI) 
* dynamic_cast
  * 用于多态类型的转换

* typeid
  * typeid 运算符允许在运行时确定对象的类型
  * type\_id 返回一个 type\_info 对象的引用
  * 如果想通过基类的指针获得派生类的数据类型，基类必须带有虚函数
  * 只能获取对象的实际类型

* type_info

  * type_info 类描述编译器在程序中生成的类型信息。 此类的对象可以有效存储指向类型的名称的指针。 type_info 类还可存储适合比较两个类型是否相等或比较其排列顺序的编码值。 类型的编码规则和排列顺序是未指定的，并且可能因程序而异。
  * 头文件：`typeinfo`

* typeid、type_info 使用  

  ```cpp
  #include <iostream>
  using namespace std;

  class Flyable                       // 能飞的
  {
  public:
      virtual void takeoff() = 0;     // 起飞
      virtual void land() = 0;        // 降落
  };
  class Bird : public Flyable         // 鸟
  {
  public:
      void foraging() {...}           // 觅食
      virtual void takeoff() {...}
      virtual void land() {...}
      virtual ~Bird(){}
  };
  class Plane : public Flyable        // 飞机
  {
  public:
      void carry() {...}              // 运输
      virtual void takeoff() {...}
      virtual void land() {...}
  };

  class type_info
  {
  public:
      const char* name() const;
      bool operator == (const type_info & rhs) const;
      bool operator != (const type_info & rhs) const;
      int before(const type_info & rhs) const;
      virtual ~type_info();
  private:
      ...
  };

  void doSomething(Flyable *obj)                 // 做些事情
  {
      obj->takeoff();

      cout << typeid(*obj).name() << endl;        // 输出传入对象类型（"class Bird" or "class Plane"）

      if(typeid(*obj) == typeid(Bird))            // 判断对象类型
      {
          Bird *bird = dynamic_cast<Bird *>(obj); // 对象转化
          bird->foraging();
      }

      obj->land();
  }

  int main(){
    Bird *b = new Bird();
    doSomething(b);
    delete b;
    b = nullptr;
    return 0;
  }
  ```

### C++ 程序的生命周期—编码、预处理、编译、链接、运行

C++ 程序的生命周期是指整个C++程序**从编写到运行过程，除编码外都是由编译器完成。**

**C++ 程序的生命周期包括编码、预处理、编译、运行四个阶段**，它们都有各自的特点；
虽然我们只写了一个 C++ 程序，但里面的代码可能会运行在不同的阶段，分别由预处理器、编译器和 CPU 执行；

#### Coding
编码阶段是C++程序生命周期的起点，也是最重要的阶段，是后续阶段的基础，直接决定了C++程序的“生存质量”。
最基本的编码要求是遵循语言规范和设计文档，再高级一点的话，还有代码规范、注释规范、设计模式、编程惯用法，等等。

#### Pre-processing
所谓的预处理，其实是相对于下一个阶段“编译”而言的，在编译之前，预先处理一下源代码，既有点像是编码，又有点像是编译，是一个中间阶段。
预处理是C/C++程序独有的阶段，其他编程语言都没有，这也算是C/C++语言的一个特色了。  
在这个阶段，发挥作用的是预处理器（Pre-processor）。<u>它的输入是编码阶段产生的源码文件，输出是经过“预处理”的源码文件。</u>“预处理”的目的是文字替换，用到的就是我们熟悉的各种预处理指令，比如#include、#define、#if等，实现“预处理编程”。
<u>预处理阶段主要处理#include和#define，它把#include包含进来的.h 文件插入到#include所在的位置，把源程序中使用到的用#define定义的宏用实际的字符串代替</u>
* 常用的预处理命令如下
```cpp
#define              定义一个预处理宏
#undef               取消宏的定义

#if                  编译预处理中的条件命令, 相当于C语法中的if语句
#ifdef               判断某个宏是否被定义, 若已定义, 执行随后的语句
#ifndef              与#ifdef相反, 判断某个宏是否未被定义
#elif                若#if, #ifdef, #ifndef或前面的#elif条件不满足, 则执行#elif之后的语句, 相当于C语法中的else-if
#else                与#if, #ifdef, #ifndef对应, 若这些条件不满足, 则执行#else之后的语句, 相当于C语法中的else
#endif               #if, #ifdef, #ifndef这些条件命令的结束标志.
defined              与#if, #elif配合使用, 判断某个宏是否被定义

#include             包含文件命令
#include_next        与#include相似, 但它有着特殊的用途

#line                标志该语句所在的行号
#                    将宏参数替代为以参数值为内容的字符窜常量
##                   将两个相邻的标记(token)连接为一个单独的标记
#pragma              说明编译器信息

#warning             显示编译警告信息
#error               显示编译错误信息
```

[关于C/C++中的预处理](https://zhuanlan.zhihu.com/p/106545689)

#### Compiling
在编译阶段，C++程序——也就是经过预处理的源码——要经过编译器和链接器的“锤炼”，生成可以在计算机上运行的二进制机器码。这里面的讲究是最多的，也是最复杂的，C++编译器要分词、语法解析、生成目标码，并尽可能地去优化。

<u>在编译的过程中，编译器还会根据C++语言规则检查程序的语法、语义是否正确，发现错误就会产生“编译失败”。这就是最基本的C++“静态检查”。</u>

在处理源码时，由于编译器是依据C++语法检查各种类型、函数的定义，所以，在这个阶段，我们就能够以编译器为目标进行编程，有意识地控制编译器的行为。这里有个新名词，叫“模板元编程”。不过，“模板元编程”比较复杂，不太好理解，属于比较高级的用法。

编译可细分为两个阶段：
##### 编译阶段
编译阶段，Gcc首先要检查代码的规范性、是否有语法错误等，以确定代码的实际要做的工作，在检查无误后，Gcc把代码翻译成汇编语言。  
编译阶段是最重要的阶段，在这个阶段GCC首先检查语法然后把由上步生成的*.i编译成*.s文件。  
 
##### 汇编阶段 
汇编阶段把*.s文件翻译成二进制机器指令文件*.o  

#### Link
```cpp
#include<stdio.h>

int main()
{
  printf("Hello! This is our embedded world!\n");
  return 0;
}
```
在成功编译之后，就进入了链接阶段。在这里涉及到一个重要的概念：函数库。  
读者可以查看这个小程序，在这个程序中并没有定义”printf”的函数实现，且在预编译中包含进的”stdio.h”中也只有该函数的声明，而没有定义函数的实现，那么，是在哪里实现”printf”函数的呢？最后的答案是：系统把这些函数实现都被做到名为libc.so.6的库文件中去了，在没有特别指定时，Gcc会到系统默认的搜索路径”/usr/lib”下进行查找，也就是链接到libc.so.6库函数中去，这样就能实现函数”printf”了，而这也就是链接的作用。

函数库一般分为静态库和动态库两种。  
* 静态库  
  是指编译链接时，把库文件的代码全部加入到可执行文件中，因此生成的文件比较大，但在运行时也就不再需要库文件了。其后缀名一般为”.a”。
* 动态库  
  与静态库相反，在编译链接时并没有把库文件的代码加入到可执行文件中，而是在程序执行时由运行时链接文件加载库，这样可以节省系统的开销。动态库一般后缀名为”.so”。Gcc在编译时默认使用动态库。

#### Running
编译阶段之后，有了可执行文件，C++程序就可以跑起来了，进入运行阶段。这个时候，“静态的程序”被载入内存，由CPU逐条语句执行，就形成了“动态的进程”。

运行阶段也是我们最熟悉的了。在这个阶段，我们常做的是GDB调试、日志追踪、性能分析等，然后收集动态的数据、调整设计思路，再返回编码阶段，重走这个“瀑布模型”，实现“螺旋上升式”的开发。

[第265期 | 重新认识C++：生命周期和编程范式](https://zhuanlan.zhihu.com/p/138875601)
[编译器编译原理：预处理，编译，汇编，链接各步骤详解](https://zhuanlan.zhihu.com/p/39219541)

### 常见的STL容器、算法

**STL包括两部分内容：容器和算法。**（重要的还有融合这二者的迭代器）

容器，即存放数据的地方。比如array等。

在STL中，**容器分为两类：序列式容器和关联式容器。**

**序列式容器，其中的元素不一定有序，但都可以被排序。**  

如：vector、list、deque、stack、queue、heap、priority_queue、slist；

**关联式容器，内部结构基本上是一颗平衡二叉树。所谓关联，指每个元素都有一个键值和一个实值，元素按照一定的规则存放。**  

如：RB-tree、set、map、multiset、multimap、hashtable、hash_set、hash_map、hash_multiset、hash_multimap。

下面各选取一个作为说明。

vector：它是一个动态分配存储空间的容器。区别于c++中的array，array分配的空间是静态的，分配之后不能被改变，而vector会自动重分配（扩展）空间。

set：其内部元素会根据元素的键值自动被排序。区别于map，它的键值就是实值，而map可以同时拥有不同的键值和实值。

算法，如排序，复制……以及个容器特定的算法。这点不用过多介绍，主要看下面迭代器的内容。

迭代器是STL的精髓，我们这样描述它：迭代器提供了一种方法，使它能够按照顺序访问某个容器所含的各个元素，但无需暴露该容器的内部结构。它将容器和算法分开，好让这二者独立设计。

[20道必须掌握的C++面试题](https://www.w3cschool.cn/cpp/cpp-a9no2ppi.html)

### const的作用

- 必背总结  

1. 修饰变量，说明该变量不可以被改变；
2. 修饰指针，分为指向常量的指针（pointer to const）和自身是常量的指针（常量指针，const pointer）；
3. 修饰引用，指向常量的引用（reference to const），用于形参类型，即避免了拷贝，又避免了函数对值的修改；
4. 修饰成员函数，说明该成员函数内不能修改成员变量。

const 是 constant 的缩写，本意是不变的，不易改变的意思。在 C++ 中是用来**修饰内置类型变量，自定义对象，成员函数，返回值，函数参数。**

C++ const 允许指定一个语义约束，编译器会强制实施这个约束，允许程序员告诉编译器某值是保持不变的。**如果在编程中确实有某个值保持不变，就应该明确使用const，这样可以获得编译器的帮助。**

#### 1. const修饰基本数据类型，表示变量值不能改变

由于const一旦创建就不可更改，所以**const对象必须初始化**。

```cpp
const int bufSize = 512;
bufSize = 512;              // 错误：试图向const对象写值
const int i = get_size();   // 正确
const int j = 42;           // 正确
const int k;                // 错误：未初始化
```

使用**值传递初始化**时，被初始化的对象是否为const与初始化对象是否为const无关。也即，const对象与非const对象可以互为初始化。）

```cpp
int i = 0;
int j = i;       // 正确，非const初始化非const
const int k = i; // 正确，非const初始化const

const int x = 0;
int y = x;       // 正确，const初始化非const
const int z = x; // 正确，const初始化const
```

#### 2、const初始化引用时的例外(非常规做法，忽略)

#### 3、顶层const和底层const

通常在指针/引用与const符同时使用时会用到这个概念。**修饰指针本身的const称为顶层const，修饰指针所指向对象的const称为底层const**。 底层const与顶层const是两个互相独立的修饰符，互不影响。

- 3.1. const与指针

指针本身是一个独立的对象，它又可以指向另一个对象。所以指针和const同时使用时，有两种情况：

```cpp
int i = 0;
int *const j = &i; // 顶层const, 指针j指向i，const修饰指针j本身，所以j的地址值不允许修改，但可以通过j修改i的值
const int *k = &i; // 底层const, 指针k指向i，const修饰k指向的i，所以k的地址值可以修改，但不可以通过k修改i的值
```

- 3.2. const与引用

引用一旦初始化，就不能再修改（绑定），所以引用本身就具有"const"的性质。

与指针相比，引用相当于内置了顶层const。

所以使用引用时，就只需考虑是否为底层const：

```cpp
int i = 0;
const int &j = i; // j为绑定到i的const引用，不允许使用j来修改i
```

- 3.3. 其他

(1). 可以将底层const的指针（或引用）指向（或绑定）到非const对象，但不允许非底层const的指针（或引用）指向（或绑定）到const对象。 （即：const对象不允许通过任何方式（指针/引用）被修改。）

(2). 修饰值本身的const均为顶层const：

```cpp
const int i = 0; // 顶层const;
```

#### 4、const与函数

- 4.1. 值传递的const形参

```cpp
void fcn(const int i) { /* ... */ }
```

这个函数中，变量i为值传递形参，根据值传递的初始化规则，**形参i是否为const与传入的实参是否为const是完全无关的。**这里的const仅表示i在函数体中不允许修改。

因为值传递的const形参在调用上与非const形参没有区别（大概是指，无论形参是否为const，实参都不会被修改。），所以仅仅使用const无法区分参数类别，所以无法实现函数重载，如下的重载是错误的：

```cpp
void fcn1(const int i) { /* ... */ }
void fcn1(int i) { /* ... */ } // 错误：重复定义函数，不能实现重载
```

- 4.2. const指针/引用的形参

对于顶层const的指针，与上一小节一样，其const性质与实参无关，顶层const仅表示指针/引用本身在函数体中不允许修改。

所以我们只需要讨论底层const的指针/引用。

```cpp
void fcn2(const int &x) { /* ... */ } // 接受const或非const的int引用，但是不允许通过x修改传入的对象
void fcn2(const int *y) { /* ... */ } // 接受const或非const的int指针，但是不允许通过y修改传入的对象
```

如上两个函数都定义了底层const的形式参数，它们可以接受const或非const对象，但是不能在函数体内修改这些对象。所以如下的调用都是合法的：

```cpp
int i = 0;
fcn2(i);  // 正确：调用第一个函数
fcn2(&i); // 正确：调用第二个函数

const int j = 0;
fcn2(j);  // 正确：调用第一个函数
fcn2(&j); // 正确：调用第二个函数
```

由于底层const描述实参性质（不允许在调用函数内部被修改），可以在调用时区分const，**所以使用底层const的指针/引用可以实现函数重载：**

```cpp
void fcn3(int &x) { /* ... */ } 
void fcn3(const int &x) { /* ... */ } // 新函数，作用于const的引用
```

所以可以分别调用两个函数：

```cpp
int i = 0;
fcn3(i); // 正确：调用第一个函数

const int j = 0;
fcn3(j); // 正确：调用第二个函数
```

注意，当传递非常量对象时，编译器会优先调用非常量版本的函数。

- 4.3. 总结

顶层const的形式参数不能实现函数重载，但底层const形参可以

当函数不修改参数值时，尽可能将形式参数定义为（底层）const参数。一方面，（底层）const参数可以保护参数对象；另一方面，因为（底层）const参数可以接受常量与非常量对象，但非（底层）const参数只能接受非常量对象。

#### 5、const与类

- 5.1. const与类的成员变量

​ 一个类通常包含成员函数和成员变量。

1. 类的对象的const修饰表示该对象的成员变量不允许被修改。
2. 无论类的成员变量本身是否为const，只要对象声明为const，成员变量就不允许被修改。

```cpp
class Number
{
  public:
    int number = 0;
};

int main()
{
    const Number n;
    n.number = 1; // 错误，n为const对象，不允许被修改
    return 0;
}
```

- 5.2. const与类的成员函数

  当对象被声明为const时，该对象不能调用非const函数，因为非const函数可能修改成员变量。

  ```cpp
  class Number
  {
  public:
      void set(int num) { number = num; }
      int get() { return number; }
  
      int number = 0;
  };
  
  int main()
  {
      const Number n;
      n.set(1); // 错误，n为const对象，不能调用非const函数
      cout << n.get() << endl; // 错误，原因同上
      return 0;
  }
  ```

  1. 将成员函数声明为const函数，则可以被const对象调用，声明const函数的方法为在其参数列表后添加const关键字。
  2. const成员函数中不允许修改成员变量。也即，并非所有成员函数都可以被声明为const函数，C++会在编译时检查被声明为const的函数是否修改了成员变量，若是，则报错，编译不通过。

  ```cpp
  class Number
  {
  public:
      void set(int num) const { number = num; } // 错误：const函数不允许修改成员变量
      int get() const { return number; } // 正确：没有修改成员变量，可被声明为const函数
  
      int number = 0;
  };
  
  int main()
  {
      const Number n;
      n.set(1);                // 错误，const函数不允许修改成员变量
      cout << n.get() << endl; // 正确，const对象可以调用const函数
      return 0;
  }
  ```

  与底层const形参一样，const成员函数也可以实现重载。同样，当非常量对象调用函数时，编译器会优先调用非常量版本的函数。

  ```cpp
  class T
  {
  public:
      int fcn() { return 1; }
      int fcn() const { return 2; } // 正确：定义了可以重载的新函数
  };
  
  int main()
  {
      T t1;
      cout << t1.fcn() << endl; // 调用第一个函数，输出"1"
  
      const T t2;
      cout << t2.fcn() << endl; // 调用第二个函数，输出"2"
      return 0;
  }
  ```

- 5.3. 总结
  - 当函数不修改成员变量时，尽可能将函数声明为const函数，因为const函数可以被非const对象和const对象调用，而非const函数只能被非const对象调用。
  - const函数并不意味着数据安全，虽然不能通过const函数修改成员变量，但是这样的const仅为顶层const（即成员变量本身不能被修改），若成员变量包含非底层const的指针/引用，虽然成员变量本身不能被修改，但依然可以通过这些指针/引用修改其指向/绑定的对象。

- 5.4. const成员函数实现机制（详见文章）

[C++中的 const 关键字](https://zhuanlan.zhihu.com/p/37514756)

### static的作用

### 堆和栈的区别以及生命周期

- 一、堆栈空间分配区别：

1、栈（操作系统）：由操作系统自动分配释放 ，存放函数的参数值，局部变量的值等。其操作方式类似于数据结构中的栈；

2、堆（操作系统）： 一般由程序员分配释放， 若程序员不释放，程序结束时可能由OS回收，分配方式倒是类似于链表。

- 二、堆栈缓存方式区别：

1、栈使用的是一级缓存， 他们通常都是被调用时处于存储空间中，调用完毕立即释放；

2、堆是存放在二级缓存中，生命周期由虚拟机的垃圾回收算法来决定（并不是一旦成为孤儿对象就能被回收）。所以调用这些对象的速度要相对来得低一些。

- 三、堆栈数据结构区别：

堆（数据结构）：堆可以被看成是一棵树（最大堆、最小堆），如：堆排序；

栈（数据结构）：一种先进后出的数据结构（本质是list）。

[20道必须掌握的C++面试题](https://www.w3cschool.cn/cpp/cpp-a9no2ppi.html)

### 指针和引用的区别？

1. 指针是一个变量，只不过这个变量存储的是一个地址，指向内存的一个存储单元；而引用仅是个别名；
2. 引用使用时无需解引用(*)，指针需要解引用；
3. 引用只能在定义时被初始化一次，之后不可变；指针可变；
5. 引用不能为空，指针可以为空；
6. “sizeof 引用”得到的是所指向的变量(对象)的大小，而“sizeof 指针”得到的是指针本身的大小；
7. 指针和引用的自增(++)运算意义不一样；
8. 指针可以有多级，但是引用只能是一级（int **p；合法 而 int &&a是不合法的）
9. 从内存分配上看：程序为指针变量分配内存区域，而引用不需要分配内存区域。

### diffrence between new and malloc

1、<u>malloc与free是C++/C语言的标准库函数，new/delete是C++的运算符。</u>它们都可用于申请动态内存和释放内存。

2、对于非内部数据类型的对象而言，光用malloc/free无法满足动态对象的要求。对象在创建的同时要自动执行构造函数，对象在消亡之前要自动执行析构函数。

3、由于malloc/free是库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加于malloc/free。因此C++语言需要一个能完成动态内存分配和初始化工作的运算符new与一个能完成清理与释放内存工作的运算符delete。注意new/delete不是库函数。

4、C++程序经常要调用C函数，而C程序只能用malloc/free管理动态内存。

5、new可以认为是malloc加构造函数的执行。new出来的指针是直接带类型信息的。而malloc返回的都是void指针。

[20道必须掌握的C++面试题](https://www.w3cschool.cn/cpp/cpp-a9no2ppi.html)

### C++内存划分类型

- 1. 栈  
  函数的参数和局部变量是分配在栈上（但不包括static声明的变量）。在函数被调用时，栈用来传递参数和返回值。由于栈的后进先出特点，所以栈特别方便用来保存/恢复调用现场。

- 2. 堆  
  用于存放进程运行中被动态分配的内存段，它的大小并不固定，可动态扩张或缩减。当进程调用malloc/free等函数分配内存时，新分配的内存就被动态添加到堆上（堆被扩张）/释放的内存从堆中被剔除（堆被缩减）

- 3. 全局变量区
  - 3.1. data段——存放已初始化的全局变量
  - 3.2. bss段——存放未初始化的全局变量  
  
- 4. 程序代码区(text段)  
  存放已编译程序的机器代码。

- 5. 只读数据区（rodata段）
  ```cpp
  例如，全局作用域中的 const int ival = 10，ival存放在.rodata段。  
  再如，函数局部作用域中的printf("Hello world %d\n", c);
  语句中的格式字符串"Hello world %d\n"，也存放在rodata段。
  ```
  
[C++内存划分类型？](https://www.zhihu.com/question/26224882)


### 指针函数和函数指针

#### pointer to function
<u>A pointer to function points to a function.</u>
<u>函数指针是一个指针，指向的是一个函数的地址。</u>

* 声明格式  
  类型说明符 (*函数名)(参数)
  ```cpp
  int (*fun)(int x, int y);
  ```

  函数指针是需要把一个函数的地址赋给它，有两种写法：
  ```cpp
  fun = &Function;
  fun = Function;
  ```
  取址运算符&不是必须的，因为一个函数标识符(function name)就表示了它的地址，如果是函数调用，还必须包含一个圆括号包起来的参数表。
  调用函数指针的方式也有两种：
  ```cpp
  x = (*fun)();
  x = fun();
  ```
  两种方式均可，其中第二种看上去和普通函数调用一模一样。这里推荐使用第一种调用方式，因为可以清楚的表示这是通过指针的方式来调用函数。

* 代码示例  
  ```cpp
  int add(int x, int y)
  {
      return x + y;
  }

  int sub(int x, int y)
  {
      return x - y;
  }

  //函数指针
  int (*fun)(int x, int y);

  int main(int argc, char *argv[])
  {
      //第一种写法
      fun = add;
      std::cout << "(*fun)(1,2) = " << (*fun)(1,2) << std::endl;
      
      //第二种写法
      fun = &sub;
      std::cout << "fun(1,2) = " << fun(1,2) << std::endl;

      return 0;
  }

  输出如下：
  (*fun)(1,2) = 3
  fun(5,3) = 2 
  ```

#### function to pointer
<u>A function to pointer is one which returns a pointer.</u>
<u>指针函数是一个函数，是返回值为指针的函数。</u>
```cpp
//一个普通的函数声明：
int fun(int x, int y);

//一个指针函数的声明：
int* fun(int x, int y);

这和上面那个普通函数声明的唯一区别就是在函数名前面多了一个*号，  
而这个函数就是一个指针函数。其返回值是一个int类型的指针，是一个地址。


//代码示例：
typedef struct{
     int a;
     int b;
}Data;

//指针函数
Data* f(int a, int b)
{
     Data *data = new Data;
     data->a = a;
     data->b = b;
     return data; 
}

int main(int argc, const char *argv[])
{
     Data *myData = f(4, 5);
     std::cout << "f(4,5)= " << myData->a << myData->b;
     return 0;
}
```
[C语言--函数指针与指针函数](https://zhuanlan.zhihu.com/p/53182241)




### 指针常量和常量指针

<u>**const分为顶层和底层，顶层const指对象本身就是一个常量，底层const指对象(指针、引用等)指向的是一个常量。 由此观之，对象本身是不是常量和对象指向的是不是常量是两个相互独立的问题，**</u>当这个问题与指针相结合的时候就出现了我们熟知的指针常量和常量指针。
#### pointer to const
指针常量（pointer to const），落点在常量上，这说明了指针常量是一个常量，类型是指针类型。  

```cpp
num = 10;
int *const p = &num;

从右向左看,离p最近的是const,首先说明了p是一个常量。  
在这里，const是一个顶层const，因为它本身是一个常量。  
我们初始化的时候将p指向num，完成了初始化，所以常量p不能在指向其他的变量(或是常量)。

p = &num2;  //错误，指针常量不可以修改指向的对象
*p = 20;    //正确，指针常量可以修改被指向对象的值 
```

#### const pointer
常量指针（const pointer），落点是指针，说明常量指针是一个指针,不同的是它指向的是一个常量。

```cpp
const int a = 5;
const int * p = &a;

从右向左看首先是一个*，首先说明p是一个指针，再向左看，是一个指向常量的指针，
所以p指向的是一个常量，我们就不能更改*p的值，但是p本身不是一个常量所以我们可以改变p指向的变量。

const int b = 10;
p = &b;     //正确，常量指针可以修改指向的对象
*p = 20;    //错误，常量指针不可以修改被指向对象的值 

int const *p = &a;
需要注意的是这还是一个常量指针，从右向左看与上面的结果是一致的。
```

[一文彻底理解:常量指针和指针常量](https://zhuanlan.zhihu.com/p/111193181)

### 指针数组和数组指针  

前言：操作符的优先级：() > [] > *

- 说明
指针数组：它是一个数组，数组的元素都是指针。它是<u>**“储存指针的数组”**</u>的简称。  
数组指针：它是一个指针，它指向一个数组。它是<u>**“指向数组的指针”**</u>的简称。

#### pointer to an array

- 数组指针(行指针) 定义  

```cpp
int (*p)[n];
```

()优先级高，首先说明p是一个指针，指向一个整型的一维数组，<u>**这个一维数组的长度是n，也可以说是p的步长。也就是说执行p+1时，p要跨过n个整型数据的长度。**</u>

- 数组指针的应用  

```cpp
如要将二维数组赋给一指针，应这样赋值：
int a[3][4];
int (*p)[4];  //该语句是定义一个数组指针，指向含4个元素的一维数组。
p = a;        //将该二维数组的首地址赋给p，也就是a[0]或&a[0][0]
p++;          //该语句执行过后，也就是p=p+1; p跨过行a[0][]指向了行a[1][]
所以数组指针也称指向一维数组的指针，亦称行指针。
```

#### array of pointers

- 指针数组定义

```cpp
int *p[n];
```

[\]优先级高，先与p结合成为一个数组，再由int\*说明这是一个整型指针数组，它有n个指针类型的数组元素。这里执行p+1时，则p指向下一个数组元素。  
这样赋值是错误的：p = a；因为p是个不可知的表示，只存在p[0]、p[1]、p[2]...p[n-1],而且它们分别是指针变量可以用来存放变量地址。  
但可以这样 \*p = a; 这里\*p表示指针数组第一个元素的值，a的首地址的值。  

- 指针数组的应用
如要将二维数组赋给一指针数组:

```cpp
int *p[3];
int a[3][4] = {...};
for(i=0; i<3; i++)
  p[i] = a[i];
这里int *p[3] 表示一个一维数组内存放着三个指针变量，分别是p[0]、p[1]、p[2]
所以要分别赋值。
```

如要将一维数组赋给一指针数组:

```cpp
int a［3］=｛1，2，3｝；
int *p［3］；
int i=0；
for（i=0；i＜3；i++）
  p［i］=&a[i］;
```

![point_of_vector_OR_vector_point](../../z_images/design_partern/../../MortonNotes/z_images/point_of_vector_OR_vector_point.jpeg)  
[数组指针和指针数组的区别](https://zhuanlan.zhihu.com/p/55724052)  
[指针数组如何赋值？](https://www.zhihu.com/question/52738167)

### 宏定义

说明：宏定义展开是在预编译期进行的，宏的本质是代码替换（宏不是函数）。  

- a，b最大值宏定义

```cpp
#define max(x, y) ({    \
    typeof(x) _x = (x); \
    typeof(y) _y = (y); \
    (void) (&_x == &_y);\
    _x > _y ? _x : _y; })
```

关于以上宏定义的说明：  

1. 使用了typeof关键字用来获取宏的两个参数类型，从而自动推断用户传入的x，y的类型；  
2. 定义临时变量_x,_y的目的，防止用户这样调用： max(a++, b++);
3. (void) (&x == &y); 这句话，简直是天才般的设计！一是用来给用户提示一个警告，对于不同类型的指针比较，编译器会给一个警告，提示两种数据类型不同；二是，当两个值比较，比较的结果没有用到，有些编译器可能会给出一个warning，加个(void)后，就可以消除这个警告！  

- a + b 宏定义

```cpp
入门级：
#define TWO_SUM(a, b) ((a) + (b))

高手级：
#define TWO_SUM(a, b)({
  typeof(a) _a = (a);\
  typeof(b) _b = (b);\
  (void *) (&_a == &_b);\
  _a + _b;})
```

- 交换a和b宏定义

```cpp
//算数技巧
#define SWAP(a, b) {a = b - a; b = b - a; a = a + b}

//位运算方式
#define SWAP(a,b) {a=a^b; b=b^a; a=a^b;}

//引入临时变量
#define SWAP(a,b,temp) {temp=a; a=b; b=temp;}
```

### c++堆栈生长方向

《程序员的自我修养》说栈的地址比堆高，栈是向下增长的，堆是向上增长的。
这个说法不够严谨。  
进程地址空间的分布取决于操作系统，栈向什么方向增长取决于操作系统与CPU的组合

* 以简化的Linux/x86模型为例  
  在简化的32位Linux/x86进程地址空间模型里，（主线程的）栈空间确实比堆空间的地址要高——它已经占据了用户态地址空间的最高可分配的区域，并且向下（向低地址）增长。  
![32bit_x86_process_addr_space](../../z_images/design_partern/../../MortonNotes/z_images/32bit_x86_process_addr_space.jpeg)  

* 结论    
1. 堆没有方向之说，每个堆都是散落的；
2. 堆和栈之间没有谁地址高之说，看操作系统实现；
3. 数组取下标偏移总是往上涨的，和在堆还是栈没啥关系；
4. 进程地址空间的分布取决于操作系统，栈向什么方向增长取决于操作系统与CPU的组合。

[堆、栈的地址高低？ 栈的增长方向？](https://www.zhihu.com/question/36103513)

### 关键字static的作用

1. 修饰普通变量  
   <u>**修改变量的存储区域和生命周期，使变量存储在静态区，在main函数运行前就分配了空间**</u>，如果有初始值就用初始值初始化它，如果没有初始值系统用默认值初始化它。  

2. 修饰普通函数  
   表明函数的作用范围，仅在定义该函数的文件内才能使用。在多人开发项目时，为了防止与他人命名空间里的函数重名，可以将函数定位为 static。

3. 修饰成员变量  
   修饰成员变量使所有的对象只保存一个该变量，而且不需要生成对象就可以访问该成员。

4. 修饰成员函数  
   修饰成员函数使得不需要生成对象就可以访问该函数，但是在static函数内不能访问非静态成员。


### this指针

1. this 指针是一个隐含于每一个非静态成员函数中的特殊指针。它指向调用该成员函数的那个对象。
2. 当对一个对象调用成员函数时，编译程序先将对象的地址赋给 this 指针，然后调用成员函数，每次成员函数存取数据成员时，都隐式使用 this 指针。  
3. 当一个成员函数被调用时，自动向它传递一个隐含的参数，该参数是一个指向这个成员函数所在的对象的指针。  
4. this 指针被隐含地声明为: ClassName \*const this(常量指针)，这意味着不能给 this 指针赋值；  
在 ClassName 类的 const 成员函数中，this 指针的类型为：const ClassName* const(指针常量)，这说明 this 指针所指向的这种对象是不可修改的（即不能对这种对象的数据成员进行赋值操作）；
5. this 并不是一个常规变量，而是个右值，所以不能取得 this 的地址（不能 &this）。
6. 在以下场景中，经常需要显式引用 this 指针：  
- 1. 为实现对象的链式引用；
- 2. 为避免对同一对象进行赋值操作；
- 3. 在实现一些数据结构时，如 list。  

### inline 内联函数

* inline特征  
1. 相当于把内联函数里面的内容写在调用内联函数处；  
2. <u>相当于不用执行进入函数的步骤，直接执行函数体；</u>  
3. 相当于宏，却比宏多了类型检查，真正具有函数特性；  
4. 编译器一般不内联包含循环、递归、switch 等复杂操作的内联函数；  
5. <u>在类声明中定义的函数，除了虚函数的其他函数都会自动隐式地当成内联函数。</u>  

* inline使用  

```cpp
// 声明1（加 inline，建议使用）
inline int functionName(int first, int second,...);

// 声明2（不加 inline）
int functionName(int first, int second,...);

// 定义
inline int functionName(int first, int second,...) {/****/};

// 类内定义，隐式内联
class A {
    int doA() { return 0; }         // 隐式内联
}

// 类外定义，需要显式内联
class A {
    int doA();
}
inline int A::doA() { return 0; }   // 需要显式内联
```

* 编译器对 inline 函数的处理步骤  

1. 将 inline 函数体复制到 inline 函数调用点处；
2. 为所用 inline 函数中的局部变量分配内存空间；
3. 将 inline 函数的的输入参数和返回值映射到调用方法的局部变量空间中；
4. 如果 inline 函数有多个返回点，将其转变为inline函数代码块末尾的分支（使用 GOTO）。

* inline 优缺点
  - 优点:
    1. 内联函数同宏函数一样将在被调用处进行代码展开，省去了参数压栈、栈帧开辟与回收，结果返回等，从而提高程序运行速度。
    2. 内联函数相比宏函数来说，在代码展开时，<u>会做安全检查或自动类型转换（同普通函数）</u>，而宏定义则不会。
    3. 在类中声明同时定义的成员函数，自动转化为内联函数，因此<u>内联函数可以访问类的成员变量，宏定义则不能</u>。
    4. 内联函数在运行时可调试，而宏定义不可以。

  - 缺点:
    1. 代码膨胀。内联是以代码膨胀（复制）为代价，消除函数调用带来的开销。如果执行函数体内代码的时间，相比于函数调用的开销较大，那么效率的收获会很少。另一方面，每一处内联函数的调用都要复制代码，<u>将使程序的总代码量增大</u>，消耗更多的内存空间。
    2. <u>inline 函数无法随着函数库升级而升级</u>。inline函数的改变需要重新编译，不像 non-inline 可以直接链接。
    3. <u>是否内联，程序员不可控。内联函数只是对编译器的建议，是否对函数内联，决定权在于编译器。</u>  

#### 虚函数（virtual）可以是内联函数（inline）吗？

> [Are "inline virtual" member functions ever actually "inlined"?](http://www.cs.technion.ac.il/users/yechiel/c++-faq/inline-virtuals.html)

- 虚函数可以是内联函数，内联是可以修饰虚函数的，但是当虚函数表现多态性的时候不能内联。
- 内联是在编译期建议编译器内联，而虚函数的多态性在运行期，编译器无法知道运行期调用哪个代码，因此虚函数表现为多态性时（运行期）不可以内联。
- `inline virtual` 唯一可以内联的时候是：编译器知道所调用的对象是哪个类（如 `Base::who()`），这只有在编译器具有实际对象而不是对象的指针或引用时才会发生。

虚函数内联使用

```cpp
#include <iostream>  
using namespace std;
class Base
{
public:
	inline virtual void who()
	{
		cout << "I am Base\n";
	}
	virtual ~Base() {}
};
class Derived : public Base
{
public:
	inline void who()  // 不写inline时隐式内联
	{
		cout << "I am Derived\n";
	}
};

int main()
{
	// 此处的虚函数 who()，是通过类（Base）的具体对象（b）来调用的，编译期间就能确定了，所以它可以是内联的，但最终是否内联取决于编译器。 
	Base b;
	b.who();

	// 此处的虚函数是通过指针调用的，呈现多态性，需要在运行时期间才能确定，所以不能为内联。  
	Base *ptr = new Derived();
	ptr->who();

	// 因为Base有虚析构函数（virtual ~Base() {}），所以 delete 时，会先调用派生类（Derived）析构函数，再调用基类（Base）析构函数，防止内存泄漏。
	delete ptr;
	ptr = nullptr;

	system("pause");
	return 0;
} 
```


### volatile

```cpp
volatile int i = 10; 
```

* volatile 关键字是一种类型修饰符，用它声明的类型变量表示可以被某些编译器未知的因素（操作系统、硬件、其它线程等）更改。所以使用 volatile 告诉编译器不应对这样的对象进行优化;
* volatile 关键字声明的变量，<u>每次访问时都必须从内存中取出值（没有被 volatile 修饰的变量，可能由于编译器的优化，从 CPU 寄存器中取值）;</u>
* const 可以是 volatile （如只读的状态寄存器）;
* 指针可以是 volatile;


### assert()

断言，是宏，而非函数。assert 宏的原型定义在 `<assert.h>`（C）、`<cassert>`（C++）中，其作用是如果它的条件返回错误，则终止程序执行。可以通过定义 `NDEBUG` 来关闭 assert，但是需要在源代码的开头，`include <assert.h>` 之前。

assert() 使用

```cpp
#define NDEBUG          // 加上这行，则 assert 不可用
#include <assert.h>

assert( p != NULL );    // assert 不可用
```

### sizeof()
* sizeof的概念  
sizeof是C语言的一种单目操作符（它并不是函数），如C语言的其他操作符++、--等。  
sizeof操作符以字节形式给出了其操作数的存储大小。  
操作数可以是一个表达式或括在括号内的类型名。  
操作数的存储大小由操作数的类型决定。  

* sizeof的使用方法
  1. 用于数据类型
  ```cpp
  sizeof使用形式: sizeof(type)
  数据类型必须用括号括住: sizeof(int)
  sizeof(char) = 1;
  ```

  2. 用于变量
  ```cpp
  sizeof使用形式: sizeof(var_name) 或 sizeof var_name
  变量名可以不用括号括住.如 sizeof (var_name)，sizeof var_name等都是正确形式
  ```

* sizeof使用举例  
  
  1. sizeof 对数组，得到整个数组所占空间大小。
  ```cpp
  char a[5];
  int b[5];
  sizeof(a) = 5;
  sizeof(b) = 20;
  ```
  2. sizeof 对指针，得到指针本身所占空间大小。
  ```cpp
  char *p;      //一般Unix/Linux的指针字节数为4。
  sizeof(p) = 4;
  ```
  3. sizeof 对联合类型，sizeof是其最大字节成员的字节数。
  ```cpp
  union  u{             //对union来说
        char c;
        double d;
  }u;
  sizeof(u) = max(sizeof(c),sizeof(d)) = sizeof(1,8) = 8;
  ```
  4. sizeof 对结构类型，sizeof是其成员类型的总字节数，包括补充字节在内。
  ```cpp
  struct a{             //对struct来说
      char b;
      double x;
  }a; 
  sizeof(a) = 12;      //在Linux环境

  在Linux上: sizeof(a) = 12;
  而一般 sizeof(char) + sizeof(double) = 9;
  在此例中，所有的结构成员都要放在被4整除的地址(Linux的存放方式)，这里补3个字节，所以为12。
  ```
  [c语言详解sizeof](https://zhuanlan.zhihu.com/p/74066384)

### \#pragma pack(n)

设定结构体、联合以及类成员变量以 n 字节方式对齐

`#pragma pack(n)` 使用

```cpp
#pragma pack(push)  // 保存对齐状态
#pragma pack(4)     // 设定为 4 字节对齐

struct test
{
    char m1;
    double m4;
    int m3;
};

#pragma pack(pop)   // 恢复对齐状态
```

### 位域

```cpp
Bit mode: 2;    // mode 占 2 位
```

类可以将其（非静态）数据成员定义为位域（bit-field），在一个位域中含有一定数量的二进制位。当一个程序需要向其他程序或硬件设备传递二进制数据时，通常会用到位域。

* 位域在内存中的布局是与机器有关的
* 位域的类型必须是整型或枚举类型，带符号类型中的位域的行为将因具体实现而定
* 取地址运算符（&）不能作用于位域，任何指针都无法指向类的位域

### extern "C"

* 被 extern 限定的函数或变量是 extern 类型的
* 被 `extern "C"` 修饰的变量和函数是按照 C 语言方式编译和链接的

`extern "C"` 的作用是让 C++ 编译器将 `extern "C"` 声明的代码当作 C 语言代码处理，可以避免 C++ 因符号修饰导致代码不能和C语言库中的符号进行链接的问题。

* extern "C" 使用  

```cpp
#ifdef __cplusplus
extern "C" {
#endif

void *memset(void *, int, size_t);

#ifdef __cplusplus
}
#endif
```


### struct 和 typedef struct

#### C 中 struct 和 typedef struct

```c
// c
typedef struct Student {
    int age; 
} S;
```

等价于

```c
// c
struct Student { 
    int age; 
};

typedef struct Student S;
```

此时 `S` 等价于 `struct Student`，但两个标识符名称空间不相同。

另外还可以定义与 `struct Student` 不冲突的 `void Student() {}`。

#### C++ 中 struct 和 typedef struct

由于编译器定位符号的规则（搜索规则）改变，导致不同于C语言。

一、如果在类标识符空间定义了 `struct Student {...};`，使用 `Student me;` 时，编译器将搜索全局标识符表，`Student` 未找到，则在类标识符内搜索。

即表现为可以使用 `Student` 也可以使用 `struct Student`，如下：

```cpp
// cpp
struct Student { 
    int age; 
};

void f( Student me );       // 正确，"struct" 关键字可省略
```

二、若定义了与 `Student` 同名函数之后，则 `Student` 只代表函数，不代表结构体，如下：

```cpp
typedef struct Student { 
    int age; 
} S;

void Student() {}           // 正确，定义后 "Student" 只代表此函数

//void S() {}               // 错误，符号 "S" 已经被定义为一个 "struct Student" 的别名

int main() {
    Student(); 
    struct Student me;      // 或者 "S me";
    return 0;
}
```

### C++ 中 struct 和 class

总的来说，struct 更适合看成是一个数据结构的实现体，class 更适合看成是一个对象的实现体。  

* 最大的两点区别：  
1. <u>默认的继承访问权限不同：</u>  
   struct 是 public 的，class 是 private 的。  
2. <u>默认的数据访问控制不同：</u>  
   struct 作为数据结构的实现体，它默认的数据访问控制是 public 的，而 class 作为对象的实现体，它默认的成员变量访问控制是 private 的。

### union 联合

<u>联合（union）是一种节省空间的特殊的类，一个 union 可以有多个数据成员，但是在任意时刻只有一个数据成员可以有值。</u>当某个成员被赋值后其他成员变为未定义状态。联合有如下特点：

* 默认访问控制符为 public
* 可以含有构造函数、析构函数
* 不能含有引用类型的成员
* 不能继承自其他类，不能作为基类
* 不能含有虚函数
* 匿名 union 在定义所在作用域可直接访问 union 成员
* 匿名 union 不能包含 protected 成员或 private 成员
* 全局匿名联合必须是静态（static）的

union 使用

```cpp
#include<iostream>

union UnionTest {
    UnionTest() : i(10) {};
    int i;
    double d;
};

static union {
    int i;
    double d;
};

int main() {
    UnionTest u;

    union {
        int i;
        double d;
    };

    std::cout << u.i << std::endl;  // 输出 UnionTest 联合的 10

    ::i = 20;
    std::cout << ::i << std::endl;  // 输出全局静态匿名联合的 20

    i = 30;
    std::cout << i << std::endl;    // 输出局部匿名联合的 30
    return 0;
}
```

### C 实现 C++ 类（待实现)

C 实现 C++ 的面向对象特性（封装、继承、多态）

* 封装：使用函数指针把属性与方法封装到结构体中
* 继承：结构体嵌套
* 多态：父类与子类方法的函数指针不同

[C语言能否通过结构体实现面向对象编程？](https://www.zhihu.com/question/59292999)


### explicit（显式）关键字

* explicit 修饰构造函数时，可以防止隐式转换和复制初始化
* explicit 修饰转换函数时，可以防止隐式转换，但 [按语境转换](https://zh.cppreference.com/w/cpp/language/implicit_conversion) 除外

explicit 使用

```cpp
struct A
{
	A(int) { }
	operator bool() const { return true; }
};

struct B
{
	explicit B(int) {}
	explicit operator bool() const { return true; }
};

void doA(A a) {}

void doB(B b) {}

int main()
{
	A a1(1);		    // OK：直接初始化
	A a2 = 1;		    // OK：复制初始化
	A a3{ 1 };		  // OK：直接列表初始化
	A a4 = { 1 };		// OK：复制列表初始化
	A a5 = (A)1;		// OK：允许 static_cast 的显式转换 
	doA(1);			    // OK：允许从 int 到 A 的隐式转换
	if (a1);		    // OK：使用转换函数 A::operator bool() 的从 A 到 bool 的隐式转换
	bool a6(a1);		// OK：使用转换函数 A::operator bool() 的从 A 到 bool 的隐式转换
	bool a7 = a1;		// OK：使用转换函数 A::operator bool() 的从 A 到 bool 的隐式转换
	bool a8 = static_cast<bool>(a1);  // OK ：static_cast 进行直接初始化

	B b1(1);		    // OK：直接初始化
	B b2 = 1;		    // 错误：被 explicit 修饰构造函数的对象不可以复制初始化
	B b3{ 1 };		  // OK：直接列表初始化
	B b4 = { 1 };		// 错误：被 explicit 修饰构造函数的对象不可以复制列表初始化
	B b5 = (B)1;		// OK：允许 static_cast 的显式转换
	doB(1);			    // 错误：被 explicit 修饰构造函数的对象不可以从 int 到 B 的隐式转换
	if (b1);		    // OK：被 explicit 修饰转换函数 B::operator bool() 的对象可以从 B 到 bool 的按语境转换
	bool b6(b1);		// OK：被 explicit 修饰转换函数 B::operator bool() 的对象可以从 B 到 bool 的按语境转换
	bool b7 = b1;		// 错误：被 explicit 修饰转换函数 B::operator bool() 的对象不可以隐式转换
	bool b8 = static_cast<bool>(b1);  // OK：static_cast 进行直接初始化

	return 0;
}
```

### friend 友元类和友元函数(todo:待补充)

* 能访问私有成员  
* 破坏封装性
* 友元关系不可传递
* 友元关系的单向性
* 友元声明的形式及数量不受限制

### `::` 范围解析运算符

* 分类

  1. 全局作用域符（`::name`）：用于类型名称（类、类成员、成员函数、变量等）前，表示作用域为全局命名空间
  2. 类作用域符（`class::name`）：用于表示指定类型的作用域范围是具体某个类的
  3. 命名空间作用域符（`namespace::name`）:用于表示指定类型的作用域范围是具体某个命名空间的

* `::` 使用

```cpp
int count = 11;         // 全局（::）的 count

class A {
public:
	static int count;   // 类 A 的 count（A::count）
};
int A::count = 21;

void fun()
{
	int count = 31;     // 初始化局部的 count 为 31
	count = 32;         // 设置局部的 count 的值为 32
}

int main() {
	::count = 12;       // 测试 1：设置全局的 count 的值为 12

	A::count = 22;      // 测试 2：设置类 A 的 count 为 22

	fun();		        // 测试 3

	return 0;
}
```

### enum 枚举类型
* 概述   
  `enum`是`C语言`中的一个关键字，`enum`叫枚举数据类型，枚举数据类型描述的是一组整型值的集合（这句话其实不太妥当），枚举型是预处理指令`#define`的替代，枚举和宏其实非常类似，宏在预处理阶段将名字替换成对应的值，枚举在编译阶段将名字替换成对应的值，我们可以将枚举理解为编译阶段的宏，使用格式：
  ```cpp
  enum typeName { valueName1, valueName2, valueName3, ...... };
  ```
  typeName是枚举类型的名字，花括号里面的<u>元素（枚举成员）是常量而不是变量，因为枚举成员是常量，所以不能对它们赋值，只能将它们的值赋给其他的变量。</u>

* 枚举类型声明  
  举个例子，比如：一星期有 7 天，如果不用枚举，我们需要使用 #define 来为每个整数定义一个别名：
  ```cpp
  #defineMON 1
  #defineTUE 2
  #defineWED 3
  #defineTHU 4
  #defineFRI 5
  #defineSAT 6
  #defineSUN 7
  ```
  这个看起来代码量就比较多，接下来我们看看使用枚举的方式：
  ```cpp
  enum DAY
  {
    MON=1, TUE, WED, THU, FRI, SAT, SUN
  };
  ```
  这样看起来是不是更简洁了。

* 枚举变量的定义  
我们可以通过以下三种方式来定义枚举变量
  1. 先定义枚举类型，再定义枚举变量
  ```cpp
  enum DAY
  {
    MON=1, TUE, WED, THU, FRI, SAT, SUN
  };
  enum DAY day;
  ```

  2. 定义枚举类型的同时定义枚举变量  
  ```cpp
  enum DAY
  {
    MON=1, TUE, WED, THU, FRI, SAT, SUN
  } day;
  ```

  3. 省略枚举名称，直接定义枚举变量
  ```cpp
  enum
  {
    MON=1, TUE, WED, THU, FRI, SAT, SUN
  } day;
  ```

  注意：同一个程序中不能定义同名的枚举类型，不同的枚举类型中也不能存在同名的命名常量。

* 使用枚举类型的变量  
  1. 先声明变量，再对变量赋值 
   ```cpp
    /* 定义枚举类型 */
    enum DAY { MON=1, TUE, WED, THU, FRI, SAT, SUN };
    void main()
    {
      /* 使用基本数据类型声明变量，然后对变量赋值 */
      int x, y, z;
      x = 10;
      y = 20;
      z = 30;

      /* 使用枚举类型声明变量，再对枚举型变量赋值 */
      enum DAY yesterday, today, tomorrow;
      yesterday = MON;
      today = TUE;
      tomorrow = WED;
      printf("%d %d %d \n", yesterday, today, tomorrow);
    }
   ``` 

  2. 声明变量的同时赋初值  
  ```cpp
  /* 定义枚举类型 */
  enum DAY { MON=1, TUE, WED, THU, FRI, SAT, SUN };
  void main()
  {
    /* 使用基本数据类型声明变量同时对变量赋初值 */
    int x=10, y=20, z=30;

    /* 使用枚举类型声明变量同时对枚举型变量赋初值 */
    enum DAY yesterday = MON, today = TUE, tomorrow = WED;
    printf("%d %d %d \n", yesterday, today, tomorrow);
  }
  ```

  3. 定义类型的同时声明变量，然后对变量赋值。
  ```cpp
  #include <stdio.h>
  /* 定义枚举类型，同时声明该类型的三个变量，它们都为全局变量 */
  enum DAY { MON=1, TUE, WED, THU, FRI, SAT, SUN } yesterday, today, tomorrow;
  /* 定义三个具有基本数据类型的变量，它们都为全局变量 */
  int x, y, z;
  void main()
  {
    /* 对基本数据类型的变量赋值 */
    x = 10; y = 20; z = 30;

    /* 对枚举型的变量赋值 */
    yesterday = MON;
    today = TUE;
    tomorrow = WED;
    printf("%d %d %d \n", x, y, z); //输出：10 20 30
    printf("%d %d %d \n", yesterday, today, tomorrow); //输出：1 2 3
  }
  ```

  4. 类型定义，变量声明，赋初值同时进行。
  ```cpp
  #include <stdio.h>
  /* 定义枚举类型，同时声明该类型的三个变量，并赋初值。它们都为全局变量 */
  enum DAY { MON=1, TUE, WED, THU, FRI, SAT, SUN } yesterday = MON, today = TUE, tomorrow = WED;
  
  /* 定义三个具有基本数据类型的变量，并赋初值。它们都为全局变量 */
  int x = 10, y = 20, z = 30;
  void main()
  {
    printf("%d %d %d \n", x, y, z); //输出：10 20 30
    printf("%d %d %d \n", yesterday, today, tomorrow); //输出：1 2 3
  }
  ```

  5. 对枚举型的变量赋整数值时，需要进行类型转换。  
  ```cpp
  #include <stdio.h>
  enum DAY { MON=1, TUE, WED, THU, FRI, SAT, SUN };
  void main()
  {
    enum DAY yesterday, today, tomorrow;
    yesterday = TUE;
    today = (enum DAY) (yesterday + 1); //类型转换
    tomorrow = (enum DAY) 30; //类型转换
    //tomorrow = 3; //错误
    printf("%d %d %d \n", yesterday, today, tomorrow); //输出：2 3 30
  }
  ```
  
* 总结  
  1. 枚举型是一个集合，集合中的元素(枚举成员)是一些命名的整型常量，元素之间用逗号,隔开。
  2. DAY是一个标识符，可以看成这个集合的名字，是一个可选项，即是可有可无的项。
  3. 第一个枚举成员的默认值为整型的0，后续枚举成员的值在前一个成员上加1。在当前值没有赋值的情况下，枚举类型的当前值总是前一个值+1.
  4. 可以人为设定枚举成员的值，从而自定义某个范围内的整数。
  5. 枚举型是预处理指令#define的替代。
  6. 类型定义以分号;结束。

[C语言丨关键字enum用法详解，看这篇就够了](https://zhuanlan.zhihu.com/p/349458733)

### decltype
* 定义  
decltype 关键字用于<u>返回给定的名字或表达式的确切型别</u>（用于检查实体的声明类型或表达式的类型及值分类）。
  
* 语法  
```cpp
decltype ( expression )
```

* decltype 使用
```cpp
vector<int> a; 
typedef decltype(a) b; 
cout << is_same<vector<int>, b>::value; // 1 
```

* 使用讲解
  * auto返回值的缺陷
  ```cpp
  template<typename Container, typename Index> 
  auto access(Container &c, Index i){ 
      return c[i]; 
  } 

  // 返回值为auto，此处返回值的型别推导为模板型别推导，
  // Container为引用，会剥去引用修饰词。  
  // auto返回的类型与T相同，此处的T为非引用，导致返回的c[i]为右值。
  // 因此，下面的语句是错误的：
  access(c, 2) = 1;   //error
  ```

  * 使用decltype将返回值变为左值
  ```cpp
  int a = 1; 
  const int &b = a; 
  decltype(auto) c = b; // 使用decltype后，c的类型与b完全相同 

  template<typename Container, typename Index>    // C++14 
  decltype(auto) access(Container &c, Index i){ 
      return c[i]; 
  } 
  // 此时，返回值为左值，下面的语句合法：
  vector<int> v{1}; 
  access(v, 0) = 2; 
  ```

  * 传入右值容器  
  
  ```cpp
  // 上述写法在面对下面的语句时会报错，
  // 原因在于不能将右值引用绑定到左值引用上。
  access(vector<int>{2}, 0) = 2; //error

  // 修改方式：
  template<typename Container, typename Index> // C++14 
  decltype(auto) access(Container &&c, Index i){ 
      return forward<Container>(c)[i]; 
  } 
  template<typename Container, typename Index> // C++11 
  auto access(Container &&c, Index i) -> decltype(forward<Container>(c)[i]){ 
      return forward<Container>(c)[i]; 
  } 
  ```

  * forward:  
  完美转发实现了参数在传递过程中保持其值属性的功能，即若是左值，则传递之后仍然是左值，若是右值，则传递之后仍然是右值。  
  换句话说，当函数参数为右值引用时，在此函数内调用另一函数，并将此右值引用作为调用函数的参数，在函数调用中右值引用会被转换为左值引用。  
  需要注意，当参数中存在万能引用时，最好加上forward保证参数的性质不变。

[理解decltype](https://zhuanlan.zhihu.com/p/265111186)


### 引用（左右引用 ）

* 左值引用  
常规引用，一般表示对象的身份。

* 右值引用  

  右值引用就是必须绑定到右值（一个临时对象、将要销毁的对象）的引用，一般表示对象的值。    

  右值引用可实现转移语义（Move Sementics）和精确传递（Perfect Forwarding），它的主要目的有两个方面：
      
  1. 消除两个对象交互时不必要的对象拷贝，节省运算存储资源，提高效率。  
  2. 能够更简洁明确地定义泛型函数。  

[谈谈C++的左值右值，左右引用，移动语意及完美转发](https://zhuanlan.zhihu.com/p/402251966)

* 引用折叠  
  * `X& &`、`X& &&`、`X&& &` 可折叠成 `X&`
  * `X&& &&` 可折叠成 `X&&`

### 左值右值
[谈谈C++的左值右值，左右引用，移动语意及完美转发](https://zhuanlan.zhihu.com/p/402251966)

### 成员初始化列表
好处：
* 更高效：少了一次调用默认构造函数的过程。
* 有些场合必须要用初始化列表：
  1. 常量成员，因为常量只能初始化不能赋值，所以必须放在初始化列表里面
  2. 引用类型，引用必须在定义的时候初始化，并且不能重新赋值，所以也要写在初始化列表里面
  3. 没有默认构造函数的类类型，因为使用初始化列表可以不必调用默认构造函数来初始化


### initializer_list 列表初始化

用花括号初始化器列表初始化一个对象，其中对应构造函数接受一个 `std::initializer_list` 参数.

* initializer_list 使用

```cpp
#include <iostream>
#include <vector>
#include <initializer_list>
 
template <class T>
struct S {
    std::vector<T> v;
    S(std::initializer_list<T> l) : v(l) {
         std::cout << "constructed with a " << l.size() << "-element list\n";
    }
    void append(std::initializer_list<T> l) {
        v.insert(v.end(), l.begin(), l.end());
    }
    std::pair<const T*, std::size_t> c_arr() const {
        return {&v[0], v.size()};  // 在 return 语句中复制列表初始化
                                   // 这不使用 std::initializer_list
    }
};
 
template <typename T>
void templated_fn(T) {}
 
int main()
{
    S<int> s = {1, 2, 3, 4, 5}; // 复制初始化
    s.append({6, 7, 8});        // 函数调用中的列表初始化
 
    std::cout << "The vector size is now " << s.c_arr().second << " ints:\n";
 
    for (auto n : s.v)
        std::cout << n << ' ';
    std::cout << '\n';
 
    std::cout << "Range-for over brace-init-list: \n";
 
    for (int x : {-1, -2, -3}) // auto 的规则令此带范围 for 工作
        std::cout << x << ' ';
    std::cout << '\n';
 
    auto al = {10, 11, 12};   // auto 的特殊规则
    std::cout << "The list bound to auto has size() = " << al.size() << '\n';
 
    //templated_fn({1, 2, 3});  // 编译错误！“ {1, 2, 3} ”不是表达式，
                                // 它无类型，故 T 无法推导
    templated_fn<std::initializer_list<int>>({1, 2, 3}); // OK
    templated_fn<std::vector<int>>({1, 2, 3});           // 也 OK
}
```
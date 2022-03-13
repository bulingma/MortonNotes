

## 官方说明
Constructs a closure: an unnamed function object capable of capturing variables in scope.  
构建一个闭包：一个可以在范围内获取变量的匿名函数对象。

* Syntax
```cpp
[ captures ] ( params ) specs requires(optional) { body }	(1)	
[ captures ] { body }	(2)	(until C++23)
[ captures ] specs { body }	(2)	(since C++23)
[ captures ] < tparams > requires(optional) ( params ) specs requires(optional) { body }	(3)	(since C++20)
[ captures ] < tparams > requires(optional) { body }	(4)	(since C++20)
(until C++23)
[ captures ] < tparams > requires(optional) specs { body }	(4)	(since C++23)
```


## 知乎回答
Lambda 表达式（Lambda Expression）是 C++11 引入的一个“语法糖”，可以方便快捷地创建一个“函数对象”。

从 C++11 开始，C++ 有三种方式可以创建/传递一个可以被调用的对象：

### 函数指针
仿函数（Functor）
Lambda 表达式
函数指针
函数指针是从 C 语言老祖宗继承下来的东西，比较原始，功能也比较弱：

无法直接捕获当前的一些状态，所有外部状态只能通过参数传递（不考虑在函数内部使用 static 变量）。
使用函数指针的调用无法 inline（编译期无法确定这个指针会被赋上什么值）。

```cpp
// 一个指向有两个整型参数，返回值为整型参数的函数指针类型
int (*)(int, int);

// 通常我们用 typedef 来定义函数指针类型的别名方便使用
typedef int (*Plus)(int, int);

// 从 C++11 开始，更推荐使用 using 来定义别名
using Plus = int (*)(int, int);
```

### 仿函数
仿函数其实就是让一个类（class/struct）的对象的使用看上去像一个函数，具体实现就是在类中实现 operator()。比如：
```cpp
class Plus {
 public:
  int operator()(int a, int b) {
    return a + b;
  }   
};

Plus plus; 
std::cout << plus(11, 22) << std::endl;   // 输出 33
```
相比函数指针，仿函数对象可通过成员变量来捕获/传递一些状态。缺点就是，写起来很麻烦（码字比较多）。


### Lambda 表达式
Lambda 表达式在表达能力上和仿函数是等价的。编译器一般也是通过自动生成类似仿函数的代码来实现 Lambda 表达式的。上面的例子，用 Lambda 改写如下：
```cpp
auto Plus = [](int a, int b) { return a + b; };
```

一个完整的 Lambda 表达式的组成如下：

```cpp
[ capture-list ] ( params ) mutable(optional) exception(optional) attribute(optional) -> ret(optional) { body } 
capture-list：捕获列表。前面的例子 auto Plus = [](int a, int b) { return a + b; }; 没有捕获任何变量。
params：和普通函数一样的参数。
mutable：只有这个 Lambda 表达式是 mutable 的才允许修改按值捕获的参数。
exception：异常标识。暂时不必理解。
attribute：属性标识。暂时不必理解。
ret：返回值类型，可以省略，让编译器通过 return 语句自动推导。
body：函数的具体逻辑。
```cpp

除了捕获列表，Lambda 表达式的其它地方其实和普通的函数基本一样。

Lambda 表达式的捕获，其实就是将局部自动变量保存到 Lambda 表达式内部（Lambda 表达式不能捕获全局变量或 static 变量）。

Lambda 表达式最常用的地方就是和标准库中的算法一起使用。下面我们用一个简单的例子来说明 Lambda 表达式的用法。

假设有一个书本信息的列表，定义如下。我们想要找出其中 title 包含某个关键字（target）的书本的数量，可以通过标准库中的 std::count_if + Lambda 表达式来实现。

```cpp
struct Book {
    int id; 
    std::string title;
    double price;
};

std::vector<Book> books;

std::string target = "C++";  // 找出其中 title 包含“C++”的书本的数量
```

Lambda 表达式的最基本的两种捕获方式是：按值捕获（Capture by Value）和按引用捕获（Capture by Reference）。

* 按值捕获
```cpp
auto cnt =
    std::count_if(books.begin(), books.end(), [target](const Book& book) {
        return book.title.find(target) != std::string::npos;
    }); 
[target] 表示按值捕获 target。Lambda 表达式内部会保存一份 target 的副本，名字也叫 target。
```

* 按引用捕获
```cpp
auto cnt =
    std::count_if(books.begin(), books.end(), [&target](const Book& book) {
        return book.title.find(target) != std::string::npos;
    }); 
[&target] 表示按引用捕获 target——不会复制多一份副本。
```

* 捕获列表初始化（Capture Initializers）
C++ 14 支持 lambda capture initializers。比如：
```cpp
// 按值捕获 target，但是在 Lambda 内部的变量名叫做 v
auto cnt =
    std::count_if(books.begin(), books.end(), [v = target](const Book& book) {
        return book.title.find(v) != std::string::npos;
    }); 

// 按引用捕获 target，但是在 Lambda 内部的名字叫做 r
auto cnt =
    std::count_if(books.begin(), books.end(), [&r = target](const Book& book) {
        return book.title.find(r) != std::string::npos;
    }); 
```
Lambda 捕获列表初始化最最最重要的一点是“支持 Capture by Move”。在 C++14 之前，Lambda 是不支持捕获一个 Move-Only 的对象的，比如：
```cpp
std::unique_ptr<int> uptr = std::make_unique<int>(123);
auto callback = [uptr]() {                               // 编译错误，uptr is move-only
    std::cout << *uptr << std::endl;
};  
```
按引用捕获虽然可以编译通过，但往往是不符合要求的。比如下面的例子，离开作用域之后 uptr 会被析构掉。但是 callback 对象已经被传给另一个线程。
```cpp
std::unique_ptr<int> uptr = std::make_unique<int>(123);
auto callback = [&uptr]() {                               
    std::cout << *uptr << std::endl;
};  
// ... 将 callback 传给另一个线程
// return => uptr delete 掉指向的内存
```

通过捕获列表初始化，完成 Move-Only 对象的“Capture by Move”。
```cpp
std::unique_ptr<int> uptr = std::make_unique<int>(123);
auto callback = [uptr = std::move(uptr)]() {    // 将 uptr 移动给 Lambda 表达式中的参数
    std::cout << *uptr << std::endl;
};
// ... 将 callback 传给另一个线程
// return => uptr 是 nullptr
```

通过捕获列表初始化，我们还可以捕获一个指针“Capture by Pointer”。
```cpp
auto cnt =
    std::count_if(books.begin(), books.end(), [p = &target](const Book& book) {
        return book.title.find(*p) != std::string::npos;
    }); 
[p = &target] 表示捕获 target 的指针，命名为 p。
```

* Default Capture  
Lambda 表示支持两种 default capture 的模式：

[=] 表示 default capture by value。按值捕获可见范围内的所有局部变量。
[&] 表示 default capture by reference。按引用捕获可见范围内的所有局部变量。
比如：
```cpp
int a = 1;
std::string s = "hello";
std::vector<int> v;

auto default_capture_by_value = [=]() {
    // 按值捕获了 a、s 和 v
};

auto default_capture_by_reference = [&]() {
    // 按引用捕获了 a、s 和 v
};
```
不建议直接使用 [&] 或 [=] 捕获所有参数，而是按需显示捕获。

按值捕获的类型是 const 的。
```cpp
int i = 100;
auto func = [i]() {
    i = 200;  // 编译错误：assignment of read-only variable ‘i’
}; 
```
如果要修改按值捕获的参数，需要将 Lambda 表达式声明为 mutable 的。
```cpp
int i = 100;  
auto func = [i]() mutable {
    i = 200; 
}; 
```
捕获 this 指针 在成员函数中的 Lambda 表达式可以捕获当前对象的 this 指针，让 Lambda 表达式拥有和当前类成员同样的访问权限，可以修改类的成员变量，使用类的成员函数。
```cpp
class Foo {
 public:
  Foo(const std::string& s, int i) : s_(s), i_(i) {}
  void Print() {
    auto do_print = [this](){
      std::cout << s_ << std::endl;
      std::cout << i_ << std::endl;
    };  
    do_print();
  }
  void Update(const std::string& s, int i) {
    auto do_update = [this, &s, i](){
      s_ = s;
      i_ = i;
    };  
    do_update();
  }
 private:
  std::string s_; 
  int i_; 
};
```
最后，this 指针只能按值捕获 [this] ，不能按引用捕获 [&this] 。

## 附录
[Lambda expressions (since C++11)](https://en.cppreference.com/w/cpp/language/lambda)  
[现代 C++：Lambda 表达式](https://zhuanlan.zhihu.com/p/150554945)
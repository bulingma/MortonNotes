

<!-- TOC -->

- [1. 什么是单例模式](#1-什么是单例模式)
- [2. 懒汉版（Lazy Singleton，Meyers' Singleton）](#2-懒汉版lazy-singletonmeyers-singleton)
  - [2.1. 双检测锁版本（Double-Checked Locking Pattern）](#21-双检测锁版本double-checked-locking-pattern)
  - [2.2. 局部静态变量版本（Meyers' Singleton）(重点掌握)](#22-局部静态变量版本meyers-singleton重点掌握)
- [3. 饿汉版（Eager Singleton）](#3-饿汉版eager-singleton)
- [4、总结](#4总结)

<!-- /TOC -->


## 1. 什么是单例模式
单例模式(Singleton Pattern，也称为单件模式)，使用最广泛的设计模式之一。其意图是保证一个类仅有一个实例，并提供一个访问它的全局访问点，该实例被所有程序模块共享。  

定义一个单例类：  
私有化它的构造函数，以防止外界创建单例类的对象；  
使用类的私有静态指针变量指向类的唯一实例；  
使用一个公有的静态方法获取该实例。  

## 2. 懒汉版（Lazy Singleton，Meyers' Singleton）  
单例实例在第一次被使用时才进行初始化，这叫做延迟初始化。

### 2.1. 双检测锁版本（Double-Checked Locking Pattern）  
```cpp
// version 1.1
class Singleton
{
    private:
        static Singleton* instance;
        Singleton() { };
        ~Singleton() { };
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);
        class Deletor {
            public:
                ~Deletor() {
                    if(Singleton::instance != NULL)
                        delete Singleton::instance;
                }
        };
        static Deletor deletor;
    public:
        static Singleton* getInstance() {
            if(instance == NULL) {
                Lock lock;              // 基于作用域的加锁，超出作用域，自动调用析构函数解锁
                if(instance == NULL) {
                    instance = new Singleton();
                }
            }
            return instance;
        }
};

// init static member
Singleton* Singleton::instance = NULL;
```
* 双检测锁模式（DCL: Double-Checked Locking Pattern）优点：   
1、 内存安全：在程序运行结束时，系统会调用静态成员deletor的析构函数，该析构函数会删除单例的唯一实例；
2、 线程安全：无race condition

* 双检测锁模式（DCL: Double-Checked Locking Pattern）缺点：  
1、 在单例类内再定义一个嵌套类（Deletor），总是感觉很麻烦；   
2、 在某些内存模型中（虽然不常见）或者是由于编译器的优化以及运行时优化等等原因，使得instance虽然已经不是nullptr但是其所指对象还没有完成构造，这种情况下，另一个线程如果调用getInstance()就有可能使用到一个不完全初始化的对象。  

### 2.2. 局部静态变量版本（Meyers' Singleton）(重点掌握)
C++11规定了local static在多线程条件下的初始化行为，要求编译器保证了内部静态变量的线程安全性。  
在C++11标准下，《Effective C++》提出了一种更优雅的单例模式实现，使用函数内的 local static 对象。  
这样，只有当第一次访问getInstance()方法时才创建实例。这种方法也被称为Meyers' Singleton。C++0x之后该实现是线程安全的，C++0x之前仍需加锁。 

```cpp
// version 1.2
class Singleton{
    private:
        Singleton(){};
        ~Singleton(){};
        Singleton& operator=(const Singleton&);
        Singleton(const Singleton&);

    public:
        static Singleton& getInstance(){
            static Singleton instance;
            return instance;
        }
}
```

## 3. 饿汉版（Eager Singleton）
指单例实例在程序运行时被立即执行初始化  
```cpp
// version 1.3
class Singleton
{
    private:
        static Singleton instance;
        Singleton();
        ~Singleton();
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);

    public:
        static Singleton& getInstance() {
            return instance;
        }
}

// initialize defaultly
Singleton Singleton::instance;
```
由于在main函数之前初始化，所以没有线程安全的问题。但是潜在问题在于no-local static对象（函数外的static对象）在不同编译单元中的初始化顺序是未定义的。也即，static Singleton instance;和static Singleton& getInstance()二者的初始化顺序不确定，如果在初始化完成之前调用 getInstance() 方法会返回一个未定义的实例。

## 4、总结
Eager Singleton 虽然是线程安全的，但存在潜在问题；  
Lazy Singleton 通常需要加锁来保证线程安全，但局部静态变量版本在C++11后是线程安全的；  
<u>局部静态变量版本（Meyers Singleton）最优雅。</u>  



[C++ 单例模式](https://zhuanlan.zhihu.com/p/37469260)
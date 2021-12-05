
<!-- TOC -->

- [序言](#序言)
- [7.1. 基本用途](#71-基本用途)
- [7.2. 对程序库的影响](#72-对程序库的影响)
- [7.3. 对面向对象程序设计的影响](#73-对面向对象程序设计的影响)
- [7.4. 对面向对象设计模式的影响](#74-对面向对象设计模式的影响)
- [7.5. 依赖注入与单元测试](#75-依赖注入与单元测试)
- [7.6. 什么时候使用继承?](#76-什么时候使用继承)
- [7.7. 基于接口的设计](#77-基于接口的设计)

<!-- /TOC -->

## 序言  
陈硕和侯杰对使用boost::function 和 boost:bind 取代虚函数 都有相关文章。  
以陈硕文章(CppPractice.pdb)中的内容为例：  

## 7.1. 基本用途
boost::function 就像 C# 里的 delegate，可以指向任何函数，包括成员函数。 当用 bind 把某个成员函数绑到某个对象上时，我们得到了一个 closure(闭包)。例 如:
```
class Foo {
   public:
    void methodA();
    void methodInt(int a);
};
class Bar {
   public:
    void methodB();
};
boost::function<void()> f1; // 无参数，无返回值
Foo foo;
f1 = boost::bind(&Foo::methodA, &foo);   
f1(); // 调用 foo.methodA();

Bar bar;
f1 = boost::bind(&Bar::methodB, &bar); 
f1(); // 调用 bar.methodB();

f1 = boost::bind(&Foo::methodInt, &foo, 42); f1(); // 调用 foo.methodInt(42);

boost::function<void(int)> f2; // int 参数，无返回值 
f2 = boost::bind(&Foo::methodInt, &foo, _1);  
f2(53); // 调用 foo.methodInt(53);  
```
如果没有 boost::bind，那么 boost::function 就什么都不是，而有了 bind， “同一个类的不同对象可以 delegate 给不同的实现，从而实现不同的行为”(myan
语)，简直就无敌了。


## 7.2. 对程序库的影响
<u>**程序库的设计不应该给使用者带来不必要的限制(耦合)，而继承是第二强的一 种耦合(最强耦合的是友元)。如果一个程序库限制其使用者必须从某个 class 派生， 那么我觉得这是一个糟糕的设计。不巧的是，目前不少 C++ 程序库就是这么做的。**</u>  


## 7.3. 对面向对象程序设计的影响  
一直以来，我对面向对象有一种厌恶感，叠床架屋，绕来绕去的，一拳拳打在棉花上，不解决实际问题。面向对象三要素是封装、继承和多态。我认为封装是根本的，继承和多态则是可有可无。用 class 来表示 concept，这是根本的;至于继承和多态，其耦合性太强，往往不划算。  

继承和多态不仅规定了函数的名称、参数、返回类型，还规定了类的继承关系。 在现代的 OO 编程语言里，借助反射和 attribute/annotation，已经大大放宽了限制。 举例来说，JUnit 3.x 是用反射，找出派生类里的名字符合 void test*() 的函数来执 行，这里就没继承什么事，只是对函数的名称有部分限制(继承是全面限制，一字不 差)。至于 JUnit 4.x 和 NUnit 2.x 则更进一步，以 annoatation/attribute 来标明 test case，更没继承什么事了。
 
我的猜测是，当初提出面向对象的时候，closure 还没有一个通用的实现，所以 它没能算作基本的抽象工具之一。现在既然 closure 已经这么方便了，或许我们应该 重新审视面向对象设计，至少不要那么滥用继承。  

自从找到了 boost::function+boost::bind 这对神兵利器，不用再考虑类直接的 继承关系，只需要基于对象的设计 (object-based)，拳拳到肉，程序写起来顿时顺手了很多。


## 7.4. 对面向对象设计模式的影响  
既然虚函数能用 closure 代替，那么很多 OO 设计模式，尤其是行为模式，失去
了存在的必要。另外，既然没有继承体系，那么创建型模式似乎也没啥用了。 最明显的是 Strategy，不用累赘的 Strategy 基类和 ConcreteStrategyA、Con-creteStrategyB 等派生类，一个 boost::function<> 成员就解决问题。另外一个例子是 Command 模式，有了 boost::function，函数调用可以直接变成对象，似乎 就没 Command 什么事了。同样的道理，Template Method 可以不必使用基类与 继承，只要传入几个 boost::function 对象，在原来调用虚函数的地方换成调用 boost::function 对象就能解决问题。  

在《设计模式》这本书提到了 23 个模式，我认为 iterator 有用(或许再加个 State)，其他都在摆谱，拉虚架子，没啥用。或许它们解决了面向对象中的常见问 题，不过要是我的程序里连面向对象(指继承和多态)都不用，那似乎也不用叨扰面 向对象设计模式了。  

或许 closure-based programming 将作为一种新的 programming paradiam 而 流行起来。

## 7.5. 依赖注入与单元测试  
前面的 EchoService 可算是依赖注入的例子，EchoService 需要一个什么东西来 发送消息，它对这个“东西”的要求只是函数原型满足 SendMessageCallback，而并 不关系数据到底发到网络上还是发到控制台。在正常使用的时候，数据应该发给网 络，而在做单元测试的时候，数据应该发给某个 DataSink。  

安照面向对象的思路，先写一个 AbstractDataSink interface，包含 sendMes- sage() 这个虚函数，然后派生出两个 classes:NetDataSink 和 MockDataSink，前 面那个干活用，后面那个单元测试用。EchoService 的构造函数应该以 Abstract- DataSink* 为参数，这样就实现了所谓的接口与实现分离。  

我认为这么做纯粹是脱了裤子放屁，直接传入一个 SendMessageCallback 对象 就能解决问题。在单元测试的时候，可以 boost::bind() 到 MockServer 上，或某个 全局函数上，完全不用继承和虚函数，也不会影响现有的设计。

## 7.6. 什么时候使用继承?  
如果是指 OO 中的 public 继承，即为了接口与实现分离，那么我只会在派生类 的数目和功能完全确定的情况下使用。换句话说，不为将来的扩展考虑，这时候面向 对象或许是一种不错的描述方法。一旦要考虑扩展，什么办法都没用，还不如把程序 写简单点，将来好大改或重写。     

如果是功能继承，那么我会考虑继承 boost::noncopyable 或 boost::enable_- shared_from_this，下一篇 blog 会讲到 enable_shared_from_this 在实现多线程安 全的对象回调时的妙用。   

例如，IO-Multiplex 在不同的操作系统下有不同的推荐实现，最通用的 select()， POSIX 的 poll()，Linux 的 epoll()，FreeBSD 的 kqueue 等等，数目固定，功能也完 全确定，不用考虑扩展。那么设计一个 NetLoop base class 加若干具体 classes 就是 不错的解决办法。换句话说，用多态来代替 switch-case 以达到简化代码的目的。

## 7.7. 基于接口的设计  

这个问题来自那个经典的讨论:不会飞的企鹅(Penguin)究竟应不应该继承自 鸟(Bird)，如果 Bird 定义了 virtual function fly() 的话。讨论的结果是，把具体的 行为提出来，作为 interface，比如 Flyable(能飞的)，Runnable(能跑的)，然后让 企鹅实现 Runnable，麻雀实现 Flyable 和 Runnable。(其实麻雀只能双脚跳，不能 跑，这里不作深究。)
进一步的讨论表明，interface 的粒度应足够小，或许包含一个 method 就够了， 那么 interface 实际上退化成了给类型打的标签 (tag)。在这种情况下，完全可以使用 boost::function 来代替，比如:
```
// 企鹅能游泳，也能跑 
class Penguin
{
   public:
    void run();
    void swim();
};
// 麻雀能飞，也能跑 
class Sparrow
{
   public:
    void fly();
    void run();
};

// 以 boost::function 作为接口
typedef boost::function<void()> FlyCallback; 
typedef boost::function<void()> RunCallback; 
typedef boost::function<void()> SwimCallback;

// 一个既用到 run，也用到 fly 的客户 class 
class Foo
{
public:
    Foo(FlyCallback flyCb, RunCallback runCb) : flyCb_(flyCb), runCb_(runCb) {}
private:
    FlyCallback flyCb_; 
    RunCallback runCb_;
};

// 一个既用到 run，也用到 swim 的客户 class 
class Bar
{
public:
    Bar(SwimCallback swimCb, RunCallback runCb) : swimCb_(swimCb), runCb_(runCb) {}
private:
    SwimCallback swimCb_; 
    RunCallback runCb_;
};
int main() {
    Sparrow s;
    Penguin p;

    // 装配起来，Foo 要麻雀，Bar 要企鹅。
    Foo foo(bind(&Sparrow::fly, &s), bind(&Sparrow::run, &s)); 
    Bar bar(bind(&Penguin::swim, &p), bind(&Penguin::run, &p));
}

最后，向伟大的 C 语言致敬!
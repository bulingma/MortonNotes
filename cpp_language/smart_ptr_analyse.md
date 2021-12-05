## 前言  
内存管理是C++中的一个常见的错误和bug来源。在大部分情形中，这些bug来自动态分配内存和指针的使用：当多次释放动态分配的内存时，可能会导致内存损坏或者致命的运行时错误；当忘记释放动态分配的内存时，会导致内存泄露。所以，我们需要智能指针来帮助我们管理动态分配的内存。其来源于一个事实：栈比堆要安全的多，因为栈上的变量离开作用域后，会自动销毁并清理。智能指针结合了栈上变量的安全性和堆上变量的灵活性。
## 引言  
```
void someFunction()
{
    Resource* ptr = new Resource; // Resource是一个类或者结构
    // 使用ptr处理
    // ...
    delete ptr;
}
```
代码很简单：申请了一份动态内存，使用之后释放了它。但是我们很容易会在函数结束前释放它。也许我们记得及时释放动态申请的内存，但是仍然有一些不可抗力导致内存无法得到释放，比如函数提前终止了。考虑下面的代码：  
```
void someFunction()
{
    Resource* ptr = new Resource; // Resource是一个类或者结构

    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;

    if (x == 0)
        return;  // 函数终止，无法释放ptr
    if (x < 0)
        throw;   // 出现异常，函数终止，无法释放ptr
    // 使用ptr处理
    // ...
    delete ptr;
}
```

此时，由于过早的return语句以及异常的抛出，ptr将得不到正确释放，从而出现内存泄露。归根到底，指针并没有一个内在机制来自动管理与释放。然后，你可能想到了类：类内部存储指针，然后在析构函数中销毁该指针。类可以实现资源的自动管理。其好处是，只要类局部变量（分配在栈上）超出其作用域（不论其是如何离开的），其析构函数一定会被执行，那么管理的内存也将一定得到销毁。基于这样的想法，我们实现了一个简单的智能指针类：  

```
template<typename T>
class Auto_ptr2{
public:
    Auto_ptr2(T* ptr = nullptr) :
        m_ptr{ ptr }
    {}

    virtual ~Auto_ptr2(){
        delete m_ptr;
    }

    Auto_ptr2(Auto_ptr2& rhs){
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
    }

    Auto_ptr2& operator=(Auto_ptr2& rhs){
        if (&rhs == this)
            return *this;
        delete m_ptr; 
        m_ptr = rhs.m_ptr; 
        rhs.m_ptr = nullptr; 
        return *this;
    }

    T& operator*() { return *m_ptr; }
    T* operator->() { return m_ptr; }
    bool isNull() const { return m_ptr == nullptr; }
private:
    T* m_ptr;
};
```
我们使用这个新类测试一下下面的代码：
```
int main()
{
    Auto_ptr2<Resource> res1(new Resource);
    Auto_ptr2<Resource> res2; // 初始化为nullptr

    cout << "res1 is " << (res1.isNull() ? "null\n" : "not null\n");
    cout << "res2 is " << (res2.isNull() ? "null\n" : "not null\n");

    res2 = res1; // 转移指针所有权

    cout << "Ownership transferred\n";

    cout << "res1 is " << (res1.isNull() ? "null\n" : "not null\n");
    cout << "res2 is " << (res2.isNull() ? "null\n" : "not null\n");

    cin.ignore(10);
    return 0;
}
```
执行上面的程序，我们可以得到下面的输出：
```
Resource acquired
res1 is not null
res2 is null
Ownership transferred
res1 is null
res2 is not null
Resource destroyed
```
可以看到，由于重载赋值运算符实现的是指针所有权转移，所以上面的代码不会出错。

如果你仔细审视Auto_ptr2，<u>**你会发现其实际上实现的是移动语义，对于移动语义来说，其将转移对象所有权，而不是进行赋值。**</u> 由于在C++11之前，并没有右值引用，所以没有机制实现移动语义。所以C++11之前的智能指针是std::auto_ptr，其实现就类似于Auto_ptr2类。但是其存在很多问题。首先如果函数中存在std::auto_ptr类型的参数，你使用一个变量进行传值时，资源所有权将会被转移，那么函数结束后资源将被销毁，然后你可能解引用这个变量，但实际上它已经是空指针了，因此程序可能崩溃。其次，std::auto_ptr内部调用的是非数组delete，那么对于动态分配的数组，std::auto_ptr无法正常工作，可能会出现内存泄露。最后，std::auto_ptr对STL不兼容，因为STL的对象在进行复制时，就是进行复制，而不是移动语义。所以实际上，在std::auto_ptr在C++11中已经被弃用了，并且在C++17中被移除标准库。

基于C++11中的右值引用与移动语义，我们可以解决上面出现的大部分问题：
```
template<typename T>
class Auto_ptr3
{
public:
    Auto_ptr3(T* ptr = nullptr):
        m_ptr{ptr}
    {}

    Auto_ptr3(const Auto_ptr3& rhs) = delete;

    Auto_ptr3(Auto_ptr3&& rhs) : m_ptr{ rhs.m_ptr }{
        rhs.m_ptr = nullptr;
    }

    Auto_ptr3& operator=(const Auto_ptr3& rhs) = delete;

    Auto_ptr3& operator=(Auto_ptr3&& rhs){
        if (this == &rhs){
            return *this;
        }
        std::swap(m_ptr, rhs.m_ptr);
        return *this;

    }

    virtual ~Auto_ptr3(){
        delete m_ptr;
    }

    T& operator*() { return *m_ptr; }
    T* operator->() { return m_ptr; }

    bool isNull() const { return m_ptr == nullptr; }
private:
    T* m_ptr;
};
```
<u>**可以看到Auto_ptr3实现了移动构造函数与移动赋值操作符的重载，进而实现了移动语义，但是同时禁用了复制构造函数与复制赋值运算符，因此这个类的变量仅可以通过传递右值，但是不能传递左值。**</u> 但是你可以将右值传递给函数的const左值引用参数。当你传递右值时，那么明显地你已经知道要转移指针所有权了，那么当前变量将不再有效。在C++11中有类似的实现，那就是std::unique_ptr，当然更智能了。

<u>**C++11标准库中含有四种智能指针：std::auto_ptr（不要使用）, std::unique_ptr, std::shared_ptr和 std::weak_ptr。下面我们逐个介绍后面三个智能指针。**</u>

## unique_ptr
std::unique_ptr是std::auto_ptr的替代品，其用于不能被多个实例共享的内存管理。这就是说，仅有一个实例拥有内存所有权。它的使用很简单：
```
class Fraction
{
private:
    int m_numerator = 0;
    int m_denominator = 1;

public:
    Fraction(int numerator = 0, int denominator = 1) :
        m_numerator(numerator), m_denominator(denominator)
    {}

    friend std::ostream& operator<<(std::ostream& out, const Fraction &f1){
        out << f1.m_numerator << "/" << f1.m_denominator;
        return out;
    }
};

int main()
{
    std::unique_ptr<Fraction> f1{ new Fraction{ 3, 5 } };
    cout << *f1 << endl; // output: 3/5

    std::unique_ptr<Fraction> f2; // 初始化为nullptr

    // f2 = f1 // 非法，不允许左值赋值
    f2 = std::move(f1);  // 此时f1转移到f2，f1变为nullptr

    // C++14 可以使用 make_unique函数
    auto f3 = std::make_unique<Fraction>(2, 7);
    cout << *f3 << endl;  // output: 2/7

    // 处理数组，但是尽量不用这样做，因为你可以用std::array或者std::vector
    auto f4 = std::make_unique<Fraction[]>(4);
    std::cout << f4[0] << endl; // output: 0/1

    cin.ignore(10);
    return 0;
}
```
如果编译器支持，尽量使用make_unique函数创建unique_ptr实例，如果不支持，你可以实现简化的版本：
```
// 注意：无法处理数组
template<typename T, typename ... Ts>
std::unique_ptr<T> make_unique(Ts ... args)
{
    return std::unique_ptr<T> {new T{ std::forward<Ts>(args) ... }};
}
```
可以看到，std::unique_ptr对象可以作为函数返回值使用，因为函数返回值是个右值，复制给其他变量时，通过移动语义来实现。当然，你可以将std::unique_ptr对象传递给函数，看下面的例子：
```
class Resource
{
public:
    Resource() { cout << "Resource acquired!" << endl; }
    virtual ~Resource() { cout << "Resource destoryed!" << endl; }

    friend std::ostream& operator<<(std::ostream& out, const Resource &res){
        out << "I am a resource" << endl;
        return out;
    }
};

void useResource(const std::unique_ptr<Resource>& res){
    if (res){
        cout << *res;
    }
}

int main()
{
    {
        auto ptr = std::make_unique<Resource>();
        useResource(ptr);
        cout << "Ending" << endl;
    }
    // output
    // Resource acquired
    // I am a resource
    // Ending
    // Resource destroyed
    cin.ignore(10);
    return 0;
}
```
可以看到std::unique_ptr对象可以传值给左值常量引用参数，因为这并不会改变内存所有权。也可以右值传值，实现移动语义：
```
void takeOwnerShip(std::unique_ptr<Resource>&& res) // 也可以用 std::unique_ptr<Resource> res{
    if (res){
        cout << *res;
    }
}

int main(){
    {
        auto ptr = std::make_unique<Resource>();
        // takeOwnerShip(ptr); // 非法
        takeOwnerShip(std::move(ptr)); // 必须传递右值
        cout << "Ending" << endl;
    }
    // output
    // Resource acquired
    // I am a resource
    // Resource destroyed
    // Ending
    cin.ignore(10);
    return 0;
}
```
可以看到，std::unique_ptr对象可以方便地管理动态内存。但是前提是该对象是建立在栈上的，千万不要使用动态分配的类对象，那么将在堆上，其行为与普通指针变得一样。

使用std::unique_ptr可能犯的两个错误是：
```
// 千万不要用同一个资源来初始化多个std::unique_ptr对象
Resource *res = new Resource;
std::unique_ptr<Resource> res1(res);
std::unique_ptr<Resource> res2(res);

// 不要混用普通指针与智能指针
Resource *res = new Resource;
std::unique_ptr<Resource> res1(res);
delete res;
```

std::unique_ptr默认使用new和delete运算符来分配和释放内存，可以修改这个行为，下面的代码使用malloc()和free()函数管理资源：
```
// 大部分时候没有理由这样做
auto deleter = [](int* p) { free(p); };
int* p = (int*)malloc(sizeof(int));
*p = 2;
std::unique_ptr<int, decltype(deleter)> mySmartPtr{ p, deleter };
cout << *mySmartPtr << endl; // output: 2
```
std::unique_ptr还有几个常用的方法： 1. release()：返回该对象所管理的指针，同时释放其所有权； 2. reset()：析构其管理的内存，同时也可以传递进来一个新的指针对象； 3. swap()：交换所管理的对象； 4. get()：返回对象所管理的指针； 5. get_deleter()：返回析构其管理指针的调用函数。

使用上面的方法还是要小心，如不要将其它对象所管理的指针传给另外一个对象的reset()方法，这会造成一块内存释放多次。更多详情可以参考这里[cppreference.com-std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)  。

## shard_ptr
std::shared_ptr与std::unique_ptr类似。要创建std::shared_ptr对象，可以使用make_shared()函数（C++11是支持的，貌似制定这个标准的人忘了make_unique()，所以在C++14追加了）。  
<u>**std::shared_ptr与std::unique_ptr的主要区别在于前者是使用引用计数的智能指针。引用计数的智能指针可以跟踪引用同一个真实指针对象的智能指针实例的数目。这意味着，可以有多个std::shared_ptr实例可以指向同一块动态分配的内存，当最后一个引用对象离开其作用域时，才会释放这块内存。**</u>还有一个区别是std::shared_ptr不能用于管理C语言风格的动态数组，这点要注意。下面看例子：
```
int main()
{
    auto ptr1 = std::make_shared<Resource>();
    cout << ptr1.use_count() << endl;  // output: 1
    {
        auto ptr2 = ptr1;  // 通过复制构造函数使两个对象管理同一块内存
        std::shared_ptr<Resource> ptr3;   // 初始化为空
        ptr3 = ptr1;   // 通过赋值，共享内存
        cout << ptr1.use_count() << endl;  // output: 3
        cout << ptr2.use_count() << endl;  // output: 3
        cout << ptr3.use_count() << endl;  // output: 3
    }
    // 此时ptr2与ptr3对象析构了
    cout << ptr1.use_count() << endl;  // output: 1

    cin.ignore(10);
    return 0;
}
```
可以看到，通过复制构造函数或者赋值来共享内存，知道这一点很重要，看下面的例子：
```
int main()
{
    Resource* res = new Resource;
    std::shared_ptr<Resource> ptr1{ res };
    cout << ptr1.use_count() << endl;  // output: 1
    {

        std::shared_ptr<Resource> ptr2{ res };   // 用同一块内存初始化

        cout << ptr1.use_count() << endl;  // output: 1
        cout << ptr2.use_count() << endl;  // output: 1

    }
    // 此时ptr2对象析构了, output：Resource destroyed
    cout << ptr1.use_count() << endl;  // output: 1

    cin.ignore(10);
    return 0;
}
```
很奇怪，ptr1与ptr2虽然是用同一块内存初始化，但是这个共享却并不被两个对象所知道。这是由于两个对象是独立初始化的，它们互相之间没有通信。当然上面的程序会最终崩溃，因为同一块内存会被析构两次。所以，还是使用复制构造函数还有赋值运算来使不同对象管理同一块内存。如果深挖的话，std::shared_ptr与std::unique_ptr内部实现机理有区别，前者内部使用两个指针，一个指针用于管理实际的指针，另外一个指针指向一个”控制块“，其中记录了哪些对象共同管理同一个指针。这是在初始化完成的，所以如果单独初始化两个对象，尽管管理的是同一块内存，它们各自的”控制块“没有互相记录的。所以，上面的问题就出现了。但是如果是使用复制构造函数还有赋值运算时，“控制块”会同步更新的，这样就达到了引用计数的目的。使用std::make_shared就不会出现上面的问题，所以要推荐使用。

std::shared_ptr还有其他方法，更多的信息在这里[std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)。  

## weak_ptr  
std::shared_ptr可以实现多个对象共享同一块内存，当最后一个对象离开其作用域时，这块内存被释放。但是仍然有可能出现内存无法被释放的情况，联想一下“死锁”现象，对于std::shared_ptr会出现类似的“循环引用”现象：
```
class Person
{
public:
    Person(const string& name):m_name{name}{
        cout << m_name << " created" << endl;
    }

    virtual ~Person(){
        cout << m_name << " destoryed" << endl;
    }

    friend bool partnerUp(std::shared_ptr<Person>& p1, std::shared_ptr<Person>& p2){
        if (!p1 || !p2){
            return false;
        }

        p1->m_partner = p2;
        p2->m_partner = p1;

        cout << p1->m_name << " is now partenered with " << p2->m_name << endl;
        return true;
    }

private:
    string m_name;
    std::shared_ptr<Person> m_partner;
};

int main()
{
    {
        auto p1 = std::make_shared<Person>("Lucy");
        auto p2 = std::make_shared<Person>("Ricky");
        partnerUp(p1, p2);  // 互相设为伙伴
    }

    cin.ignore(10);
    return 0;
}
```
整个程序很简单，创建两个Person动态对象，交由智能指针管理，并且通过partnerUp()函数互相引用为自己的伙伴。但是执行的结果却却有问题：
```
Lucy created
Ricky created
Lucy is now partnered with Ricky
```
对象没有被析构！出现内存泄露！仔细想想std::shared_ptr对象是什么时候才能被析构，就是引用计数变为0时，但是当你想析构p1时，p2内部却引用了p1，无法析构；反过来也无法析构。互相引用造成了“死锁”，最终内存泄露！这样的情形也会出现在“自锁”中：
```
int main()
{
    {
        auto p1 = std::make_shared<Person>("Lucy");
        partnerUp(p1, p1);  // 自己作为自己的伙伴
    }

    cin.ignore(10);
    return 0;
}
```
<u>**这时候std::weak_ptr应运而生。std::weak_ptr可以包含由std::shared_ptr所管理的内存的引用。但是它仅仅是旁观者，并不是所有者。那就是std::weak_ptr不拥有这块内存，当然不会计数，也不会阻止std::shared_ptr释放其内存。**</u>但是它可以通过lock()方法返回一个std::shared_ptr对象，从而访问这块内存。这样我们可以用std::weak_ptr来解决上面的“循环引用”问题：  
```
class Person
{
public:
    Person(const string& name): m_name{name}{
        cout << m_name << " created" << endl;
    }

    virtual ~Person() {
        cout << m_name << " destoryed" << endl;
    }

    friend bool partnerUp(std::shared_ptr<Person>& p1, std::shared_ptr<Person>& p2) {
        if (!p1 || !p2){
            return false;
        }

        p1->m_partner = p2;  // weak_ptr重载的赋值运算符中可以接收shared_ptr对象
        p2->m_partner = p1;

        cout << p1->m_name << " is now partenered with " << p2->m_name << endl;
        return true;
    }

private:
    string m_name;
    std::weak_ptr<Person> m_partner;
};

int main()
{
    {
        auto p1 = std::make_shared<Person>("Lucy");
        auto p2 = std::make_shared<Person>("Ricky");
        partnerUp(p1, p2);  // 互相设为伙伴
    }

    cin.ignore(10);
    return 0;
}
```
程序正常输出（注意创建与析构的顺序是反的？在栈上！）：  
```
Lucy created
Ricky created
Lucy is now partnered with Ricky
Ricky destroyed
Lucy destroyed
```
有关std::weak_ptr更多的信息在这里[std::weak_ptr](https://en.cppreference.com/w/cpp/memory/weak_ptr)。  

最后说点感想：还是要深刻理解智能指针的内部机理，采能用好它，否则你可能会遇到与普通指针一样的问题。  


[为什么常量左值引用可以绑定到右值？](https://www.zhihu.com/search?hybrid_search_extra=%7B%22sourceType%22%3A%22article%22%2C%22sourceId%22%3A54078587%7D&hybrid_search_source=Entity&q=%E5%B7%A6%E5%80%BC%E5%B8%B8%E9%87%8F&search_source=Entity&type=content)  

[C++智能指针]([C++智能指针](https://zhuanlan.zhihu.com/p/54078587))
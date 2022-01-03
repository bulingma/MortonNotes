
<!-- TOC -->

- [1、简单工厂模式](#1简单工厂模式)
  - [1.1. 设计思路](#11-设计思路)
  - [1.2. 结构图](#12-结构图)
  - [1.3. 代码](#13-代码)
- [2、工厂方法模式](#2工厂方法模式)
  - [2.1. 设计思路](#21-设计思路)
  - [2.2. 结构图](#22-结构图)
  - [2.3. 代码](#23-代码)
- [3、抽象工厂模式](#3抽象工厂模式)
  - [3.1. 设计思路](#31-设计思路)
  - [3.2. 结构图](#32-结构图)
  - [3.3. 代码](#33-代码)
- [4、参考资料](#4参考资料)

<!-- /TOC -->


## 1、简单工厂模式

### 1.1. 设计思路    
1. 工厂模式包含两类对象——工厂和产品，所以有两个大类factory和product。  
2. 不同的产品类型都继承自产品类基类。生产不同产品的工厂都继承自工厂基类，因此都继承了工厂基类的创建产品成员函数，并重载以生产不同产品。  
3. 工厂类和产品类的联系是，工厂生产（函数）产品。因此工厂类中要包含一个返回产品类（指针）的成员函数，这是二者的桥梁。  
4. c++基类指针可以指向子类对象（多态特性，反之不行），因此工厂类成员函数返回基类指针。  
5. 工厂使用:先有工厂后有产品，因此先创建想生产的产品的工厂，再生产对应的产品（调用生产产品对应的成员函数）。具体做法: 先定义一个工厂基类指针指向具体的工厂对象（new一个），再定义一个产品基类指针指向具体的产品对象（用上一步中得到的工厂基类指针调用生产产品的成员函数（多态），该函数new一个产品子类，并返回产品基类指针）。new出来的是对象的指针。  
6. 从基类继承方法后，只用修改函数体内部即可完成重载。产品怎么生产，是由产品类决定的（继承加多态）;生产哪个产品，是由工厂类决定的。只是一种情况，不绝对，还可以适当＋想象力。  

### 1.2. 结构图
![simple_factory_utl](../../z_images/design_partern/../../MortonNotes/z_images/design_partern/simple_factory_utl.jpeg)

### 1.3. 代码
```cpp
_______________________________________________________________________________________
// 工厂
class Factory
{
public:
    enum CAR_TYPE {
       BENZ_CAR,  // 奔驰汽车
       BMW_CAR,  // 宝马汽车
       AUDI_CAR  // 奥迪汽车
    };

    // 生产汽车
    ICar* CreateCar(CAR_TYPE type) {
        ICar *pCar = NULL;
        switch(type) {
        case CAR_TYPE::BENZ_CAR:
            pCar = new BenzCar();  // 奔驰汽车
            break;
        case CAR_TYPE::BMW_CAR:
            pCar = new BmwCar();  // 宝马汽车
            break;
        case CAR_TYPE::AUDI_CAR:
            pCar = new AudiCar();  // 奥迪汽车
            break;
        default:
            break;
        }
        return pCar;
    }
};

___________________________________________________________
// 汽车接口
class ICar
{
public:
    virtual string Name() = 0;  // 汽车名称
};

___________________________________________________________
// 奔驰汽车
class BenzCar : public ICar
{
public:
    string Name() {
        return "Benz Car";
    }
};

// 宝马汽车
class BmwCar : public ICar
{
public:
    string Name() {
        return "Bmw Car";
    }
};

// 奥迪汽车
class AudiCar : public ICar
{
public:
    string Name() {
        return "Audi Car";
    }
};
___________________________________________________________
#define SAFE_DELETE(p) { if(p){delete(p); (p)=NULL;} }
int main()
{
    // 工厂
    Factory *pFactory = new Factory();

    // 奔驰汽车
    ICar *pCar = pFactory->CreateCar(Factory::BENZ_CAR);
    cout << pCar->Name() << endl;

    SAFE_DELETE(pCar);
}
```
## 2、工厂方法模式
### 2.1. 设计思路    
工厂方法模式：不再只由一个工厂类决定那一个产品类应当被实例化, 这个决定权被交给子类去做。当有新的产品（新型汽车）产生时，只要按照抽象产品角色、抽象工厂角色提供的方法来生成即可（新车型可以用一个新类继承创建产品即可），那么就可以被客户使用，而不必去修改任何已有的代码。<u>**可以看出工厂角色的结构也是符合开闭原则**</u>。  

### 2.2. 结构图   
![simple_factofactory_func](../../z_images/design_partern/../../MortonNotes/z_images/design_partern/factory_func.jpeg)
### 2.3. 代码  
```cpp
___________________________________________________
// 汽车接口
class ICar
{
public:
    virtual string Name() = 0;  // 汽车名称
};

// 奔驰汽车
class BenzCar : public ICar
{
public:
    string Name() {
        return "Benz Car";
    }
};

// 宝马汽车
class BmwCar : public ICar
{
public:
    string Name() {
        return "Bmw Car";
    }
};

// 奥迪汽车
class AudiCar : public ICar
{
public:
    string Name() {
        return "Audi Car";
    }
};

___________________________________________________
// 工厂接口
class AFactory
{
public:
    virtual ICar* CreateCar() = 0;  // 生产汽车
};

// 奔驰工厂
class BenzFactory : public AFactory
{
public:
    ICar* CreateCar() {
        return new BenzCar();
    }
};

// 宝马工厂
class BmwFactory : public AFactory
{
public:
    ICar* CreateCar() {
        return new BmwCar();
    }
};

// 奥迪工厂
class AudiFactory : public AFactory
{
public:
    ICar* CreateCar() {
        return new AudiCar();
    }
};

___________________________________________________
#define SAFE_DELETE(p) { if(p){delete(p); (p)=NULL;} }
int main()
{
    // 奔驰
    AFactory *pFactory = new BenzFactory();
    ICar *pCar = pFactory->CreateCar();
    cout << "Benz factory: " << pCar->Name() << endl;

    SAFE_DELETE(pCar);
    SAFE_DELETE(pFactory);
}
```

## 3、抽象工厂模式
### 3.1. 设计思路 
在上面的工厂方法模式基础上，又需要生产新的车型。
抽象工厂模式可以说是：简单工厂模式 + 工厂方法模式。  

### 3.2. 结构图   
![abstract_factory](../../z_images/design_partern/../../MortonNotes/z_images/design_partern/abstract_factory.jpeg)

### 3.3. 代码
```cpp
_________________________________________
// 抽象工厂 
class AFactory
{
public:
    enum FACTORY_TYPE {
        BENZ_FACTORY,  // 奔驰工厂
        BMW_FACTORY,  // 宝马工厂
        AUDI_FACTORY  // 奥迪工厂
    };

    virtual ICar* CreateCar() = 0;  // 生产汽车
    virtual IBike* CreateBike() = 0;    // 生产自行车
    static AFactory* CreateFactory(FACTORY_TYPE factory);  // 创建工厂
};

_________________________________________
// 创建工厂
AFactory* AFactory::CreateFactory(FACTORY_TYPE factory)
{
    AFactory *pFactory = NULL;
    switch (factory) {
    case FACTORY_TYPE::BENZ_FACTORY:  // 奔驰工厂
        pFactory = new BenzFactory();
        break;
    case FACTORY_TYPE::BMW_FACTORY:  // 宝马工厂
        pFactory = new BmwFactory();
        break;
    case FACTORY_TYPE::AUDI_FACTORY:  // 奥迪工厂
        pFactory = new AudiFactory();
        break;
    default:
        break;
    }
    return pFactory;
}

_________________________________________
// 汽车接口
class ICar
{
public:
    virtual string Name() = 0;  // 汽车名称
};

// 自行车接口
class IBike
{
public:
    virtual string Name() = 0;  // 自行车名称
};

_________________________________________
/********** 汽车 **********/
// 奔驰
class BenzCar : public ICar
{
public:
    string Name() {
        return "Benz Car";
    }
};

// 宝马
class BmwCar : public ICar
{
public:
    string Name() {
        return "Bmw Car";
    }
};

// 奥迪
class AudiCar : public ICar
{
public:
    std::string Name() {
        return "Audi Car";
    }
};

/********** 自行车 **********/
// 奔驰
class BenzBike : public IBike
{
public:
    string Name() {
        return "Benz Bike";
    }
};

// 宝马
class BmwBike : public IBike
{
public:
    string Name() {
        return "Bmw Bike";
    }
};

// 奥迪
class AudiBike : public IBike
{
public:
    string Name() {
        return "Audi Bike";
    }
};


_________________________________________
// 奔驰工厂
class BenzFactory : public AFactory
{
public:
    ICar* CreateCar() {
        return new BenzCar();
    }

    IBike* CreateBike() {
        return new BenzBike();
    }
};

// 宝马工厂
class BmwFactory : public AFactory
{
public:
    ICar* CreateCar() {
        return new BmwCar();
    }

    IBike* CreateBike() {
        return new BmwBike();
    }
};

// 奥迪工厂
class AudiFactory : public AFactory
{
public:
    ICar* CreateCar() {
        return new AudiCar();
    }

    IBike* CreateBike() {
        return new AudiBike();
    }
};


_________________________________________
#define SAFE_DELETE(p) { if(p){delete(p); (p)=NULL;} }
int main()
{
    // 奔驰
    AFactory *pFactory = AFactory::CreateFactory(AFactory::FACTORY_TYPE::BENZ_FACTORY);
    ICar *pCar = pFactory->CreateCar();
    IBike *pBike = pFactory->CreateBike();

    cout << "Benz factory - Car: " << pCar->Name() << endl;
    cout << "Benz factory - Bike: " << pBike->Name() << endl;

    SAFE_DELETE(pCar);
    SAFE_DELETE(pBike);
    SAFE_DELETE(pFactory);
}
```


## 4、参考资料
[c++设计模式——工厂模式](https://zhuanlan.zhihu.com/p/151390048)
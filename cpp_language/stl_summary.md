
<!-- TOC -->

- [STL 容器特点总结](#stl-容器特点总结)
- [STL 算法特点总结](#stl-算法特点总结)
- [STL 容器展开讲解](#stl-容器展开讲解)
  - [容器（containers）分类](#容器containers分类)
  - [array](#array)
  - [vector](#vector)
  - [deque](#deque)
  - [forward\_list](#forward_list)
  - [list](#list)
  - [stack](#stack)
  - [queue](#queue)
  - [priority_queue](#priority_queue)
  - [set](#set)
  - [multiset](#multiset)
  - [map](#map)
  - [multimap](#multimap)
- [STL面试题汇总](#stl面试题汇总)
  - [STL的六大组件](#stl的六大组件)
  - [容器分类介绍](#容器分类介绍)
  - [迭代器失效](#迭代器失效)

<!-- /TOC -->


### STL 容器特点总结

| 容器                                                                                         | 底层数据结构      | 时间复杂度                                                    | 有无序 | 可不可重复 | 其他                                                                         |
| -------------------------------------------------------------------------------------------- | ----------------- | ------------------------------------------------------------- | ------ | ---------- | ---------------------------------------------------------------------------- |
| [array](https://github.com/huihut/interview/tree/master/STL#array)                           | 数组              | 随机读改 O(1)                                                 | 无序   | 可重复     | 支持随机访问                                                                 |
| [vector](https://github.com/huihut/interview/tree/master/STL#vector)                         | 数组              | 随机读改、尾部插入、尾部删除 O(1)<br/>头部插入、头部删除 O(n) | 无序   | 可重复     | 支持随机访问                                                                 |
| [deque](https://github.com/huihut/interview/tree/master/STL#deque)                           | 双端队列          | 头尾插入、头尾删除 O(1)                                       | 无序   | 可重复     | 一个中央控制器 + 多个缓冲区，支持首尾快速增删，支持随机访问                  |
| [forward_list](https://github.com/huihut/interview/tree/master/STL#forward_list)             | 单向链表          | 插入、删除 O(1)                                               | 无序   | 可重复     | 不支持随机访问                                                               |
| [list](https://github.com/huihut/interview/tree/master/STL#list)                             | 双向链表          | 插入、删除 O(1)                                               | 无序   | 可重复     | 不支持随机访问                                                               |
| [stack](https://github.com/huihut/interview/tree/master/STL#stack)                           | deque / list      | 顶部插入、顶部删除 O(1)                                       | 无序   | 可重复     | deque 或 list 封闭头端开口，不用 vector 的原因应该是容量大小有限制，扩容耗时 |
| [queue](https://github.com/huihut/interview/tree/master/STL#queue)                           | deque / list      | 尾部插入、头部删除 O(1)                                       | 无序   | 可重复     | deque 或 list 封闭头端开口，不用 vector 的原因应该是容量大小有限制，扩容耗时 |
| [priority_queue](https://github.com/huihut/interview/tree/master/STL#priority_queue)         | vector + max-heap | 插入、删除 O(log<sub>2</sub>n)                                | 有序   | 可重复     | vector容器+heap处理规则                                                      |
| [set](https://github.com/huihut/interview/tree/master/STL#set)                               | 红黑树            | 插入、删除、查找 O(log<sub>2</sub>n)                          | 有序   | 不可重复   |
| [multiset](https://github.com/huihut/interview/tree/master/STL#multiset)                     | 红黑树            | 插入、删除、查找 O(log<sub>2</sub>n)                          | 有序   | 可重复     |
| [map](https://github.com/huihut/interview/tree/master/STL#map)                               | 红黑树            | 插入、删除、查找 O(log<sub>2</sub>n)                          | 有序   | 不可重复   |
| [multimap](https://github.com/huihut/interview/tree/master/STL#multimap)                     | 红黑树            | 插入、删除、查找 O(log<sub>2</sub>n)                          | 有序   | 可重复     |
| [unordered_set](https://github.com/huihut/interview/tree/master/STL#unordered_set)           | 哈希表            | 插入、删除、查找 O(1) 最差 O(n)                               | 无序   | 不可重复   |
| [unordered_multiset](https://github.com/huihut/interview/tree/master/STL#unordered_multiset) | 哈希表            | 插入、删除、查找 O(1) 最差 O(n)                               | 无序   | 可重复     |
| [unordered_map](https://github.com/huihut/interview/tree/master/STL#unordered_map)           | 哈希表            | 插入、删除、查找 O(1) 最差 O(n)                               | 无序   | 不可重复   |
| [unordered_multimap](https://github.com/huihut/interview/tree/master/STL#unordered_multimap) | 哈希表            | 插入、删除、查找 O(1) 最差 O(n)                               | 无序   | 可重复     |



### STL 算法特点总结

| 算法                                                                                             | 底层算法                                            | 时间复杂度            | 可不可重复 |
| ------------------------------------------------------------------------------------------------ | --------------------------------------------------- | --------------------- | ---------- |
| [find](http://www.cplusplus.com/reference/algorithm/find/)                                       | 顺序查找                                            | O(n)                  | 可重复     |
| [sort](https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/include/bits/stl_algo.h#L4808) | [内省排序](https://en.wikipedia.org/wiki/Introsort) | O(n*log<sub>2</sub>n) | 可重复     |


### STL 容器展开讲解
#### 容器（containers）分类

* 序列式容器（sequence containers）：元素都是可序（ordered），但未必是有序（sorted）
* 关联式容器（associattive containers）

#### array

array是固定大小的顺序容器，它们保存了一个以严格的线性顺序排列的特定数量的元素。

在内部，一个数组除了它所包含的元素（甚至不是它的大小，它是一个模板参数，在编译时是固定的）以外不保存任何数据。存储大小与用语言括号语法（[]）声明的普通数组一样高效。这个类只是增加了一层成员函数和全局函数，所以数组可以作为标准容器使用。

与其他标准容器不同，数组具有固定的大小，并且不通过分配器管理其元素的分配：它们是封装固定大小数组元素的聚合类型。因此，他们不能动态地扩大或缩小。

零大小的数组是有效的，但是它们不应该被解除引用（成员的前面，后面和数据）。

与标准库中的其他容器不同，交换两个数组容器是一种线性操作，它涉及单独交换范围内的所有元素，这通常是相当低效的操作。另一方面，这允许迭代器在两个容器中的元素保持其原始容器关联。

数组容器的另一个独特特性是它们可以被当作元组对象来处理：array头部重载get函数来访问数组元素，就像它是一个元组，以及专门的tuple_size和tuple_element类型。

#### vector
vector是表示可以改变大小的数组的序列容器。

就像数组一样，vector为它们的元素使用连续的存储位置，这意味着它们的元素也可以使用到其元素的常规指针上的偏移来访问，而且和数组一样高效。但是与数组不同的是，它们的大小可以动态地改变，它们的存储由容器自动处理。

在内部，vector使用一个动态分配的数组来存储它们的元素。这个数组可能需要重新分配，以便在插入新元素时增加大小，这意味着分配一个新数组并将所有元素移动到其中。就处理时间而言，这是一个相对昂贵的任务，因此每次将元素添加到容器时矢量都不会重新分配。

相反，vector容器可以分配一些额外的存储以适应可能的增长，并且因此容器可以具有比严格需要包含其元素（即，其大小）的存储更大的实际容量。库可以实现不同的策略的增长到内存使用和重新分配之间的平衡，但在任何情况下，再分配应仅在对数生长的间隔发生尺寸，使得在所述载体的末端各个元件的插入可以与提供分期常量时间复杂性。

因此，与数组相比，载体消耗更多的内存来交换管理存储和以有效方式动态增长的能力。

与其他动态序列容器（deques，lists和 forward\_lists ）相比，vector非常有效地访问其元素（就像数组一样），并相对有效地从元素末尾添加或移除元素。对于涉及插入或移除除了结尾之外的位置的元素的操作，它们执行比其他位置更差的操作，并且具有比列表和 forward\_lists 更不一致的迭代器和引用。

针对 vector 的各种常见操作的复杂度（效率）如下： 
* 随机访问 - 常数 O(1) 
* 在尾部增删元素 - 平摊（amortized）常数 O(1)}} 
* 增删元素 - 至 vector 尾部的线性距离 O(n)}}



#### deque
deque（['dek]）（双端队列）是double-ended queue 的一个不规则缩写。deque是具有动态大小的序列容器，可以在两端（前端或后端）扩展或收缩。

特定的库可以以不同的方式实现deques，通常作为某种形式的动态数组。但是在任何情况下，它们都允许通过随机访问迭代器直接访问各个元素，通过根据需要扩展和收缩容器来自动处理存储。

因此，它们提供了类似于vector的功能，但是在序列的开始部分也可以高效地插入和删除元素，而不仅仅是在结尾。但是，与vector不同，deques并不保证将其所有元素存储在连续的存储位置：deque通过偏移指向另一个元素的指针访问元素会导致未定义的行为。

vector和deques提供了一个非常相似的接口，可以用于类似的目的，但内部工作方式完全不同：虽然vector使用单个数组需要偶尔重新分配以增长，但是deque的元素可以分散在不同的块的容器，容器在内部保存必要的信息以提供对其任何元素的持续时间和统一的顺序接口（通过迭代器）的直接访问。因此，deques在内部比vector更复杂一点，但是这使得他们在某些情况下更有效地增长，尤其是在重新分配变得更加昂贵的很长序列的情况下。

对于频繁插入或删除开始或结束位置以外的元素的操作，deques表现得更差，并且与列表和转发列表相比，迭代器和引用的一致性更低。

deque上常见操作的复杂性（效率）如下： 

* 随机访问 - 常数O(1) 
* 在结尾或开头插入或移除元素 - 摊销不变O(1) 
* 插入或移除元素 - 线性O(n)
```cpp
template < class T, class Alloc = allocator<T> > class deque;
```
![](http://img.blog.csdn.net/20170727225856144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRlg2Nzc1ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://images0.cnblogs.com/blog/559453/201401/092150340824.png)



#### forward\_list

forward_list（单向链表）是序列容器，允许在序列中的任何地方进行恒定的时间插入和擦除操作。

forward\_list（单向链表）被实现为单链表; 单链表可以将它们包含的每个元素存储在不同和不相关的存储位置中。通过关联到序列中下一个元素的链接的每个元素来保留排序。forward\_list容器和列表

之间的主要设计区别容器是第一个内部只保留一个到下一个元素的链接，而后者每个元素保留两个链接：一个指向下一个元素，一个指向前一个元素，允许在两个方向上有效的迭代，但是每个元素消耗额外的存储空间并且插入和移除元件的时间开销略高。因此，forward_list对象比列表对象更有效率，尽管它们只能向前迭代。

与其他基本的标准序列容器（array，vector和deque），forward_list通常在插入，提取和移动容器内任何位置的元素方面效果更好，因此也适用于密集使用这些元素的算法，如排序算法。

的主要缺点修饰符Modifiers S和列表相比这些其它序列容器s是说，他们缺乏可以通过位置的元素的直接访问; 例如，要访问forward_list中的第六个元素，必须从开始位置迭代到该位置，这需要在这些位置之间的线性时间。它们还消耗一些额外的内存来保持与每个元素相关联的链接信息（这可能是大型小元素列表的重要因素）。

该修饰符Modifiersclass模板的设计考虑到效率：按照设计，它与简单的手写C型单链表一样高效，实际上是唯一的标准容器，为了效率的考虑故意缺少尺寸成员函数：由于其性质作为一个链表，具有一个需要一定时间的大小的成员将需要它保持一个内部计数器的大小（如列表所示）。这会消耗一些额外的存储空间，并使插入和删除操作效率稍低。要获取forward_list对象的大小，可以使用距离算法的开始和结束，这是一个需要线性时间的操作。

![](http://img.blog.csdn.net/20160407212133266)


#### list

#### stack

#### queue

#### priority_queue

#### set

#### multiset

#### map

map 是关联容器，按照特定顺序存储由 key value (键值) 和 mapped value (映射值) 组合形成的元素。

在映射中，键值通常用于对元素进行排序和唯一标识，而映射的值存储与此键关联的内容。该类型的键和映射的值可能不同，并且在部件类型被分组在一起VALUE_TYPE，这是一种对类型结合两种：
 
```cpp
typedef pair<const Key, T> value_type;
```

在内部，映射中的元素总是按照由其内部比较对象（比较类型）指示的特定的严格弱排序标准按键排序。映射容器通常比unordered_map容器慢，以通过它们的键来访问各个元素，但是它们允许基于它们的顺序对子集进行直接迭代。 在该映射值地图可以直接通过使用其相应的键来访问括号运算符（（操作符[] ）。 映射通常如实施

```cpp
template < class Key,                                     // map::key_type
           class T,                                       // map::mapped_type
           class Compare = less<Key>,                     // map::key_compare
           class Alloc = allocator<pair<const Key,T> >    // map::allocator_type
           > class map;
```


#### multimap

### STL面试题汇总
#### STL的六大组件
STL的六大组件：容器，算法，迭代器，空间配置器，适配器，仿函数。

* 容器  
  各种数据结构，来存放数据，从实现角度来看是一种类模板；

* 算法  
  各种算法的封装，从实现角度来看是一种函数模板；

* 迭代器  
  是容器与算法之间的胶合剂，即泛型指针；

* 仿函数  
  一般函数指针可视为狭义的仿函数；

* 空间配置器  
  负责空间的配置与管理；

* 适配器（配接器）  
  用来修饰容器或仿函数的迭代器接口。通俗的讲，就是像改锥的接口，装上不同的适配器，才能装上不同的改锥头。

#### 容器分类介绍  
容器包含关联容器和顺序容器两大类。  
关联容器和顺序容器的根本不同在于：关联容器中的元素是按关键字来保存和访问的，而顺序容器中的元素则是按它们在容器中的位置来顺序保存和访问的。  
* 顺序容器有:    
vector、deque、list；  

* 关联容器有:    
按关键字有序保存：map、set、multimap、multiset；  
无序集合：unordered_map、unordered_multimap、unordered_set、unordered_multiset;     

#### 迭代器失效  
* 一、序列式容器(数组式容器)  
<u>对于序列式容器(如vector,deque)，序列式容器就是数组式容器</u>，删除当前的iterator会使后面所有元素的iterator都失效。  
vector,deque是一个顺序容器，在内存中是一块连续的内存(deque是分段连续)，当删除一个元素后，内存中的数据会发生移动，以保证数据的紧凑。所以删除一个数据后，其他数据的地址发生了变化，之前获取的迭代器根据原有的信息就访问不到正确的数据。  
所以为了防止vector迭代器失效，常用如下方法：

  ```cpp
  for (iter = cont.begin(); iter != cont.end();)
  {
    (*it)->doSomething();
    if (shouldDelete(*iter))
        iter = cont.erase(iter);  //erase删除元素，返回下一个迭代器
    else
        ++iter;
  }
  ```
  这样<u>删除iter指向的元素后，返回的是下一个元素的迭代器，这个迭代器是vector内存调整过后新的有效的迭代器。</u>

* 二、关联式容器  
对于关联容器(如map, set,multimap,multiset)，删除当前的iterator，仅仅会使当前的iterator失效，只要在erase时，递增当前iterator即可。这是因为map之类的容器，使用了红黑树来实现，插入、删除一个结点不会对其他结点造成影响。<u>erase迭代器只是被删元素的迭代器失效，但是返回值为void，所以要采用erase(iter++)的方式删除迭代器。</u>  
map是关联容器，以红黑树或者平衡二叉树组织数据，虽然删除了一个元素，整棵树也会调整，以符合红黑树或者二叉树的规范，但是单个节点在内存中的地址没有变化，变化的是各节点之间的指向关系。  
所以在map中为了防止迭代器失效，在有删除操作时，常用如下方法：
  ```cpp
  for (iter = dataMap.begin(); iter != dataMap.end(); )
  {
          int nKey = iter->first;
          string strValue = iter->second;

          if (nKey % 2 == 0)
          {
              dataMap.erase(iter++) 
              //这样也行
              // map<int, string>::iterator tmpIter = iter;
              // iter++;
              // dataMap.erase(tmpIter);

          }else
          {
              iter++;
          }
  }
  ```

* 三、链表式容器  
对于链表式容器(如list,forwordlist)，删除当前的iterator，仅仅会使当前的iterator失效，这是因为list之类的容器，使用了链表来实现，插入、删除一个结点不会对其他结点造成影响。<u>只要在erase时，递增当前iterator即可，并且erase方法可以返回下一个有效的iterator。</u>  
  * 方式一: 递增当前iterator
  ```cpp
  for (iter = cont.begin(); it != cont.end();)
  {
    (*iter)->doSomething();
    if (shouldDelete(*iter))
        cont.erase(iter++);
    else
        ++iter;
  }
  ```

  * 方式二: 通过erase获得下一个有效的iterator
  ```cpp
  for (iter = cont.begin(); iter != cont.end();)
  {
    (*it)->doSomething();
    if (shouldDelete(*iter))
        iter = cont.erase(iter);  //erase删除元素，返回下一个迭代器
    else
        ++iter;
  }
  ```

* 四、总结  

<u>迭代器失效分三种情况考虑，也是分三种数据结构考虑，分别为数组型，链表型，树型数据结构。</u>

  * 数组型数据结构：  
  该数据结构的元素是分配在连续的内存中，insert和erase操作，都会使得删除点和插入点之后的元素挪位置，所以，插入点和删除掉之后的迭代器全部失效，也就是说insert(*iter)(或erase(*iter))，然后在iter++，是没有意义的。解决方法：erase(*iter)的返回值是下一个有效迭代器的值。 iter =cont.erase(iter);

  * 链表型数据结构：  
  对于list型的数据结构，使用了不连续分配的内存，删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器.解决办法两种，erase(*iter)会返回下一个有效迭代器的值，或者erase(iter++).

  * 树形数据结构：  
  使用红黑树来存储数据，插入不会使得任何迭代器失效；删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器。erase迭代器只是被删元素的迭代器失效，但是返回值为void，所以要采用erase(iter++)的方式删除迭代器。
  

注意：经过erase(iter)之后的迭代器完全失效，该迭代器iter不能参与任何运算，包括iter++,*ite


[STL面试知识点](https://zhuanlan.zhihu.com/p/139892287)  
[C++迭代器失效的几种情况总结](https://www.cnblogs.com/fnlingnzb-learner/p/9300073.html)  
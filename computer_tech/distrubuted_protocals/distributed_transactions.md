
<!-- TOC -->

- [分布式事务](#分布式事务)
  - [1. 基础概念](#1-基础概念)
    - [1.1 什么是事务](#11-什么是事务)
    - [1.2 本地事务](#12-本地事务)
    - [1.3 分布式事务](#13-分布式事务)
    - [1.4 分布式事务产生的情景](#14-分布式事务产生的情景)
  - [2. 分布式事务基础理论](#2-分布式事务基础理论)
    - [2.1 CAP理论](#21-cap理论)
      - [2.1.1 理解CAP](#211-理解cap)
      - [2.1.2 CAP组合方式](#212-cap组合方式)
      - [2.1.3 总结](#213-总结)
    - [2.2 BASE 理论](#22-base-理论)
  - [3、分布式事务解决方案之 2PC](#3分布式事务解决方案之-2pc)
    - [3.1 什么是 2PC](#31-什么是-2pc)
    - [3.2 解决方案](#32-解决方案)
      - [3.2.1 XA 方案](#321-xa-方案)
      - [3.2.2 Seata 方案](#322-seata-方案)
  - [4. 分布式事务解决方案之TCC](#4-分布式事务解决方案之tcc)
    - [4.1 什么是TCC事务](#41-什么是tcc事务)
    - [4.2 TCC 异常处理](#42-tcc-异常处理)
  - [5. 分布式事务解决方案之可靠消息最终一致性](#5-分布式事务解决方案之可靠消息最终一致性)
  - [6 分布式事务解决方案之最大努力通知](#6-分布式事务解决方案之最大努力通知)

<!-- /TOC -->

## 分布式事务  
### 1. 基础概念  
#### 1.1 什么是事务  
事务可以看做是一次大的活动，它由不同的小活动组成，这些活动要么全部成功，要么全部失败。  

#### 1.2 本地事务
在计算机系统中，更多的是通过关系型数据库来控制事务，这是利用数据库本身的事务特性来实现的，因此叫数据库事务，由于应用主要靠关系数据库来控制事务，而数据库通常和应用在同一个服务器，所以基于关系型数据库的事务又被称为本地事务。  

数据库事务的四大特性：ACID    
* A（Atomic）   
原子性，构成事务的所有操作，要么都执行完成，要么全部不执行，不可能出现部分成功部分失败的情况。  

* C（Consistency）  
一致性，在事务执行前后，数据库的一致性约束没有被破坏。比如：张三向李四转 100 元，转账前和转账后的数据是正确状态这叫一致性，如果出现张三转出 100 元，李四账户没有增加 100 元这就出现了数 据错误，就没有达到一致性。  

* I（Isolation）  
隔离性，数据库中的事务一般都是并发的，隔离性是指并发的两个事务的执行互不干扰，一个事务不能看到其他事务的运行过程的中间状态。通过配置事务隔离级别可以比避免脏读、重复读问题。  

* D（Durability）  
持久性，事务完成之后，该事务对数据的更改会持久到数据库，且不会被回滚。  

数据库事务在实现时会将一次事务的所有操作全部纳入到一个不可分割的执行单元，该执行单元的所有操作要么都成功，要么都失败，只要其中任一操作执行失败，都将导致整个事务的回滚。

#### 1.3 分布式事务  
随着互联网的快速发展，软件系统由原来的单体应用转变为分布式应用。   
分布式系统会把一个应用系统拆分为可独立部署的多个服务，因此需要服务与服务之间远程协作才能完成事务操作，这种分布式系统环境下由不同的服务之间通过网络远程协作完成事务称之为分布式事务，例如用户注册送积分事务、创建订单减库存事务，银行转账事务等都是分布式事务。

我们知道本地事务依赖数据库本身提供的事务特性来实现，因此以下逻辑可以控制本地事务：
```
begin transaction；
    //1.本地数据库操作：张三减少金额
    //2.本地数据库操作：李四增加金额
commit transation;
```
但是在分布式环境下，会变成下边这样：
```
begin transaction；
    //1.本地数据库操作：张三减少金额
    //2.远程调用：让李四增加金额
commit transation;
```
​可以设想，当远程调用让李四增加金额成功了，由于网络问题远程调用并没有返回，此时本地事务提交失败就回滚了张三减少金额的操作，此时张三和李四的数据就不一致了。    

因此在分布式架构的基础上，传统数据库事务就无法使用了，张三和李四的账户不在一个数据库中甚至不在一个应用系统里，实现转账事务需要通过远程调用，由于网络问题就会导致分布式事务问题。

#### 1.4 分布式事务产生的情景
1、跨JVM进程产生分布式事务  
2、跨数据库实例产生分布式事务  
3、多服务访问同一个数据库实例  

### 2. 分布式事务基础理论
通过前面的学习，我们了解到了分布式事务的基础概念。与本地事务不同的是，分布式系统之所以叫分布式，是因为提供服务的各个节点分布在不同机器上，相互之间通过网络交互。不能因为有一点网络问题就导致整个系统无法提供服务，网络因素成为了分布式事务的考量标准之一。因此，分布式事务需要更进一步的理论支持，接下来，我们先来学习一下分布式事务的CAP理论。  

#### 2.1 CAP理论  
##### 2.1.1 理解CAP  
CAP 是 Consistency、Availability、Partition tolerance 三个单词的缩写，分别表示一致性、可用性、分区容忍性。

##### 2.1.2 CAP组合方式  
在所有分布式事务场景中不会同时具备 CAP 三个特性，因为在具备了P的前提下C和A是不能共存的。  
所以在生产中对分布式事务处理时要根据需求来确定满足 CAP 的哪两个方面。
* AP  
放弃一致性，追求分区容忍性和可用性。这是很多分布式系统设计时的选择。
通常实现 AP 都会保证最终一致性，后面将的 BASE 理论就是根据 AP 来扩展的，一些业务场景比如：订单退款，今日退款成功，明日账户到账，只要用户可以接受在一定的时间内到账即可。

* CP  
放弃可用性，追求一致性和分区容错性，zookeeper 其实就是追求的强一致，又比如跨行转账，一次转账请求要等待双方银行系统都完成整个事务才算完成。

* CA  
放弃分区容忍性，即不进行分区，不考虑由于网络不通或结点挂掉的问题，则可以实现一致性和可用性。那么系统将不是一个标准的分布式系统，最常用的关系型数据就满足了 CA。

##### 2.1.3 总结
CAP 是一个已经被证实的理论，一个分布式系统最多只能同时满足：一致性（Consistency）、可用性（Availability）和分区容忍性（Partition tolerance）这三项中的两项。它可以作为我们进行架构设计、技术选型的考量标准。对于多数大型互联网应用的场景，结点众多、部署分散，而且现在的集群规模越来越大，所以节点故障、网络故障是常态，而且要保证服务可用性达到 N 个 9（99.99..%），并要达到良好的响应性能来提高用户体验，因此一般都会做出如下选择：保证 P 和 A ，舍弃 C 强一致，保证最终一致性。  


#### 2.2 BASE 理论
1. 强一致性和最终一致性  
CAP 理论告诉我们一个分布式系统最多只能同时满足一致性（Consistency）、可用性（Availability）和分区容忍性（Partition tolerance）这三项中的两项，其中AP在实际应用中较多，AP 即舍弃一致性，保证可用性和分区容忍性，但是在实际生产中很多场景都要实现一致性，比如前边我们举的例子主数据库向从数据库同步数据，即使不要一致性，但是最终也要将数据同步成功来保证数据一致，这种一致性和 CAP 中的一致性不同，CAP 中的一致性要求 在任何时间查询每个结点数据都必须一致，它强调的是强一致性，但是最终一致性是允许可以在一段时间内每个结点的数据不一致，但是经过一段时间每个结点的数据必须一致，它强调的是最终数据的一致性。

2. Base 理论介绍  
BASE 是 Basically Available（基本可用）、Soft state（软状态）和 Eventually consistent （最终一致性）三个短语的缩写。BASE 理论是对 CAP 中 AP 的一个扩展，通过牺牲强一致性来获得可用性，当出现故障允许部分不可用但要保证核心功能可用，允许数据在一段时间内是不一致的，但最终达到一致状态。满足BASE理论的事务，我们称之为“柔性事务”。  

3. 基本可用
分布式系统在出现故障时，允许损失部分可用功能，保证核心功能可用。如电商网站交易付款出现问题了，商品依然可以正常浏览。

4. 软状态  
由于不要求强一致性，所以BASE允许系统中存在中间状态（也叫软状态），这个状态不影响系统可用性，如订单的"支付中"、“数据同步中”等状态，待数据最终一致后状态改为“成功”状态。

5. 最终一致
最终一致是指经过一段时间后，所有节点数据都将会达到一致。如订单的"支付中"状态，最终会变 为“支付成功”或者"支付失败"，使订单状态与实际交易结果达成一致，但需要一定时间的延迟、等待。

### 3、分布式事务解决方案之 2PC
前面学习了分布式事务的基础理论，以理论为基础，针对不同的分布式场景业界常见的解决方案有 2PC、3PC、TCC、可靠消息最终一致性、最大努力通知这几种。  

#### 3.1 什么是 2PC  
2PC 即两阶段提交协议，是将整个事务流程分为两个阶段，准备阶段（Prepare phase）、提交阶段（commit phase），2 是指两个阶段，P 是指准备阶段，C 是指提交阶段。

举例：张三和李四好久不见，老友约起聚餐，饭店老板要求先买单，才能出票。这时张三和李四分别抱怨近况不如意，囊中羞涩，都不愿意请客，这时只能AA。只有张三和李四都付款，老板才能出票安排就餐。但由于张三和李四都是铁公鸡，形成了尴尬的一幕：

准备阶段：老板要求张三付款，张三付款。老板要求李四付款，李四付款。

提交阶段：老板出票，两人拿票纷纷落座就餐。

例子中形成了一个事务，若张三或李四其中一人拒绝付款，或钱不够，店老板都不会给出票，并且会把已收款退回。

整个事务过程由事务管理器和参与者组成，店老板就是事务管理器，张三、李四就是事务参与者，事务管理器负责决策整个分布式事务的提交和回滚，事务参与者负责自己本地事务的提交和回滚。

#### 3.2 解决方案
##### 3.2.1 XA 方案  
##### 3.2.2 Seata 方案  

### 4. 分布式事务解决方案之TCC    
#### 4.1 什么是TCC事务  
TCC 是 Try、Conﬁrm、Cancel 三个词语的缩写，TCC 要求每个分支事务实现三个操作：预处理 Try、确认 Conﬁrm、撤销 Cancel。Try 操作做业务检查及资源预留，Conﬁrm 做业务确认操作，Cancel 实现一个与 Try 相反的操作即回滚操作。TM 首先发起所有的分支事务的 Try 操作，任何一个分支事务的Try操作执行失败，TM 将会发起所有分支事务的 Cancel 操作，若 Try 操作全部成功，TM 将会发起所有分支事务的 Conﬁrm 操作，其中 Conﬁrm/Cancel 操作若执行失败，TM 会进行重试。  

TCC 分为三个阶段：  
1. Try 阶段是做完业务检查（一致性）及资源预留（隔离），此阶段仅是一个初步操作，它和后续的 Conﬁrm 一起才能真正构成一个完整的业务逻辑。    
2. Confirm 阶段是做确认提交，Try 阶段所有分支事务执行成功后开始执行 Conﬁrm。通常情况下，采用 TCC 则认为 Conﬁrm 阶段是不会出错的。即：只要 Try 成功，Conﬁrm 一定成功。若 Conﬁrm 阶段真的出错了，需引入重试机制或人工处理。  
3. Cancel 阶段是在业务执行错误需要回滚的状态下执行分支事务的业务取消，预留资源释放。通常情况下，采用 TCC 则认为 Cancel 阶段也是一定成功的。若 Cancel 阶段真的出错了，需引入重试机制或人工处理。  

#### 4.2 TCC 异常处理  
TCC需要注意三种异常处理分别是空回滚、幂等、悬挂
* 空回滚  
在没有调用 TCC 资源 Try 方法的情况下，调用了二阶段的 Cancel 方法，Cancel 方法需要识别出这是一个空回滚，然后直接返回成功。
出现原因是当一个分支事务所在服务宕机或网络异常，分支事务调用记录为失败，这个时候其实是没有执行 Try 阶段，当故障恢复后，分布式事务进行回滚则会调用二阶段的 Cancel 方法，从而形成空回滚。  
解决思路是关键就是要识别出这个空回滚。思路很简单就是需要知道一阶段是否执行，如果执行了，那就是正常回滚；如果没执行，那就是空回滚。前面已经说过 TM 在发起全局事务时生成全局事务记录，全局事务 ID 贯穿整个分布式事务调用链条。再额外增加一张分支事务记录表，其中有全局事务 ID 和分支事务 ID，第一阶段 Try 方法里会插入一条记录，表示一阶段执行了。Cancel 接口里读取该记录，如果该记录存在，则正常回滚；如果该记录不存在，则是空回滚。

* 幂等   
通过前面介绍已经了解到，为了保证 TCC 二阶段提交重试机制不会引发数据不一致，要求 TCC 的二阶段 Try、Conﬁrm 和 Cancel 接口保证幂等，这样不会重复使用或者释放资源。如果幂等控制没有做好，很有可能导致数据不一致等严重问题。
解决思路在上述"分支事务记录"中增加执行状态，每次执行前都查询该状态。

* 悬挂  
悬挂就是对于一个分布式事务，其二阶段 Cancel 接口比 Try 接口先执行。
出现原因是在 RPC 调用分支事务 Try 时，先注册分支事务，再执行 RPC 调用，如果此时 RPC 调用的网络发生拥堵，通常 RPC 调用是有超时时间的，RPC 超时以后，TM 就会通知 RM 回滚该分布式事务，可能回滚完成后，RPC 请求才到达参与者真正执行，而一个 Try 方法预留的业务资源，只有该分布式事务才能使用，该分布式事务第一阶段预留的业务资源就再也没有人能够处理了，对于这种情况，我们就称为悬挂，即业务资源预留后没法继续处理。
解决思路是如果二阶段执行完成，那一阶段就不能再继续执行。在执行一阶段事务时判断在该全局事务下，"分支事务记录"表中是否已经有二阶段事务记录，如果有则不执行 Try。


### 5. 分布式事务解决方案之可靠消息最终一致性  

### 6 分布式事务解决方案之最大努力通知  


[分布式事务有这一篇就够了！](https://zhuanlan.zhihu.com/p/263555694)
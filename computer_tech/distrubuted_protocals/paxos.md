

## 一、Paxos算法背景
Paxos算法是Lamport宗师提出的一种基于消息传递的分布式一致性算法，使其获得2013年图灵奖。
Paxos由Lamport于1998年在《The Part-Time Parliament》论文中首次公开
后来在2001年，Lamport觉得同行不能理解他的幽默感，于是重新发表了朴实的算法描述版本《Paxos Made Simple》。
**自Paxos问世以来就持续垄断了分布式一致性算法，Paxos这个名词几乎等同于分布式一致性。Google的很多大型分布式系统都采用了Paxos算法来解决分布式一致性问题，如Chubby、Megastore以及Spanner等。开源的ZooKeeper，以及MySQL 5.7推出的用来取代传统的主从复制的MySQL Group Replication等纷纷采用Paxos算法解决分布式一致性问题。**

然而，Paxos的最大特点就是难，不仅难以理解，更难以实现。

[Paxos Made Simple](https://lamport.azurewebsites.net/pubs/paxos-simple.pdf)
[Paxos算法详解](https://zhuanlan.zhihu.com/p/31780743)
## Introduction
## 重要设计

### NVMe 命令 
NVMe 有两种命令，一种叫做 Admin Command, 用来对 Host 进行管理以及控制 SSD；
另外就是 I/O Command，用来处理 Host 和 SSD 之间的数据传输.

### NVMe重要部件
NVMe 有三种特殊的部件，  
Submission Queue（SQ）提交队列，  
Completion Queue（CQ）完成队列，  
Doorbell Register（DB）门铃寄存器。  
SQ 和 CQ 位于 Host 的内存中，DB 位于 SSD 的内部。

### NVMe读写流程
NVMe的整个流程很简单，加起来就是八个步骤：
```
第一步：Host 写入命令到 SQ 中；
第二步：Host 写 DB 寄存器通知 SSD 来取命令
第三步：SSD 收到通知，从 SQ 中取命令
第四步：SSD 执行命令
第五步：指令执行完成，SSD 往 CQ 中写入指令完成结果
第六步：SSD 通过中断通知 Host 指令执行完毕
第七步：Host 收到通知，处理 CQ 队列中的命令完成状态
第八步：Host 处理完毕命令状态，通过 DB 回复 SSD 整个指令处理完毕，结束
```
![picture 2](../../z_images/5f9d6f0196359da1c072d3ea71f4ac3a9e11cb639b9c7ade4c2cde857e8d863b.png)  


### NVMe 的三大部件详解
SQ 和 CQ 与 Command 一样是区分 Admin 域和 I/O 域的，这很好理解，放 Admin Command 的就是 Admin SQ 和 Admin CQ，放 I/O Command 的就是 I/O SQ 和 I/O CQ，这也是一种对应关系，NVMe 协议规定不允许将 I/O Command 放到 Admin SQ 中，反之亦然，不然后果不可预期。
**另外值得关注的是，I/O SQ/CQ 是可以存在多个队列对的，而 Admin SQ/CQ 仅仅只有一对（很好理解，管理层自然很少，干活的员工自然要多）。**  
**一般来说，除了 Admin SQ/CQ 为唯一的一对之外，I/O 的 CQ 建议为每一个 Core 提供一个，I/O 的 SQ 为每一个 Core 提供多个。为什么每一个 Core 提供一个 CQ 就行了，而 SQ 却最好多个呢？一个是为了性能，每一个 CPU 都可能存在多个流水线，那么可以满足性能最大化，此外就是为了 QoS，可以为不同的 I/O 命令提供不同的 I/O 优先级，这样可以保证高优先级的 I/O 更快的完成。**


[NVMe 设计逻辑与原理解析](https://www.byteisland.com/nvme-%e8%ae%be%e8%ae%a1%e9%80%bb%e8%be%91%e4%b8%8e%e5%8e%9f%e7%90%86%e8%a7%a3%e6%9e%90/)
[深入理解nvme hardware queue pair](https://blog.51cto.com/xiamachao/2380622)
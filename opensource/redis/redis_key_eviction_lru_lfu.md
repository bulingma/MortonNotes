
### Redis缓存淘汰策略
Redis 4.0 之前一共实现了 6 种内存淘汰策略，在 4.0 之后，又增加了 2 种策略。  
<u>我们可以按照是否会进行数据淘汰把它们分成两类：</u>

* 不进行数据淘汰的策略：  
  只有 noeviction 这一种。  
* 会进行淘汰的 7 种其他策略。<u>会进行淘汰的 7 种策略，我们可以再进一步根据淘汰候选数据集的范围把它们分成两类：</u>  
  * 在设置了过期时间的数据中进行淘汰：
    包括 volatile-random、volatile-ttl、volatile-lru、volatile-lfu（Redis 4.0 后新增）四种。
  * 在所有数据范围内进行淘汰：
    包括 allkeys-lru、allkeys-random、allkeys-lfu（Redis 4.0 后新增）三种。

* 7 种淘汰策略含义  
volatile-ttl：表示在设置可过期时间的键值对中，根据过期时间的先后进行淘汰数据，越早被过期的数据，越先被淘汰。  
volatile-random：从名字可以看出来，就是在设置了过期时间的键值对中，随机淘汰数据。  
volatile-lfu 会根据 lfu 算法进行数据的淘汰    
volatile-lru：会根据 lru 算法进行数据的淘汰    
allkeys-random：在全部的键值对数据中，进行数据的随机淘汰。  
allkeys-lru：在全部的键值对数据中，根据 lru 算法进行数据的淘汰。  
allkeys-lfu：在全部的键值对数据中，根据 lfu 算法进行数据的淘汰。  



### LRU——最近最少使用（时效性）

### LFU——最近访问次数最少（访问频次）


### 参考
[Redis 缓存淘汰策略](https://zhuanlan.zhihu.com/p/343963744)
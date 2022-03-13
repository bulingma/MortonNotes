
<!-- TOC -->

- [1. spdk 用户态页表（可管理256TB）](#1-spdk-用户态页表可管理256tb)
	- [1.0. 总结](#10-总结)
	- [1.1. spdk用户态页表基础](#11-spdk用户态页表基础)
	- [1.2. spdk 对于用户态内存页表的管理](#12-spdk-对于用户态内存页表的管理)
	- [1.3. 内部实现](#13-内部实现)
		- [1.3.1. spdk_mem_map_alloc实现](#131-spdk_mem_map_alloc实现)
		- [1.3.2. spdk_mem_register实现](#132-spdk_mem_register实现)
		- [1.3.3. sdpk的三个默认g_spdk_mem_maps](#133-sdpk的三个默认g_spdk_mem_maps)
	- [1.4. benchmarks-性能](#14-benchmarks-性能)
	- [1.5. spdk_mem_register 调用链](#15-spdk_mem_register-调用链)
- [2. 参考链接](#2-参考链接)

<!-- /TOC -->

### 1. spdk 用户态页表（可管理256TB）

#### 1.0. 总结
0. spdk18.10提供了一个spdk_mem_map_alloc函数，该函数的作用是：Allocate a virtual memory address translation map. 
1. spdk_mem_map_alloc 可以注册多个自定义的钩子，可以作用于整个 spdk 的已经注册的内存；
2. spdk_mem_map_alloc 或者 spdk_mem_register 的先后顺序可以无关：

- spdk_mem_register 会作用于之前注册的钩子；

- spdk_mem_map_alloc 也会对已经注册的内存执行一遍钩子函数；
  这两个函数协作，可以完成一些非常有意义的事情。
  例如在操作 RDMA 的时候，内存需要注册为 MR，那么就可以调用 spdk_mem_map_alloc 来达到目的。

  ——<u>vhost-user 来自 qemu的内存经过 spdk_mem_register 来注册到 spdk 的用户态内存页表管理中，从而实现qemu内存到vhost内存的zero copy。</u>

  

#### 1.1. spdk用户态页表基础

spdk 建立了一套用户态的 3级页表，<u>**来方便在用户态进行虚拟地址到物理地址的转化。**</u>
3级页表是：256TiB-> 1GiB -> 2MiB

- 第三级页表：[30..47]
- 第二级页表：[21..29]
- 最小的页是2M，偏移范围是[0..20]

```
一级页表容纳 2MB 的地址空间，那么需要占用  2^21, 也就是 21 bits 的地址位。也就是 [0..20];
二级页表容纳 1GB 的地址空间，那么需要占用 1G/2M = 2^9, 也就是 9 bits 的地址位。也就是 [21..29]。
三级页表容纳 256TB 的地址空间，那么需要占用 256 T/1G = 2^18, 也就是 18 bits 的地址位。也就是[30..47]。
```

<u>dpdk 的内存，以及非dpdk 的内存，例如 vhost-user 来自 qemu的内存都需要经过 spdk_mem_register 来注册到 spdk 的用户态内存页表管理中。</u>



#### 1.2. spdk 对于用户态内存页表的管理

1. g_mem_reg_map 管理256TB 的三级页表；

2. 通过 spdk_mem_map_alloc 注册hook 回调，同时也会调用已经注册（spdk_mem_register）的回调；

3. spdk_mem_register 注册新的虚拟内存到 g_mem_reg_map 上，并且触发所有的回调；

4. 初始化的时候，dpdk 的内存需要调用 spdk_mem_register 注册到 g_mem_reg_map 上。

   

#### 1.3. 内部实现

##### 1.3.1. spdk_mem_map_alloc实现

```cpp
// file: spdk/lib/env_dpdk/memory.c
struct spdk_mem_map *
spdk_mem_map_alloc(uint64_t default_translation, const struct spdk_mem_map_ops *ops, void *cb_ctx)
{
	struct spdk_mem_map *map;
	...
	if (ops && ops->notify_cb) {
		pthread_mutex_lock(&g_spdk_mem_map_mutex);
    // 遍历g_mem_reg_map(主内存注册map表)上注册的内存段，回调新map的notify回调函数。
		rc = mem_map_notify_walk(map, SPDK_MEM_MAP_NOTIFY_REGISTER);
		if (rc != 0) {
			pthread_mutex_unlock(&g_spdk_mem_map_mutex);
			DEBUG_PRINT("Initial mem_map notify failed\n");
			pthread_mutex_destroy(&map->mutex);
			free(map);
			return NULL;
		}
    // 此处会把新map对象插入g_spdk_mem_maps全局链表中（当有用户调用spdk_mem_register
    // 注册内存时，会遍历该链表，触发所有用户的回调，完成notify通知工作）
		TAILQ_INSERT_TAIL(&g_spdk_mem_maps, map, tailq);
		pthread_mutex_unlock(&g_spdk_mem_map_mutex);
	}

	return map;
}
```

spdk_mem_map_alloc 会添加 map 到 g_spdk_mem_maps 里面，且会调用 spdk_mem_map_notify_walk(xxx);
spdk_mem_map_notify_walk 的作用是，找到已经在主map 上注册的内存段，然后调用 *新* map 的 notify 回调。

##### 1.3.2. spdk_mem_register实现

```cpp
// file: spdk/lib/env_dpdk/memory.c
int
spdk_mem_register(void *vaddr, size_t len)
{
  	struct spdk_mem_map *map;
  	...
  	seg_vaddr = vaddr;
    seg_len = len;
  	// 下面的循环翻译虚拟地址对应的页表项是否已经设置（registered）。如果已经注册，那么就返回 EBUSY。
    while (seg_len > 0) {
      reg = spdk_mem_map_translate(g_mem_reg_map, (uint64_t)seg_vaddr, NULL);
      if (reg & REG_MAP_REGISTERED) {
        pthread_mutex_unlock(&g_spdk_mem_map_mutex);
        fprintf(stderr, "address already registered, vaddr=%p len=%ju\n", seg_vaddr, seg_len);
        return -EBUSY;
      }
      seg_vaddr += VALUE_2MB;
      seg_len -= VALUE_2MB;
    }
  
  	// 下面的循环只是设置对应的页表项为 REG_MAP_REGISTERED，表明已经注册了此段虚拟地址空间。
  	seg_vaddr = vaddr;
    seg_len = 0;
    while (len > 0) {
      spdk_mem_map_set_translation(g_mem_reg_map, (uint64_t)vaddr, VALUE_2MB,
                 seg_len == 0 ? REG_MAP_REGISTERED | REG_MAP_NOTIFY_START : REG_MAP_REGISTERED);
      seg_len += VALUE_2MB;
      vaddr += VALUE_2MB;
      len -= VALUE_2MB;
    }
  
  	// 遍历g_spdk_mem_maps中每一个mem_map表（订阅项）,notify回调通知新内存的注册事件和内存信息
  	TAILQ_FOREACH(map, &g_spdk_mem_maps, tailq) {
		rc = map->ops.notify_cb(map->cb_ctx, map, SPDK_MEM_MAP_NOTIFY_REGISTER, seg_vaddr, seg_len);
		if (rc != 0) {
			pthread_mutex_unlock(&g_spdk_mem_map_mutex);
			fprintf(stderr, "notify_cb failed, vaddr=%p len=%ju\n", seg_vaddr, seg_len);
			return rc;
		}
	}
  
  ...
}

// spdk_mem_map_set_translation 核心逻辑如下，遍历每一个 2GB，然后设置 map_2mb->translation_2mb = translation; 后面可以通过 spdk_mem_map_translate 来翻译地址。
int
spdk_mem_map_set_translation(struct spdk_mem_map *map, uint64_t vaddr, uint64_t size,
			     uint64_t translation)
{
  ...
	while (size) {
		map_1gb = mem_map_get_map_1gb(map, vfn_2mb);
		if (!map_1gb) {
			DEBUG_PRINT("could not get %p map\n", (void *)vaddr);
			return -ENOMEM;
		}

		idx_1gb = MAP_1GB_IDX(vfn_2mb);
		map_2mb = &map_1gb->map[idx_1gb];
		map_2mb->translation_2mb = translation;

		size -= VALUE_2MB;
		vfn_2mb++;
	}	
  ...
}
```



##### 1.3.3. sdpk的三个默认g_spdk_mem_maps

spdk 默认有如下的3个 g_spdk_mem_maps：

1. vtophys_init func

   ```
   // vtophys_init
   g_phys_ref_map = spdk_mem_map_alloc(0, &phys_ref_map_ops, NULL);
   
   g_vtophys_map = spdk_mem_map_alloc(SPDK_VTOPHYS_ERROR, &vtophys_map_ops, NULL);
   ```

2. mem_map_init

   ```
   g_mem_reg_map = spdk_mem_map_alloc(0, NULL, NULL);
   ```

   

* g_mem_reg_map

  g_mem_reg_map是全局的一个内存 map，相当于位图的概念，标记一个区间已经注册或者未注册。

* g_vtophys_map 

  g_vtophys_map是用来做地址翻译的，用户可以根据自己的需求注册一个自定义的 map 即可，注册一个 notify_cb 进来，执行自己想执行的操作。

* g_phys_ref_map

#### 1.4. benchmarks-性能

下面是写的一个测试 spdk 注册内存的benchmark，测试注册 100G 内存消耗的时间—17s左右。
结果如下：

```cpp
./memory_register 100 time spent: 16.930000 s, memory register: 100 G, performance: 169.300000 ms/GB 
```



#### 1.5. spdk_mem_register 调用链

```cpp
int
spdk_env_init(const struct spdk_env_opts *opts)
{
	rc = rte_eal_init(g_eal_cmdline_argcount, dpdk_args);
	...
	
	legacy_mem = false;
	if (opts->env_context && strstr(opts->env_context, "--legacy-mem") != NULL) {
		legacy_mem = true;
	}
	rc = spdk_env_dpdk_post_init(legacy_mem);
}

spdk_env_dpdk_post_init(bool legacy_mem)——>
mem_map_init(legacy_mem)
rte_memseg_contig_walk(memory_iter_cb, NULL)——>
memory_iter_cb

int
mem_map_init(bool legacy_mem)
{
	g_legacy_mem = legacy_mem;

	// 全局主map初始化
	g_mem_reg_map = spdk_mem_map_alloc(0, NULL, NULL);
	if (g_mem_reg_map == NULL) {
		DEBUG_PRINT("memory registration map allocation failed\n");
		return -ENOMEM;
	}

	/*
	 * Walk all DPDK memory segments and register them
	 * with the main memory map
	 */
	rte_mem_event_callback_register("spdk", memory_hotplug_cb, NULL);
	rte_memseg_contig_walk(memory_iter_cb, NULL);
	return 0;
}
```



### 2. 参考链接

[spdk 建立的用户态页表.  




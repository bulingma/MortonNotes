
<!-- TOC -->

- [1、virtqueue资料](#1virtqueue资料)
- [2、spdk virtue_ring分析](#2spdk-virtue_ring分析)

<!-- /TOC -->

## 1、virtqueue资料

[virtqueue想法提出者 Rusty Russell 的论文](https://ozlabs.org/~rusty/virtio-spec/virtio-paper.pdf)


## 2、spdk virtue_ring分析

* 文件: spdk/dpdk/drivers/net/virtio/virtio_ring.h    

```
struct vring_desc {
	uint64_t addr;  /*  Address (guest-physical). */
	uint32_t len;   /* Length. */
	uint16_t flags; /* The flags as indicated above. */
	uint16_t next;  /* We chain unused descriptors via this. */
};

struct vring_avail {
	uint16_t flags;
	uint16_t idx;
	uint16_t ring[0];
};

struct vring_used {
	uint16_t flags;
	uint16_t idx;
	struct vring_used_elem ring[0];
};

struct vring_packed_desc {
	uint64_t addr;
	uint32_t len;
	uint16_t id;
	uint16_t flags;
};

struct vring_packed_desc_event {
	uint16_t desc_event_off_wrap;
	uint16_t desc_event_flags;
};

struct vring_packed {
	unsigned int num;
	struct vring_packed_desc *desc;
	struct vring_packed_desc_event *driver;
	struct vring_packed_desc_event *device;
};

struct vring {
	// virtqueue包括三部分，分别是描述符表，可用环（buffer），已用环（buffer）
	unsigned int num;
	struct vring_desc  *desc;	// Descriptor Table
	struct vring_avail *avail;// Available Ring
	struct vring_used  *used; // Used Ring
};
```

* 文件： virtio_rxtx.h  

```
struct virtnet_rx {
	struct virtqueue *vq;
	/* dummy mbuf, for wraparound when processing RX ring. */
	struct rte_mbuf fake_mbuf;
	uint64_t mbuf_initializer; /**< value to init mbufs. */
	struct rte_mempool *mpool; /**< mempool for mbuf allocation */

	uint16_t queue_id;   /**< DPDK queue index. */
	uint16_t port_id;     /**< Device port identifier. */

	/* Statistics */
	struct virtnet_stats stats;

	const struct rte_memzone *mz; /**< mem zone to populate RX ring. */
};

struct virtnet_tx {
	struct virtqueue *vq;
	/**< memzone to populate hdr. */
	const struct rte_memzone *virtio_net_hdr_mz;
	rte_iova_t virtio_net_hdr_mem;   /**< hdr for each xmit packet */

	uint16_t    queue_id;            /**< DPDK queue index. */
	uint16_t    port_id;             /**< Device port identifier. */

	/* Statistics */
	struct virtnet_stats stats;

	const struct rte_memzone *mz;    /**< mem zone to populate TX ring. */
};

struct virtnet_ctl {
	struct virtqueue *vq;
	/**< memzone to populate hdr. */
	const struct rte_memzone *virtio_net_hdr_mz;
	rte_iova_t virtio_net_hdr_mem;  /**< hdr for each xmit packet */
	uint16_t port_id;               /**< Device port identifier. */
	const struct rte_memzone *mz;   /**< mem zone to populate CTL ring. */
	rte_spinlock_t lock;              /**< spinlock for control queue. */
};
```

* 文件：spdk/dpdk/drivers/net/virtio/virtio_pci.h  
```
struct virtio_hw {
	struct virtnet_ctl *cvq;
	uint64_t    req_guest_features;
	uint64_t    guest_features;
	uint32_t    max_queue_pairs;
	bool        started;
	uint16_t	max_mtu;
	uint16_t    vtnet_hdr_size;
	uint8_t	    vlan_strip;
	uint8_t	    use_msix;
	uint8_t     modern;
	uint8_t     use_vec_rx;
	uint8_t     use_vec_tx;
	uint8_t     use_inorder_rx;
	uint8_t     use_inorder_tx;
	uint8_t     weak_barriers;
	bool        has_tx_offload;
	bool        has_rx_offload;
	uint16_t    port_id;
	uint8_t     mac_addr[RTE_ETHER_ADDR_LEN];
	uint32_t    notify_off_multiplier;
	uint32_t    speed;  /* link speed in MB */
	uint8_t     duplex;
	uint8_t     *isr;
	uint16_t    *notify_base;
	struct virtio_pci_common_cfg *common_cfg;
	struct virtio_net_config *dev_cfg;
	void	    *virtio_user_dev;
	/*
	 * App management thread and virtio interrupt handler thread
	 * both can change device state, this lock is meant to avoid
	 * such a contention.
	 */
	rte_spinlock_t state_lock;
	struct rte_mbuf **inject_pkts;
	bool        opened;

	struct virtqueue **vqs;
};
```

* 文件 spdk/dpdk/drivers/net/virtio/virtqueue.h 
```
struct vq_desc_extra {
	void *cookie;
	uint16_t ndescs;
	uint16_t next;
};

struct vring_packed {
	unsigned int num;
	struct vring_packed_desc *desc;
	struct vring_packed_desc_event *driver;
	struct vring_packed_desc_event *device;
};

struct virtqueue {
	struct virtio_hw  *hw; /**< virtio_hw structure pointer. */
	union {
		struct {
      /**< vring keeping desc, used and avail */
			struct vring ring;   /*vring定义在/dpdk/drivers/net/virtio/virtio_ring.h中*/
		} vq_split;

		struct {
			/**< vring keeping descs and events */
			struct vring_packed ring;
			bool used_wrap_counter;
			uint16_t cached_flags; /**< cached flags for descs */
			uint16_t event_flags_shadow;
		} vq_packed;
	};

	uint16_t vq_used_cons_idx; /**< last consumed descriptor */
	uint16_t vq_nentries;  /**< vring desc numbers */
	uint16_t vq_free_cnt;  /**< num of desc available */
	uint16_t vq_avail_idx; /**< sync until needed */
	uint16_t vq_free_thresh; /**< free threshold */

	void *vq_ring_virt_mem;  /**< linear address of vring*/
	unsigned int vq_ring_size;

	union {
		struct virtnet_rx rxq;
		struct virtnet_tx txq;
		struct virtnet_ctl cq;
	};

    /** typedef uint64_t rte_iova_t; **/
	rte_iova_t vq_ring_mem; /**< physical address of vring,
	                         * or virtual address for virtio_user. */

	/**
	 * Head of the free chain in the descriptor table. If
	 * there are no free descriptors, this will be set to
	 * VQ_RING_DESC_CHAIN_END.
	 */
	uint16_t  vq_desc_head_idx;
	uint16_t  vq_desc_tail_idx;
	uint16_t  vq_queue_index;   /**< PCI queue index */
	uint16_t offset; /**< relative offset to obtain addr in mbuf */
	uint16_t  *notify_addr;
	struct rte_mbuf **sw_ring;  /**< RX software ring. */
	struct vq_desc_extra vq_descx[0];
};
```

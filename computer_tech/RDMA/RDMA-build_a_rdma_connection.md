## RDMA链接建立
### 方式1、tcp协商模式
* 思路
polling线程建链时，将建立qp链接的任务交给tcp协商线程，tcp协商线程负责链接建立，建立好后给polling线程做相应的赋值操作，polling线程即可获取到一个可用的rdma qp链接。

* Demo

* 建链方式
模拟tcp建立链接的三次握手方式，tcp线程及socket链接在此不做描述，以下重点描述建立链接中的重点任务
```
struct RdmaHandShakeInfoV0 {
  RdmaHandShakeHeader header;
  uint16_t magic = kRdmaHandShakeV0Magic;
  uint16_t lid = 0;
  uint16_t peer_lid = 0;
  uint32_t qp_num = 0;
  uint32_t peer_qp_num = 0;
  union ibv_gid gid;
  union ibv_gid peer_gid;
  bool is_setup_sync() { return header.flag == kSetupSync; }
  bool is_setup_sync_ack() { return header.flag == (kSetupSync | kSetupAck); }
  bool is_setup_ack() { return header.flag == kSetupAck; }
} __attribute__((packed));
```

建链的过程就是交换双方的qp_num, lid, gid三个信息
client                                  server
(qp_num, lid, gid)                      (peer_qp_num, peer_lid, peer_gid)

1、client发送（self_info.header.flag = kSetupSync && qp_num, lid, gid）信息到server;
2、server收到client发来的如上信息，保存后，回包（self_info.header.flag = kSetupSync | kSetupAck && peer_qp_num, peer_lid, peer_gid）给client，此时ModifyQpToRts状态到IBV_QPS_RTS
3、client收到server的 kSetupSync｜kSetupAck 回包，保存下server端的peer_qp_num, peer_lid, peer_gid信息，此时ModifyQpToRts状态到IBV_QPS_RTS
4、client发送（self_info.header.flag = kSetupAck && qp_num, lid, gid）信息到server;
5、server收到（self_info.header.flag = kSetupAck && qp_num, lid, gid）,此时server端的框架才将上面创建的qp设置为KConnected状态，业务才能能够发送包。

注：上面的ModifyQpToRts中的实现如下，在ModifyQpToRts()中分两次设置了IBV_QPS_RTR和IBV_QPS_RTS
```
int RdmaQueuePair::ModifyQpToRts() {
    ...
  qp_attr.qp_state = IBV_QPS_RTR;
    ...
  err = ibv_modify_qp(qp_, &qp_attr,
                      IBV_QP_STATE | IBV_QP_AV | IBV_QP_PATH_MTU |
                          IBV_QP_DEST_QPN | IBV_QP_RQ_PSN |
                          IBV_QP_MAX_DEST_RD_ATOMIC | IBV_QP_MIN_RNR_TIMER);
  ...

  qp_attr.qp_state = IBV_QPS_RTS;
    ...
  err = ibv_modify_qp(qp_, &qp_attr, IBV_QP_STATE | IBV_QP_TIMEOUT |
                                       IBV_QP_RETRY_CNT | IBV_QP_RNR_RETRY |
                                       IBV_QP_SQ_PSN | IBV_QP_MAX_QP_RD_ATOMIC);
  ...
}
```


### 方式2、调用rdmacm中间件创建（cm api）
* 思路   
利用rdma cm中间件进行api调用进行编码

* Demo   
[MLNX_OFED_LINUX-5.1-2.3.7.1-RHEL7.4-X86_64](https://docs.mellanox.com/pages/viewpage.action?pageId=39279600)
路径：src/MLNX_OFED_SRC-5.1-2.3.7.1/SRPMS/rdma-core-51mlnx1/librdmacm/examples/rdma_xclient.c

* 使用方式   
```
//1、获取addrinfo
ret = rdma_getaddrinfo(server, port, &hints, &res);

//2、创建qp对象
ret = rdma_create_ep(&id, res, NULL, &attr);

//3、qp链接
ret = rdma_connect(id, NULL);
```



The following subsections briefly describe the various components of the Mellanox OFED stack.
![picture 2](images/e579b374986c0a936602290b6c4677a2ba78f6534668640a31fb390405a118e7.png)  

* mlx4 VPI Driver  
the low-level driver implementation for the ConnectX® family adapters. 
To accommodate the supported configurations, the driver is split into the following modules:
```
mlx4_core (Handles low-level functions)
mlx4_ib (IB)
mlx4_en (Ethernet)
```
* mlx5 Driver  
the low-level driver implementation for the Connect-IB® and ConnectX®-4 adapters.
Connect-IB® operates as an InfiniBand adapter whereas and ConnectX®-4 operates as a VPI adapter (Infiniband and Ethernet). 
The mlx5 driver is comprised of the following kernel modules: 
```
mlx5_core(Acts as a library of common functions)
mlx5_ib(Handles InfiniBand-specific functions and plugs into the InfiniBand mid layer.)
libmlx5(libmlx5 is the provider library that implements hardware specific user-space functionality.)
```
* Mid-layer core  
Verbs, MADs, SA, CM, CMA, uVerbs, uMADs
* Upper Layer Protocols (ULPs)  
```
IP over IB (IPoIB);  
iSCSI Extensions for RDMA (iSER);
SCSI RDMA Protocol (SRP);
```

* User Direct Access Programming Library (uDAPL)
* Message Passing Interface (MPI)  
* InfiniBand Subnet Manager
* Diagnostic Utilities
* Mellanox Firmware Tools


[MLNX_OFED Documentation Rev 4.6-1.0.1.1](https://docs.mellanox.com/display/MLNXOFEDv461000/Introduction)  
[MLNX_EN Documentation Rev 5.0-1.0.0.0](https://docs.mellanox.com/display/MLNXEN501000/Introduction)
[NVIDIA® MLNX_OFED Documentation Rev 5.2-2.2.3.0](https://docs.mellanox.com/display/MLNXOFEDv522230/Introduction)
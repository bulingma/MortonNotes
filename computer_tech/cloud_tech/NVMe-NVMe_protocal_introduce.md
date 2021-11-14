
## What is NVMe®?
The NVM Express® (NVMe®) family of specifications define how host software communicates with non-volatile memory across multiple transports like PCI Express® (PCIe®), RDMA, TCP and more. It is the industry standard for solid state drives (SSDs) in all form factors (U.2, M.2, AIC, EDSFF). NVM Express is the non-profit consortium(国际联盟、财团) of tech industry leaders defining, managing and marketing NVMe technology.  

The latest NVMe 2.0 library of specifications consists of multiple documents, including the: 
* NVMe Base specification 
* Command Set specifications（NVM Command Set specification、ZNS Command Set specification、KV Command Set specification)
* Transport specifications (PCIe Transport specification, Fibre Channel Transport specification, RDMA Transport specification and TCP Transport specification) 
* NVMe Management Interface specification.

The NVMe specification was designed from the ground up for SSDs. It is a much more efficient interface, providing lower latency, and is more scalable for SSDs than legacy interfaces, like serial ATA (SATA). The first part of the specification is the host control interface. The NVMe architecture brings a new high performance queuing mechanism that supports 65,535 I/O queues each with 65,535 commands (referred to as queue depth, or number of outstanding commands). Queues are mapped to CPU cores delivering scalable performance. <u>**The NVMe interface significantly reduces the number of memory-mapped input/output commands and accommodates(适配) operating system device drivers running in interrupt or polling modes for higher performance and lower latency.**</u> 
The NVMe specification also contains host-to-device protocol for SSD commands used by an operating system for: read, write, flush, TRIM, firmware management, temperature, errors and others.



[NVME官网](https://nvmexpress.org/)  
[NVMe Specifications Overview](https://nvmexpress.org/developers/)  
[NVMe-OF-Specification](https://nvmexpress.org/developers/nvme-of-specification/)
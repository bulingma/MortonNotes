
## 异步io
#### 1、Glibc AIO
glibc在用户态用pthread实现的，采用的是POSIX的接口，不需要内核支持，模拟异步io的功能，内部其实调用的是普通的blocking io接口，其能正常工作起来，还依赖内部buffer cache的实现，即会多存在一次copy。

* 代码实现文件
/lib64/librt.so  

* 相关头文件及Api定义(/usr/include/aio.h)
```
int aio_read(struct aiocb *aiocbp);
int aio_write(struct aiocb *aiocbp);
int aio_fsync(int op, struct aiocb *aiocbp);
int aio_error(const struct aiocb *aiocbp);
ssize_t aio_return(struct aiocb *aiocbp);
int aio_suspend(const struct aiocb * const aiocb_list[], int nitems, const struct timespec *timeout);
int aio_cancel(int fd, struct aiocb *aiocbp);
int lio_listio(int mode, struct aiocb *const aiocb_list[], int nitems, struct sigevent *sevp);
```

* 相关链接  
[aio_read(3)](https://man7.org/linux/man-pages/man3/aio_read.3.html)

#### 2、Kernel Native AIO  
linux AIO是linux**内核原生支持**的异步IO**系统调用**，行为更加低级，其内部实现无需额外的buffer cache。  
linux内核仅仅提供了5个系统调用用以进行异步io处理。

* 代码实现文件
内核代码中的fs/aio.c 文件是Linux AIO的相关实现。

* 相关头文件及Api定义(include/linux/aio_abi.h)
```
int io_setup(unsigned nr_events, aio_context_t *ctxp);
int io_destroy(aio_context_t ctx);
int io_submit(aio_context_t ctx, long nr, struct iocb **iocbpp);
int io_cancel(aio_context_t ctx, struct iocb *, struct io_event *result);
int io_getevents(aio_context_t ctx, long min_nr, long nr, struct io_event *events, struct timespec *timeout); 
```

* 相关链接  
[io_setup](https://man7.org/linux/man-pages/man2/io_setup.2.html)

#### 3、libaio  
libaio 是 Kernel Native AIO 的一个Wrapper(libaio是Kernel Native AIO的一种封装接口)。  

* libaio 和 kernel native AIO的区别说明
```
       Glibc does not provide a wrapper for this system call.  You could
       invoke it using syscall(2).  But instead, you probably want to
       use the io_submit() wrapper function provided by libaio.
            
        Note that the libaio wrapper function uses a different type
        (io_context_t) for the ctx_id argument.  Note also that the
        libaio wrapper does not follow the usual C library conventions
        for indicating errors: on error it returns a negated error number
        (the negative of one of the values listed in ERRORS).  If the
        system call is invoked via syscall(2), then the return value
        follows the usual conventions for indicating an error: -1, with
        errno set to a (positive) value that indicates the error.
```
[io_submit(2) — Linux manual page](https://man7.org/linux/man-pages/man2/io_submit.2.html)

* 代码实现文件

* 相关头文件及Api定义(/usr/include/libaio.h)
```
/* library wrappers */
extern int io_queue_init(int maxevents, io_context_t *ctxp);
/*extern int io_queue_grow(io_context_t ctx, int new_maxevents);*/
extern int io_queue_release(io_context_t ctx);
/*extern int io_queue_wait(io_context_t ctx, struct timespec *timeout);*/
extern int io_queue_run(io_context_t ctx);

/* Actual syscalls */
extern int io_setup(int maxevents, io_context_t *ctxp);
extern int io_destroy(io_context_t ctx);
extern int io_submit(io_context_t ctx, long nr, struct iocb *ios[]);
extern int io_cancel(io_context_t ctx, struct iocb *iocb, struct io_event *evt);
extern int io_getevents(io_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct timespec *timeout);
```

* 相关链接  
[libaio.h源码](https://code.woboq.org/linux/include/libaio.h.html)

#### 4、io_uring


### 相关参考
[libaio](https://pagure.io/libaio)
[Difference between POSIX AIO and libaio on Linux?](https://stackoverflow.com/questions/8768083/difference-between-posix-aio-and-libaio-on-linux)
[Linux Asynchronous I/O](https://oxnz.github.io/2016/10/13/linux-aio/)
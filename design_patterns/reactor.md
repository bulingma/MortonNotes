IO-Multiplex 在不同的操作系统下有不同的推荐实现，最通用的 select()， POSIX 的 poll()，Linux 的 epoll()，FreeBSD 的 kqueue 等等。


## 0. reactor的代码模型
```cpp
while(1){
    // poll模型
    int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);

    // epoll模型
    int numEvents = ::epoll_wait(epollfd_,
                            &*events_.begin(),
                            static_cast<int>(events_.size()),
                            timeoutMs);
    
    // select模型
    select(maxfdpl, &readset, NULL, NULL, NULL);

    //kqueue模型
    int nev = kevent(kq, changes, FD_NUM, events, FD_NUM, NULL);
    
    ...
}
```

[kqueue用法简介](https://www.cnblogs.com/luminocean/p/5631336.html)
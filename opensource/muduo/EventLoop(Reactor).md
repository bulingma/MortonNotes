
<!-- TOC -->

- [Muduo reactor模型代码分析](#muduo-reactor模型代码分析)
  - [0. reactor的代码模型](#0-reactor的代码模型)
  - [1. EventLoop::loop()——Muduo reactor的核心实现代码](#1-eventlooploopmuduo-reactor的核心实现代码)
  - [2. poller_->poll的两种实现](#2-poller_-poll的两种实现)
    - [2.1. poll](#21-poll)
    - [2.2. Epoll](#22-epoll)

<!-- /TOC -->


## Muduo reactor模型代码分析
Muduo的io多路复用模型采用reactor模型，以muduo-2.0.2版本进行分析

### 0. reactor的代码模型
```
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
    
    ...
}
```

### 1. EventLoop::loop()——Muduo reactor的核心实现代码 

```
void EventLoop::loop()
{
  ...
  quit_ = false;  // FIXME: what if someone calls quit() before loop() ?
  ...

  while (!quit_){
    ...
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);  
    ...
    // TODO sort channel by priority
    for (Channel* channel : activeChannels_){
      currentActiveChannel_ = channel;
      currentActiveChannel_->handleEvent(pollReturnTime_);
    }
    currentActiveChannel_ = NULL;
    doPendingFunctors();
  }
}
```

### 2. poller_->poll的两种实现 

Muduo中poller_->poll的实现有两种, poll 和 EPoll

#### 2.1. poll 
```
Timestamp PollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
  // XXX pollfds_ shouldn't change
  int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
  if (numEvents > 0)
  {
    LOG_TRACE << numEvents << " events happened";
    fillActiveChannels(numEvents, activeChannels);
  }
  ...
}

void PollPoller::fillActiveChannels(int numEvents,
                                    ChannelList* activeChannels) const
{
  for (PollFdList::const_iterator pfd = pollfds_.begin();
      pfd != pollfds_.end() && numEvents > 0; ++pfd){
    if (pfd->revents > 0){
      --numEvents;
      ChannelMap::const_iterator ch = channels_.find(pfd->fd);
      Channel* channel = ch->second;
      channel->set_revents(pfd->revents);
      activeChannels->push_back(channel);  //将某个fd的事件放入相关的Channel
    }
  }
}
```

#### 2.2. Epoll 
```
Timestamp EPollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
  int numEvents = ::epoll_wait(epollfd_,
                               &*events_.begin(),
                               static_cast<int>(events_.size()),
                               timeoutMs);
  if (numEvents > 0){
    fillActiveChannels(numEvents, activeChannels);
    if (implicit_cast<size_t>(numEvents) == events_.size()){
      events_.resize(events_.size()*2);
    }
  }
}

void EPollPoller::fillActiveChannels(int numEvents,
                                     ChannelList* activeChannels) const
{
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
    int fd = channel->fd();
    ChannelMap::const_iterator it = channels_.find(fd);
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}
```
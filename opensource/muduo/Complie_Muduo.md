## 参考
[centos7下muduo库的安装](https://blog.csdn.net/weixin_41498246/article/details/103996962)
[centos下muduo库的安装与使用](https://blog.csdn.net/qq_34673519/article/details/97753784)

## 环境
1、需要一台有外网的linux机器
2、centos 7系统

## muduo下载和编译
### 1、下载muduo源码  
```
git clone https://github.com/chenshuo/muduo.git
```

### 2、环境准备  
#### 2.1、安装cmake-3.62    
确保系统安装gcc,ncurses-devel（注意gcc的版本要支持c++11，centos的默认版本不支持）
```
yum -y install gcc-c++
yum -y install ncurses-devel
```
[cmake安装教程](https://blog.csdn.net/qq_19734597/article/details/104200371)


#### 2.2、安装非必要的依赖库
```
//如下两个命令需要先安装apt命令，apt在cmake安装中已经安装过了
apt-get install libcurl4-openssl-dev libc-ares-dev
apt-get install protobuf-compiler libprotobuf-dev
yum install gcc gcc-c++ bzip2 bzip2-devel bzip2-libs python-devel
//这两个在centos下执行会失败，按照上面两个执行
yum install openssl  
yum install protobuf
```

#### 2.3、下载、编译、安装boost库  
* 2.3.1、下载boost库    
这里一定要注意电脑中的Linux系统是否已经存在boost相关的包，如果不注意随便下载一个版本，到后面编译muduo会提示找不到boost库。运行命令查看系统中已经有的boost相关的包）
```
(base) [root@compile-morton-ma-spdk2101 CMakeFiles]# rpm -qa boost*
boost-regex-1.53.0-28.el7.x86_64
boost-date-time-1.53.0-28.el7.x86_64
boost-system-1.53.0-28.el7.x86_64
boost-thread-1.53.0-28.el7.x86_64
```
我的电脑里面的版本是1.53.0
官网下载boost 1.53.0的后缀为zip的压缩包(根据自己电脑的版本进行下载哦） 
https://sourceforge.net/projects/boost/files/boost/

* 2.3.2、编译、安装boost库     
```
unzip boost_1_53_0.zip
cd boost_1_53_0
./bootstrap.sh
./b2 install
```

### 4、编译muduo
```
cd muduo 
./build.sh -j2
```

编译完成
```
[ 98%] Built target sudoku_solver_hybrid
Scanning dependencies of target sudoku_solver_prod
[ 98%] Building CXX object examples/sudoku/CMakeFiles/sudoku_solver_prod.dir/server_prod.cc.o
[ 98%] Linking CXX executable ../../../bin/memcached_debug
[ 98%] Built target memcached_debug
[ 98%] Building CXX object examples/sudoku/CMakeFiles/sudoku_solver_prod.dir/sudoku.cc.o
[100%] Linking CXX executable ../../bin/sudoku_solver_prod
[100%] Built target sudoku_solver_prod
[root@10-13-0-163 muduo]# pwd
/home/morton/muduo
```

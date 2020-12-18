# 操作系统实验：Linux驱动开发

## 1. 读写int数据驱动

[代码点我](https://github.com/AgentGuo/linux_device_driver/tree/main/intDevice)，更加详细的说明可以参考[这里](https://blog.csdn.net/weixin_44338712/article/details/111322809)

### 1.1 功能说明

能够像驱动缓冲区中写入和读取数据，就是这么简单哈哈哈 :-)

### 1.2 how to run it

编译驱动程序

~~~
make
~~~

然后安装驱动模块：

~~~
sudo insmod intDevice.ko
~~~

创建设备文件

~~~cpp
mknod /dev/intDevice c 112 0
// mknod /dev/[设备名] c [主设备号] [次设备号]
~~~

### 1.3 驱动测试

运行test.c即可~~

## 2. 读写char数据驱动

代码[在这](https://github.com/AgentGuo/linux_device_driver/tree/main/intDevice)

基本和int数据驱动没区别~~ 不知道老师咋想的 :-(
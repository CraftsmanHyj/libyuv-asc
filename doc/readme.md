Tips：图片需要把文件拉到本地才能显示，用的是本地相对路径

# NDK配置

创建新项目之后，最开始调试的时候以library的方式导入项目，需要在build.gradle中配置ndk，以便导入library中libs目录下的so文件。

```xml
defaultConfig {
	......
    ndk {
        abiFilters 'armeabi', "armeabi-v7a"//, "x86", "mips"
    }
	......
}
```

# SO动态库编译

# 打包aar

# NDK中输出日志方法



- 问题定位方法
  - adder2line
  - ndk-slack



# 问题集锦

在项目的开发测试过程中碰到的一些问题记录下来，以待日后查看。

## This file is not part of the project

当项目的ndk的路径没有设置的时候，就会在JNI的C实现方法上面报这个错误：

<img src="readme/image/image-20201208095800565.png" alt="image-20201208095800565"  />

这个错误的原因是因为没有设置项目的NDK的路径，解决步骤如下：

1. 项目根目录上右键→open module settings
   ![image-20201208100126319](readme/image/image-20201208100126319.png)
2. 在SDK Location中输入你对应的NDK的的路径即可
   <img src="readme/image/image-20201208100305840.png" alt="image-20201208100305840" style="zoom:67%;" />
3. 最后点击Apply即可解决问题。
# Project Configuration - 工程配置

本节介绍如何从创建一个新的WCH-CH32V103工程，并进行初步配置。

- 简单食用：直接看“简单食用”一节，即可快速建立模板工程
- 从零开始：利用Moun_River Studio从零建立一个CH32V103工程
- 进阶技巧：一些trick

[TOC]



## 简单食用

仅需三步，即可快速建立模板工程。

- 下载工程到自己的工作空间
- 打开Moun_River Studio
- 菜单栏 `File` -> `Lode Project`，选择工程目录中 `.project`文件的路径。工程将自动导入



## 从零开始

本节利用Moun_River Studio从零建立一个CH32V103工程。

### 创建工程

- 打开Moun_River Studio

- 菜单栏 `File` -> `New` ->`MounRiver Project`，进入新建工程界面如下
  - 在默认配置上，更改以下设置
    - `Project Name` ：工程名
    - `Location` ：工作空间
    - `Vendor/Series` ：系列。选择 `WCH/CH32V103[RISC-V]`
    - Type：具体型号：根据需要选择。当前使用的是 CH32V103R8T6，我们选中它
  - 点击 `Finish`

![image-20201219110836275](Project Configuration.assets/image-20201219110836275.png)

- 工程建立后，工程目录如下：

![image-20201219114129696](Project Configuration.assets/image-20201219114129696.png)

### 整理工作目录，配置路径

推荐按照以下范式重新整理其工程结构。整理后，使用习惯贴近传统NXP工程。

![image-20201219112221975](Project Configuration.assets/image-20201219112221975.png)

- `board`：主控板的相关配置
- `component`：组件。这里仅有`debug.c/h`
- `core`：有关risc-v内核的相关设置
- `Debug`：链接文件生成位置
- `device`：ch32v103芯片的相关宏定义
- `doc`：文档
- `drivers`：ch32v103的驱动
- `source`：用户自己的代码存放位置
- `Startup`：ch32v103启动文件

------

整理好后，按照实际情况自行添加引用目录路径。方法与其他Eclipse工程一致。



## 进阶技巧

本节介绍一些trick。

### 开启C++支持

由于未知原因，MounRiver Studio创建的工程竟然没有默认C++支持。

![image-20201219114323948](Project Configuration.assets/image-20201219114323948.png)

为了使用C++相关功能，必须手动修改eclipse相关配置文件。


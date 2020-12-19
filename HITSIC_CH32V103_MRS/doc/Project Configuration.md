Project Configuration - 工程配置

本节介绍如何从创建一个新的WCH-CH32V103工程，并进行初步配置。

- 简单食用：如果只想快速上手底层库，请直接看“简单食用”一节，即可快速建立模板工程
- 从零开始：如果希望从零开始配置一个模板工程，请参阅本节，并利用Moun_River Studio从零建立一个CH32V103工程
- 灾难恢复：如果发生灾难性工程属性配置错误，请参看“从零开始-C/C++ Build 所有工程参数设置”一节。

[TOC]



## 简单食用

仅需三步，即可快速建立模板工程。

- 下载工程到自己的工作空间
- 打开Moun_River Studio
- 菜单栏 `File` -> `Lode Project`，选择工程目录中 `.project`文件的路径。工程将自动导入



可供使用的工程在`cpp_support`分支下。



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

![image-20201219110836275](figure/project_configuration/image-20201219110836275.png)

- 工程建立后，工程目录如下：

![image-20201219114129696](figure/project_configuration/image-20201219114129696.png)

### 整理工作目录，配置路径

推荐按照以下范式重新整理其工程结构。整理后，使用习惯贴近传统NXP工程。

![image-20201219112221975](figure/project_configuration/image-20201219112221975.png)

- `board`：主控板的相关配置
- `component`：组件。这里仅有`debug.c/h`
- `core`：有关risc-v内核的相关设置
- `Debug`：链接文件生成位置
- `device`：ch32v103芯片的相关宏定义
- `doc`：文档
- `drivers`：ch32v103的驱动，将原工程中`Peripheral`文件夹下所有内容放在同级目录即可
- `source`：用户自己的代码存放位置
- `Startup`：ch32v103启动文件

------

整理好后，按照实际情况自行添加引用目录路径。方法与其他Eclipse工程一致。



### 重定向syscall

在source目录下添加syscall.c，用以重定向IO操作

```c++
#include "board.h"

#define RETARGET_INST   (LPUART0)

#ifdef __cplusplus
extern "C"{
#endif

#if defined (__NEWLIB__)

// ******************************************************************
// Redlib C Library function : __sys_write
// Newlib C library function : _write
//
// Function called by bottom level of printf routine within C library
// to write multiple characters.
// ******************************************************************
int _write(int iFileHandle, char *pcBuffer, int iLength)
{
    // Check that iFileHandle == 1 to confirm that read is from stdout
    if(1 == iFileHandle)
    {
        for(int i=0; i < iLength; ++i)
        {
#if (DEBUG_UART == DEBUG_UART1)
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
            USART_SendData(USART1, *pcBuffer++);
#elif (DEBUG_UART == DEBUG_UART2)
            while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
            USART_SendData(USART2, *pcBuffer++);
#elif (DEBUG_UART == DEBUG_UART3)
            while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
            USART_SendData(USART3, *pcBuffer++);
#endif
        }
    }

    // Function returns number of unwritten bytes if error
    return 0;
}

// ******************************************************************
// Function _read
//
// Called by bottom level of scanf routine within Newlib C library
// to read multiple characters.
// ******************************************************************
int _read(int iFileHandle, char *pcBuffer, int iLength)
{
    // Check that iFileHandle == 0 to confirm that read is from stdin
    if(0 == iFileHandle)
    {
//    if(kStatus_Success == LPUART_ReadBlocking(RETARGET_INST, pcBuffer, iLength))
//    {
//        return iLength;
//    }
//    else
//    {
//        return 0;// Count of characters read
//    }
    }
    return 0;
}

#if 0

/* Provide prototypes for most of the _<systemcall> names that are
 provided in newlib for some compilers.  */
int _close(int file)
{

}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}
int _open(const char *name, int flags, int mode)
{
    return -1;
}

void _exit(void)
{
    while (1);
}

void _kill(void)
{

}

void _fstat(void)
{

}

#endif

#endif // ! __NEWLIB__


#ifdef __cplusplus
}
#endif
```

- 移动UART宏定义：将以下宏定义由`debug.h`移动至`board.h`中

```c++
/* UART Printf Definition */
#define DEBUG_UART1    1
#define DEBUG_UART2    2
#define DEBUG_UART3    3

/* DEBUG UATR Definition */
#define DEBUG   DEBUG_UART1
//#define DEBUG   DEBUG_UART2
//#define DEBUG   DEBUG_UART3

```



- 移动UART写函数：移动`debug.c/h`中的函数`void USART_Printf_Init(uint32_t baudrate)`，`int _write(int fd, char *buf, int size)`至`board.c/h`中

### 进阶技巧--开启C++支持

由于未知原因，MounRiver Studio创建的默认Moun_River工程竟然没有默认C++支持。

![image-20201219114323948](figure/project_configuration/image-20201219114323948.png)

**为了使用C++相关功能，必须手动修改eclipse相关配置文件。**其原理简单描述为：对比Moun_River默认工程和RISC-V默认工程的配置文件获得。

- 创建参考C++工程：
  - 菜单栏选择`File->New->Project`，在窗口中选择`C++ Project`
  - 工程设置界面，选择Hello World RISC-V C++ Project，其他保持默认设置。完成后点击Next![image-20201219214409869](figure/project_configuration/image-20201219214409869.png)
  - 之后的`Basic Settings`，`Select Configuration`选项卡可以根据自己需要更改，不做强制要求。
  - 工具链选择：在工具链选项卡中，选择`risc-none-embed-gcc`，之后点击Finish![image-20201219214841881](figure/project_configuration/image-20201219214841881.png)

- 对比工程，更改工程的`C/C++ Nature`
  - 对比新创建的C++工程和默认的Moun_River工程，发现在`.project`文件中的`nature`项中，缺少C++ Nature，如图：![image-20201219215134518](figure/project_configuration/image-20201219215134518.png) 

- 将这一行复制到Moun_River的`.project`的对应位置。

```js
		<nature>org.eclipse.cdt.core.ccnature</nature>
```

- 其它配置文件也会存在一些区别，但是不用手动更改。这是因为：更改完C++ Nature 后，再次打开eclipse工程属性后，eclipse会重新生成其他相关配置文件。
- 关闭原始的Moun_River，之后重新打开工程。再次打开工程属性，发现Hack成功！![image-20201219215629028](figure/project_configuration/image-20201219215629028.png)

### C/C++ Build 所有工程参数设置

请按照以下过程更改工程属性`Properties->C/C++ Build->Settings` 下的部分设置。

这一部分也可用来在发生灾难性设置更改后恢复正常工程状态。

#### Tool Settings

本节只涉及可能改变的设置。其他未涉及的请保持默认。

- Target Processor

![image-20201219221020828](figure/project_configuration/image-20201219221020828.png)

- Optimization

  ![image-20201219220945717](figure/project_configuration/image-20201219220945717.png)

- Warning

![image-20201219221134701](figure/project_configuration/image-20201219221134701.png)

- Debugging

![image-20201219221204516](figure/project_configuration/image-20201219221204516.png)

- GNU RISC-V Cross C Compiler

  - Preprocessor

    ![image-20201219222239437](figure/project_configuration/image-20201219222239437.png)

  ```
  CPU_CH32V103R8T6
  __NEWLIB__
  I2C_RETRY_TIMES=32768
  UART_RETRY_TIMES=32768
  ```

  - Includes

    ![image-20201219222339776](figure/project_configuration/image-20201219222339776.png)

  - Optimization

  ![image-20201219222439287](figure/project_configuration/image-20201219222439287.png)

  ```
  -std=c++17 -fno-common -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -fno-excep
  ```

  - Warning

  ![image-20201219222504871](figure/project_configuration/image-20201219222504871.png)

  - Miscellaneous

    ![image-20201219222611207](figure/project_configuration/image-20201219222611207.png)

    ```
    -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -frtti -fno-exceptions 
    ```

    

- GNU RISC-V Cross C++ Compiler

  - Optimization

    ![image-20201219222753229](figure/project_configuration/image-20201219222753229.png)

    ```
    -std=c++17 -fno-common -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -fn
    ```

    

  - Warning

  ![image-20201219222836175](figure/project_configuration/image-20201219222836175.png)

  - Miscellaneous

  ![image-20201219222902456](figure/project_configuration/image-20201219222902456.png)

  ```
  -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -frtti -fno-excep
  ```

- GUN RISC-V Cross C++ Linker

  - Libraries

    ![image-20201219223106976](figure/project_configuration/image-20201219223106976.png)

  - Miscellaneous

  ![image-20201219223140495](figure/project_configuration/image-20201219223140495.png)



#### Toolchains

![image-20201219223226366](figure/project_configuration/image-20201219223226366.png)



## Changelog

- ver 1.0：by JerrySkywalker, 12/18/2020。完成“简单食用”和“从零开始”所有章节。
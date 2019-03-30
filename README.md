# STemWin

## 1、介绍

emWin 设计用于提供高效且独立于处理器和显示控制器的图形用户界面，用于任何使用图形显示进行操作的应用。它与单任务和多任务环境、专用操作系统或具有任何商业 RTOS 兼容， emWin 的发货形式为 C 语言源代码。

STemWin 是 SEGGER 公司授权给 ST（意法半导体）的。使用 ST 芯片的用户可以免费使用 STemWin。

本源码实现了在RT-Thread上的移植。源码地址：
<https://github.com/loogg/STemWin>  作者：[loogg](https://github.com/loogg)

### 1.1 目录结构


| 名称 | 说明 |
| ---- | ---- |
| demo | lcd驱动实现的例子 |
| docs  | 文档目录 |
| examples | 例子目录，并有相应的一些说明 |
| inc  | 头文件目录 |
| src  | 源代码目录 |

### 1.2 许可证

STemWin package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- RT-Thread 4.0+


## 2、如何打开 STemWin

使用 STemWin package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    multimedia packages --->
        [*] STemWin: a STemWin package for rt-thread
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 STemWin

在打开 STemWin package 后，当进行 bsp 编译时，它会被加入到 bsp 工程中进行编译。

具体API参照官方手册。

## 4、注意事项

- 在初始化GUI `GUI_Init()`之前务必**开启CRC时钟**。如下：
```
    CRC_HandleTypeDef   CrcHandle;
    CrcHandle.Instance = CRC; 
    HAL_CRC_Init(&CrcHandle);
    GUI_Init();
```

- STemWin软件包通过lcd设备进行画点、读点等操作，确保LCD驱动已经注册进设备。

- LCD驱动示例位于[`/demo`](/demo) 下，示例使用并口LCD和u8g2 OLED。编写LCD驱动前 **务必查看**。

- 目前只支持如下颜色格式：
```
    switch(info.pixel_format)
    {
        case RTGRAPHIC_PIXEL_FORMAT_MONO:
            color = GUICC_1;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB565:
            color = GUICC_M565;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB888:
            color = GUICC_M888;
            break;
        default:
            color = GUICC_M565;
            break;
    }
```

## 5、联系方式 & 感谢

* 维护：malongwei
* 主页：<https://github.com/loogg/STemWin>
* 邮箱：<2544047213@qq.com>

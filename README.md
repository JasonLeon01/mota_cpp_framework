# mota_cpp_framework

## 简介

![image](sample.png)

基于C++和SFML制作的魔塔框架。

- [Docs / 使用文档](https://mota-cpp-tutorial.readthedocs.io/zh-cn/latest/)
- [Open Source / 动画编辑器开源](https://github.com/JasonLeon01/MotaAnimationDesigner)
- [Open Source / RM动画分离器开源](https://github.com/JasonLeon01/MotaAnimationSeparator)
- [Open Source / 怪物设计器开源](https://github.com/JasonLeon01/MotaEnemyDesigner)
- [Open Source / 地图编辑器开源](https://github.com/JasonLeon01/MotaMapDesigner)
- [Open Source / 物品编辑器开源](https://github.com/JasonLeon01/ItemDesigner)
- [Open Source / NPC设计器开源](https://github.com/JasonLeon01/MotaNPCDesigner)
- [Open Source / 打包工具开源](https://github.com/JasonLeon01/MotaPackager)

## 目录结构

```bash
├── /data/                    # 数据文件
│ ├─ /actor/                  # 角色信息的数据
│ ├─ /animation/              # 动画信息的数据
│ ├─ /element/                # 属性信息的数据
│ ├─ /enemy/                  # 敌人信息的数据
│ ├─ /item/                   # 物品信息的数据
│ ├─ /map/                    # 地图信息的数据
│ └─ /npc/                    # NPC信息的数据
├── /font/                    # 使用到的字体文件
├── /graphics/                # 使用到的图片素材文件
├── /ico/                     # exe文件icon相关
├── /include/                 # 包含的头文件
│ ├─ /SFML/                   # SFML库相关的头文件
│ ├─ /Game/                   # 游戏框架相关的头文件
│ │ ├─ /stdafx.hpp/          # 大多数头文件和外部函数的存放
│ │ ├─ /System/              # 游戏系统的设置
│ │ │ ├─ /Graphics.hpp/     # 图形画面相关的头文件，用于更新画面
│ │ │ ├─ /Image.hpp/        # 图像相关的头文件
│ │ │ ├─ /KeyBoard.hpp/     # 按键输入相关的头文件
│ │ │ ├─ /System.hpp/       # 游戏系统相关的头文件
│ │ │ ├─ /Text.hpp/         # 显示文字相关的头文件
│ │ │ └─ /Window.hpp/       # 游戏窗口相关的头文件
│ │ ├─ /Data/                # 游戏数据的设置
│ │ │ ├─ /Actor.hpp/        # 角色相关的头文件
│ │ │ ├─ /Animation.hpp/    # 动画相关的头文件
│ │ │ ├─ /Data.hpp/         # 游戏数据相关的头文件
│ │ │ ├─ /Element.hpp/      # 怪物属性相关的头文件
│ │ │ ├─ /Enemy.hpp/        # 怪物数据相关的头文件
│ │ │ ├─ /Item.hpp/         # 物品数据相关的头文件
│ │ │ ├─ /Map.hpp/          # 地图数据相关的头文件
│ │ │ ├─ /NPC.hpp/          # NPC数据相关的头文件
│ │ │ ├─ /Object.hpp/       # 地图事件对象相关的头文件
│ │ │ ├─ /Screen.hpp/       # 游戏画面相关的头文件
│ │ │ ├─ /Temp.hpp/         # 临时数据相关的头文件
│ │ │ └─ /Variables.hpp/    # 游戏变量相关的头文件
│ └─ /nlohmann/               # nlohmann库，主要使用其JSON文件解析
├── /lib/                     # 第三方库目录
├── /ref/                     # 游戏的参考文件
│ ├─ /config.ini/             # 游戏设置相关配置
│ ├─ /main.ini/               # 游戏初始化配置
│ ├─ /motaName.ini/           # 魔塔名对应初始化
│ └─ /ShortcutKey.txt/        # 快捷键的描述文本
├── /sound/                   # 使用到的音乐素材文件
├── /src/                     # 源文件的存放
│ ├─ /stdafx.cpp/             # stdafx源文件
│ ├─ /System/                 # include\System内部头文件对应源文件
│ ├─ /Data/                   # include\Data内部头文件源文件
│ └─ /main.cpp/               # 主源文件
├── /tools/                   # 常用小工具
│ ├─ /AnimationDesigner.exe/  # 游戏动画设计器
│ ├─ /AnimationSeparator.exe/ # RM格式动画转换器
│ ├─ /EnemyDesigner.exe/      # 游戏怪物设计器
│ ├─ /ItemDesigner.exe/       # 游戏物品编辑器
│ ├─ /MapDesigner.exe/        # 游戏地图设计器
│ ├─ /NPCDesigner.exe/        # 游戏NPC对话设计器
│ └─ /Packager.exe/           # 游戏内容打包器
├── /CMakeLists.txt/          # CMake相关
├── /config.exe/              # 游戏设置可执行文件
└── /main.exe/                # 启动游戏的可执行文件
```

## 更新说明

### 2023.11.29 Version 2.0

* [x] 增加物品编辑器
* [x] 将所有数据文件都改成json格式，增加可读性
* [x] 大幅优化代码，减少重复冗余
* [x] 将SFML的-s-d部分也添加到lib中，方便用户进行Debug
* [x] 游戏内添加设置分区，可以调整分辨率和音量等
* [x] 将游戏窗口名外置，在main.ini中修改
- [x] 键盘的repeat添加了首回延迟
- [x] 取消箭头素材，采用RMXP窗口自带的箭头
- [x] 将存读档按键改为“[”和“]”
- [x] 大幅优化架构，将原先文件里面的类尽量拆开到各文件
- [x] 增添自动事件，将出现触发条件设为3即可

### 2023.12.7 Version 2.1

- [x] 更换更加清晰的素材，让素材平滑放大
- [x] 在未能成功读取文件时增加弹窗提示

# 联系我们

本框架主要由[Jason Leon](https://github.com/JasonLeon01)（魔塔圈内ID `PRE`）制作编写

交流QQ群号：117445471（小号魔塔群）、210766281（大傻官方交流群）、662891318（盐铁桶子の魔塔小屋）

如有相关意见、建议或者合作意向，可发送邮件至[jasonleon01@outlook.com](mailto:jasonleon01@outlook.com)联系我。

# 合作者

[@Jason Leon](https://github.com/JasonLeon01)：PRE，b站空间[@Jason Leon](https://space.bilibili.com/439537579/)，本框架主要编写者，核心代码和主要地图绘制。

[@Youwei Zhao](https://github.com/zhaouv)：推动了各类编辑器的改动，使得更加对用户友好。

@忆忆：b站空间[@卡路十里](https://space.bilibili.com/200729395)，帮助找出bug、添加必要功能等。

# 版权声明

Copyright © 2023 Metempsychosis Game Studio. All rights reserved.

# 简介

![image](sample.png)

基于C++和SFML制作的魔塔框架。

- [Docs / 使用文档](https://mota-cpp-tutorial.readthedocs.io/zh-cn/latest/)
- [Open Source / 动画编辑器开源](https://github.com/JasonLeon01/MotaAnimationDesigner)
- [Open Source / RM动画分离器开源](https://github.com/JasonLeon01/MotaAnimationSeparator)
- [Open Source / 怪物设计器开源](https://github.com/JasonLeon01/MotaEnemyDesigner)
- [Open Source / 地图编辑器开源](https://github.com/JasonLeon01/MotaMapDesigner)
- [Open Source / NPC设计器开源](https://github.com/JasonLeon01/MotaNPCDesigner)

# 目录结构

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
│ └─ /Game/                   # 游戏框架相关的头文件
│ └─ ├─ /stdafx.h/            # 大多数头文件和外部函数的存放
│    ├─ /GameSystem.h/        # 游戏系统的设置
│    ├─ /GameData.h/          # 游戏数据的设置
├── /lib/                     # 第三方库目录
├── /ref/                     # 游戏的参考文件
│ ├─ /config.ini/             # 游戏设置相关配置
│ ├─ /main.ini/               # 游戏初始化配置
│ ├─ /motaName.ini/           # 魔塔名对应初始化
│ ├─ /ShortcutKey.txt/        # 快捷键的描述文本
├── /sound/                   # 使用到的音乐素材文件
├── /src/                     # 源文件的存放
│ ├─ /stdafx.cpp/             # stdafx源文件
│ ├─ /GameSystem.cpp/         # GameSystem源文件
│ ├─ /GameData.cpp/           # GameData源文件
│ ├─ /main.cpp/               # 主源文件
├── /tools/                   # 常用小工具
│ ├─ /AnimationDesigner.exe/  # 游戏动画设计器
│ ├─ /AnimationSeparator.exe/ # RM格式动画转换器
│ ├─ /EnemyDesigner.exe/      # 游戏怪物设计器
│ ├─ /MapDesigner.exe/        # 游戏地图设计器
│ ├─ /NPCDesigner.exe/        # 游戏NPC对话设计器
├── /CMakeLists.txt/          # CMake相关
├── /config.exe/              # 游戏设置可执行文件
├── /main.exe/                # 启动游戏的可执行文件
```

# 联系我们

本框架主要由[Jason Leon](https://github.com/JasonLeon01)（魔塔圈内ID `PRE`）制作编写

交流QQ群号：117445471（小号魔塔群）、210766281（大傻官方交流群）、662891318（盐铁桶子の魔塔小屋）

如有相关意见、建议或者合作意向，可发送邮件至[jasonleon01@outlook.com](mailto:jasonleon01@outlook.com)联系我。

# 合作者

[@Jason Leon](https://github.com/JasonLeon01)：PRE，b站空间[@Jason Leon](https://space.bilibili.com/439537579/)，本框架主要编写者，核心代码和主要地图绘制。

@忆忆：b站空间[@卡路十里](https://space.bilibili.com/200729395)，帮助找出bug、添加必要功能等。

# 版权声明

Copyright © 2023 Metempsychosis Game Studio. All rights reserved.

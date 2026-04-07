# ai-search-engine

一个基于 C++14 实现的搜索引擎学习项目，当前已经完成离线数据构建与在线关键词推荐的第一阶段闭环。

项目现阶段分为两部分：

- `offline/`：离线预处理，负责生成词典、词典索引、网页库、网页偏移库和倒排索引库
- `online/`：在线查询，当前已完成中英文关键词推荐模块

## 当前已完成功能

### 1. 离线词典模块 `offline/word`

已完成：

- 英文词典构建
- 英文词典索引构建
- 中文词典构建
- 中文词典索引构建
- 英文停用词过滤
- 中文停用词过滤
- 中文无效 token 过滤
- 输出目录自动创建

当前产物：

- `en_dict.dat`
- `en_index.dat`
- `cn_dict.dat`
- `cn_index.dat`

说明：

- 英文索引按单个字母建立
- 中文索引按 UTF-8 字符粒度建立
- 中文字符拆分使用 `cppjieba::DecodeRunesInString`

### 2. 离线网页库模块 `offline/page`

已完成：

- XML 网页解析
- 网页库构建 `pagelib.dat`
- 网页偏移库构建 `offsetlib.dat`
- 倒排索引库构建 `indexlib.dat`
- 网页去重
- 中文停用词过滤
- 中文无效 token 过滤

当前产物：

- `pagelib.dat`
- `offsetlib.dat`
- `indexlib.dat`

说明：

- 网页库构建依赖 `tinyxml2`
- 网页去重依赖 `simhash`
- 中文处理依赖 `cppjieba`

### 3. 在线关键词推荐模块 `online/recommend`

已完成：

- 英文关键词推荐
- 中文关键词推荐
- 中英文查询类型识别
- 查询词 token 化
- 候选词召回
- 基于编辑距离的候选排序
- `topk` 结果截断

当前支持：

- 纯英文查询
- 纯中文查询
- 含中文的中英混合查询按中文路径处理

当前排序规则：

1. 编辑距离升序
2. 词频降序
3. 单词字典序升序

说明：

- 英文查询按单字符切分
- 中文查询按 UTF-8 字符切分
- 编辑距离使用动态规划版本的 Levenshtein Distance

## 当前项目结构

```text
ai-search-engine/
├── CMakeLists.txt
├── README.md
├── app/
├── offline/
│   ├── CMakeLists.txt
│   ├── word/
│   │   ├── CMakeLists.txt
│   │   ├── conf/
│   │   ├── data/
│   │   ├── include/
│   │   └── src/
│   └── page/
│       ├── CMakeLists.txt
│       ├── conf/
│       ├── data/
│       ├── include/
│       └── src/
├── online/
│   ├── CMakeLists.txt
│   ├── common/
│   │   ├── include/
│   │   └── src/
│   ├── conf/
│   ├── recommend/
│   │   ├── include/
│   │   └── src/
│   └── server/
│       ├── include/
│       └── src/
└── third_party/
    ├── cppjieba/
    ├── simhash/
    └── tinyxml2/
```

## 构建环境

- C++14
- CMake >= 3.10

第三方库已以内嵌源码方式放在 `third_party/`：

- `cppjieba`
- `simhash`
- `tinyxml2`

## 构建方式

在仓库根目录执行：

```bash
cmake -S . -B build
cmake --build build
```

生成的可执行文件会输出到各模块自己的 `bin/` 目录：

- `offline/word/bin/word_producer`
- `offline/page/bin/page_processor`
- `online/bin/server`

## 使用方式

### 1. 生成词典与词典索引

```bash
./offline/word/bin/word_producer
```

默认配置文件：

- `offline/word/conf/word.conf`

运行后会在 `offline/word/data/output/` 下生成：

- `en_dict.dat`
- `en_index.dat`
- `cn_dict.dat`
- `cn_index.dat`

### 2. 生成网页库与倒排索引

```bash
./offline/page/bin/page_processor
```

默认配置文件：

- `offline/page/conf/page.conf`

运行后会在 `offline/page/data/output/` 下生成：

- `pagelib.dat`
- `offsetlib.dat`
- `indexlib.dat`

### 3. 在线关键词推荐

```bash
./online/bin/server hello
./online/bin/server helo
./online/bin/server 中国
./online/bin/server AI搜索
```

默认配置文件：

- `online/conf/online.conf`

当前在线模块依赖离线词典模块先生成以下文件：

- `offline/word/data/output/en_dict.dat`
- `offline/word/data/output/en_index.dat`
- `offline/word/data/output/cn_dict.dat`
- `offline/word/data/output/cn_index.dat`

## 关键配置说明

### `offline/word/conf/word.conf`

负责配置：

- 英文原始语料路径
- 中文原始语料路径
- 中英文停用词路径
- 词典输出目录与输出文件名

### `offline/page/conf/page.conf`

负责配置：

- 原始网页数据目录
- XML 数据目录
- 输出目录
- `cppjieba` 所需词典路径
- 中文停用词文件

### `online/conf/online.conf`

负责配置：

- 英文词典路径
- 英文索引路径
- 中文词典路径
- 中文索引路径
- 推荐结果数量 `topk`

## 当前阶段进度

当前已经完成：

- 离线词典构建
- 离线网页库构建
- 在线中英文关键词推荐

当前尚未完成：

- 在线网页查询模块
- 缓存系统
- Reactor 网络层
- 协议解析与完整服务化

## 下一阶段计划

下一阶段建议重点推进 `online/search`：

1. 读取 `pagelib.dat`
2. 读取 `offsetlib.dat`
3. 读取 `indexlib.dat`
4. 构建网页查询与召回流程
5. 完成网页结果排序与输出

## 项目说明

这个仓库当前更适合作为“搜索引擎项目学习实现”而不是完整产品工程。现阶段重点已经从离线构建推进到在线查询，后续工作会集中在网页搜索、缓存和网络服务层。

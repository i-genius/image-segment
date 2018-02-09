# image-segment

---
**基于图的图像分割**，即使用一种基于图表示（graph-based）的图像分割方法将图像（image）分割成若干个特定的、具有独特性质的区域（region），然后从中提取出感兴趣的目标（object）。

---

## 工程目录

* 目录树
>
```
.
│  .gitignore
│  README.md
│
├─data
│  │  src.png
│  │  src2.png
│  │
│  ├─regions
│  │      region_%d.PPM
│  │
│  ├─regions_2
│  │      region_%d.PPM
│  │
│  └─result
│          bound_coordinate.txt
│          res.png
│
├─distribute
│  └─win
│      ├─Debug
│      └─x64
│          └─Debug
│
├─include
│  │  color_hist.h
│  │  convert.h
│  │  generate_bound.h
│  │  region.h
│  │  region_param.h
│  │  texture_hist.h
│  │  type_convert.h
│  │
│  ├─3rdparty
│  │  ├─opencv
│  │  │
│  │  └─opencv2
│  │      
│  ├─anigauss
│  │      anigauss.h
│  │
│  └─FelZenSegment
│          convolve.h
│          disjoint_set.h
│          filter.h
│          image.h
│          imconv.h
│          imutil.h
│          misc.h
│          pnmfile.h
│          segment_graph.h
│          segment_image.h
│
├─libs
│  ├─linux
│  │  ├─x64
│  │  └─x86
│  └─win
│      ├─x64
│      │
│      └─x86
│
├─sample
│      convert.cpp
│      test.cpp
│
├─src
│  │  color_hist.cpp
│  │  generate_bound.cpp
│  │  region.cpp
│  │  region_param.cpp
│  │  texture_hist.cpp
│  │
│  ├─anigauss
│  │      anigauss.cpp
│  │
│  └─FelZenSegment
│          convolve.cpp
│          filter.cpp
│          imconv.cpp
│          pnmfile.cpp
│          segment_graph.cpp
│          segment_image.cpp
│
└─win
    │  design.sdf
    │  design.sln
    │
    └─design
        │
        ├─Debug
        │
        └─x64
            └─Debug

```

* `src/`目录下为工程`.cpp`源码文件，`include/`目录下为`.h`文件，`sample/`目录下为测试用例，输入数据存放于`data/`目录，输出数据存放于`data/result/`下，目录`data/regions`和`data/regions_2`保存图像分割中间结果，工程产生的可执行程序或库文件存放于`distribute/`目录，第三方依赖库置于`libs/`目录下。


* 工程编译依赖`opencv`，以便于图像数据的读取与写回。

---

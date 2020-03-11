# pyltp

[![PyPI Status](https://badge.fury.io/py/pyltp.svg)](https://badge.fury.io/py/pyltp)
[![Readthedocs](https://readthedocs.org/projects/pyltp/badge/?version=latest)](http://pyltp.readthedocs.io/)
[![Build Status](https://travis-ci.org/HIT-SCIR/pyltp.svg?branch=master)](https://travis-ci.org/HIT-SCIR/pyltp)
[![Build status](https://ci.appveyor.com/api/projects/status/kp2kjujo4amunyvr/branch/master?svg=true)](https://ci.appveyor.com/project/Oneplus/pyltp/branch/master)
[![PyPI Downloads](https://img.shields.io/pypi/dm/pyltp.svg)](https://pypi.python.org/pypi/pyltp)

pyltp 是 [语言技术平台（Language Technology Platform, LTP）](https://github.com/HIT-SCIR/ltp)的 Python 封装。

在使用 pyltp 之前，您需要简要了解 [语言技术平台（LTP）](http://ltp.readthedocs.org/zh_CN/latest/) 能否帮助您解决问题。

## 依赖支持情况
|               | Py2.6 | Py2.6 | Py3.4 | Py3.5 | Py3.6 | conda-python |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| Linux         | 支持   | 支持  | 支持   | 支持   | 支持  | 不支持  |
| Mac OS        | 支持   | 支持  | 支持   | 支持   | 支持  | 不支持  |
| Windows VS2015| 不支持 | 不支持 | 不支持 | 支持   | 支持  | 不支持  |
| Windows VS2017| 不支持 | 不支持 | 不支持 | 支持   | 支持  | 不支持  |

## 一个简单的例子

下面是一个使用 pyltp 进行分词的例子

```python
# -*- coding: utf-8 -*-
from pyltp import Segmentor
segmentor = Segmentor()
segmentor.load("/path/to/your/cws/model")
words = segmentor.segment("元芳你怎么看")
print "|".join(words)
segmentor.release()
```
除了分词之外，pyltp 还提供词性标注、命名实体识别、依存句法分析、语义角色标注等功能。

详细使用方法请参考 [在线文档](http://pyltp.readthedocs.io/)。

## 安装

* 第一步，安装 pyltp

	使用 pip 安装

	```
	$ pip install pyltp
	```
	或从源代码安装

	```
	$ git clone https://github.com/HIT-SCIR/pyltp
	$ git submodule init
	$ git submodule update
	$ python setup.py install
	```

	+ Mac系统出现版本问题使用 MACOSX_DEPLOYMENT_TARGET=10.7 python setup.py install
	+ 编译时间较长（约5分钟左右），请耐心等待

* 第二步，下载模型文件

	[七牛云](http://ltp.ai/download.html)，当前模型版本 3.4.0

## 版本对应

* pyltp 版本：0.2.0
* LTP 版本：3.4.0
* 模型版本：3.4.0

## 作者

* 徐梓翔 << zxxu@ir.hit.edu.cn >> 2015-01-20 解决跨平台运行问题
* 刘一佳 << yjliu@ir.hit.edu.cn >> 2014-06-12 重组项目
* HuangFJ << biohfj@gmail.com >> 本项目最初作者

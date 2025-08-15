# 存取指令测试运行方法
运行GenData.py，会在build文件夹中生成测试用例。
```
cd load_store
python GenData.py
make asm src=samples_8bits.s
make gem5 src=samples_8bit.s trace=1
```

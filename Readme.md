# 存取指令测试运行方法

```
cd load_store
python GenData.py
make asm src=samples_8bits.s
make gem5 src=samples_8bit.s trace=1
```

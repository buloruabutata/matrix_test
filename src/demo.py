import random

from lib.load_store.test import LSTest

if __name__ == "__main__":
    # 设置随机种子确保可重现
    random.seed(42)
    test = LSTest()
    test.gen_all_asm()

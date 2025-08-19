import random
import sys

from lib.csr.test import CSRTest
from lib.load_store.test import LSTest

class_registry = {"csr": CSRTest, "load_stroe": LSTest}

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <folder_class_name>")
        sys.exit(1)

    class_name = sys.argv[1]

    # 设置随机种子确保可重现
    random.seed(42)
    # 查找并实例化
    if class_name in class_registry:
        obj = class_registry[class_name]()
        obj.gen_all_asm()
    else:
        raise ValueError(f"Unknown class: {class_name}")

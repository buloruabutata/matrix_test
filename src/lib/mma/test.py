import random
from pathlib import Path

from ..base_test import BaseTest  # 相对导入，需要作为包运行


class MMATest(BaseTest):
    ls_instr_type = ["a", "at", "b", "bt", "c", "ct", "tr", "acc"]

    def __init__(self, config_file="config.json"):
        super().__init__(Path(__file__).parent / config_file)

        for config in self.config_list:
            config["data_a"] = [
                random.randint(0, 2 ** config["width"] - 1)
                for _ in range(config["m"] * config["k"])
            ]
            config["data_b"] = [
                random.randint(0, 2 ** config["width"] - 1)
                for _ in range(config["k"] * config["n"])
            ]

    def gen_data_segment(self, config) -> str:
        segment = super().gen_data_segment(config)
        segment += self.gen_array_asm(config["data_a"], config, 0)
        segment += self.gen_array_asm(config["data_b"], config, 1)

        return segment

    def gen_text_segment(self, config) -> str:
        segment = super().gen_text_segment(config)
        # ==================== Tile操作代码部分 ====================
        segment += f"# TILE configuration for {config['width']}-bit processing\n"
        segment += f"    msettypei t0, e8\n"
        segment += f"    msettilemi t0, 0x{config['m']:X}\n"
        segment += f"    msettileki t0, 0x{config['n']:X}\n"
        segment += f"    msettileni t0, 0x{config['k']:X}\n"
        segment += f"    li t0, {config['width']}\n"
        segment += f"    la t1, array_0\n"
        segment += f"    la t2, array_1\n"

        md, ms1, ms2 = random.sample(range(0, 8), 3)
        # 目前只进行简单测试，由于t2对应的数据段为空，该操作类似于数据平移
        segment += f"    mlae16.m tr{ms1}, (t1), t0\n"
        segment += f"    mlbe16.m tr{ms2}, (t2), t0\n"
        segment += f"    mmau.h.mm acc{md}, tr{ms1}, tr{ms2}\n"

        return segment

    def gen_reference(self, config) -> str:
        ref = [0 for _ in range(config["m"] * config["n"])]
        for mi in range(config["m"]):
            for ni in range(config["n"]):
                for ki in range(config["k"]):
                    ref[mi * config["n"] + ni] += (
                        config["data_a"][mi * config["k"] + ki]
                        * config["data_b"][ki * config["n"] + ni]
                    )
        return self.gen_array_asm(ref, config, 0)

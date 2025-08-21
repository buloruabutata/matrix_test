import random
from pathlib import Path

from ..base_test import BaseTest  # 相对导入，需要作为包运行


class EWTest(BaseTest):
    ls_instr_type = ["a", "at", "b", "bt", "c", "ct", "tr", "acc"]

    def __init__(self, config_file="config.json"):
        super().__init__(Path(__file__).parent / config_file)

        for config in self.config_list:
            max_tile = max(config["m"], config["n"], config["k"])
            config["data"] = [
                random.randint(0, 2 ** config["width"] - 1)
                for _ in range(max_tile * max_tile)
            ]

    def gen_data_segment(self, config) -> str:
        segment = super().gen_data_segment(config)
        segment += self.gen_array_asm(config["data"], config, 0)

        # 创建空白区域
        segment += f"{config['name']}_store:\n"
        # .space以字节为单位，乘以8用于兼容64bit情况
        segment += f"    .space {len(config['data']) * 8}\n\n"
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
        # segment += f"    la t2, {config['name']}_load_2\n"
        segment += f"    la t2, {config['name']}_store\n\n"

        md, ms1, ms2 = random.sample(range(0, 8), 3)
        # 目前只进行简单测试，由于t2对应的数据段为空，该操作类似于数据平移
        segment += f"    mlacce8.m acc{ms1}, (t1), t0\n"
        segment += f"    mlacce8.m acc{ms2}, (t2), t0\n"
        segment += f"    maddu.b.mm acc{md}, acc{ms1}, acc{ms2}\n"

        return segment

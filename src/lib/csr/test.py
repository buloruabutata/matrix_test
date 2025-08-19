import random
from pathlib import Path

from ..base_test import BaseTest  # 相对导入，需要作为包运行


class CSRTest(BaseTest):
    tile_instr_list = ["m", "n", "k"]
    type_sew_list = ["e8", "e16", "e32", "e64"]
    type_imm_list = [
        "int4",
        "int8",
        "int16",
        "int32",
        "int64",
        "ba",
        "fp16",
        "fp32",
        "fp64",
    ]
    # LLVM 编译器不支持
    unused_imm_list = ["e4m3", "e5m2", "e3m4", "bu", "tf32", "bf16"]

    def __init__(self, config_file="config.json"):
        super().__init__(Path(__file__).parent / config_file)
        self.imm32_list = [2**i - 1 for i in range(0, 33)]
        self.imm10_list = [2**i for i in range(0, 10)]

    def gen_text_segment(self, config) -> str:
        segment = super().gen_text_segment(config)
        return segment + getattr(self, config["name"])()

    def mtype(self) -> str:
        segment = ""

        for i in self.type_sew_list:
            segment += f"    msettypei t1, {i}\n"
        segment += "\n"

        for i in self.type_imm_list:
            segment += f"    mset{i} t1, 1\n"
            segment += f"    mset{i} t1, 0\n"
        segment += "\n"

        for i in self.imm32_list:
            segment += f"    li t0, 0x{i:X}\n"
            segment += f"    msettype t1, t0\n"
        segment += "\n"

        return segment

    def mtile(self) -> str:
        segment = ""
        for i in self.imm32_list:
            segment += f"    li t0, 0x{i:X}\n"
            for item in self.tile_instr_list:
                segment += f"    msettile{item} t1, t0\n"
            segment += "\n"

        for i in self.imm10_list:
            for item in self.tile_instr_list:
                segment += f"    msettile{item}i t1, 0x{i:X}\n"
            segment += "\n"

        return segment

import random
from pathlib import Path

from ..base_test import BaseTest  # 相对导入，需要作为包运行


class LSTest(BaseTest):
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
        """
        生成RISC-V汇编数据文件，包含数据定义和tile操作代码
        参数:
        array_name: 数组名称
        data: 数据列表
        config["width"]: 数据宽度 (8, 16, 32, 64)
        output_dir: 输出目录（相对于脚本位置）
        """
        # 验证参数
        if config["width"] not in self.directive_map:
            raise ValueError(
                f"Unsupported data width: {config['width']}. Use 8, 16, 32, or 64"
            )

        if not config["data"]:
            raise ValueError("Data list cannot be empty")

        segment = super().gen_data_segment(config)
        # 数组开始标签
        segment += f"{config['name']}_load:\n"

        # 选择正确的汇编指令和格式化函数
        directive = self.directive_map[config["width"]]
        formatter = self.format_func[config["width"]]

        current_line = []
        data_per_line = 128 // config["width"]
        assert (data_per_line > 0, "data_per_line must > 0")

        for i, value in enumerate(config["data"]):
            # 验证数据范围
            max_val = (1 << config["width"]) - 1
            if value < 0 or value > max_val:
                raise ValueError(
                    f"Value {value} out of range for {config['width']}-bit"
                )

            # 使用16进制格式
            hex_value = formatter(value)
            current_line.append(hex_value)

            # 每行写入指定数量的值
            if len(current_line) >= data_per_line:
                segment += f"    {directive} {', '.join(current_line)}\n"
                current_line = []
        # 写入剩余的值
        if current_line:
            segment += f"    {directive} {', '.join(current_line)}\n\n"

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
        segment += f"    la t1, {config['name']}_load\n"
        segment += f"    la t2, {config['name']}_store\n\n"

        for instr_type in self.ls_instr_type:
            mat_reg_index = random.randint(1, 7)
            mat_reg_type = "acc" if "c" in instr_type else "tr"
            segment += f"    ml{instr_type}e{config['width']}.m \
                    {mat_reg_type}{mat_reg_index}, (t1), t0\n"
            segment += f"    ms{instr_type}e{config['width']}.m \
                    {mat_reg_type}{mat_reg_index}, (t2), t0\n"
            segment += f"    ms{instr_type}e{config['width']}.m \
                    {mat_reg_type}0, (t2), t0\n\n"
        return segment

import json
from pathlib import Path


class BaseTest:
    directive_map = {8: ".byte", 16: ".half", 32: ".word", 64: ".dword"}  # 或 .quad
    # 格式化函数映射
    format_func = {
        8: lambda x: f"0x{x & 0xFF:02X}",
        16: lambda x: f"0x{x & 0xFFFF:04X}",
        32: lambda x: f"0x{x & 0xFFFFFFFF:08X}",
        64: lambda x: f"0x{x & 0xFFFFFFFFFFFFFFFF:016X}",
    }

    def __init__(self, config_file="config.json"):
        try:
            with open(config_file, encoding="utf-8") as f:
                self.config_list = json.load(f)
        except FileNotFoundError:
            raise FileNotFoundError(
                f"配置文件 {config_file} 未找到，请检查路径是否正确。"
            )
        except json.JSONDecodeError as e:
            raise ValueError(f"配置文件格式错误，不是有效的 JSON: {e}")
        except Exception as e:
            raise RuntimeError(f"加载配置文件时发生未知错误: {e}")
        self.output_folder = self.__class__.__name__

    def gen_data_segment(self, config) -> str:
        return ".data\n"

    def gen_text_segment(self, config) -> str:
        return ".text\n.global _start\n_start:\n"

    def gen_output_dir(self):
        # 创建输出目录
        script_dir = Path(__file__).parent.parent
        output_path = script_dir / "build" / self.output_folder
        output_path.mkdir(exist_ok=True)  # 创建目录（如果不存在）
        return output_path

    def gen_one_asm(self, config):
        # 生成文件名
        output_file = self.gen_output_dir() / f"{config['name']}.s"

        # 生成汇编代码
        with open(output_file, "w") as f:
            f.write(self.gen_data_segment(config))
            f.write("\n")
            f.write(self.gen_text_segment(config))
            # 使用m5_exit 系统调用号退出程序
            f.write(f"    li a0, 0\n    li a7, 93\n    ecall\n")

    def gen_all_asm(self):
        for config in self.config_list:
            self.gen_one_asm(config)
            self.save_reference(config)

    def gen_reference(self, config) -> str:
        return ""

    def save_reference(self, config) -> str:
        ref = self.gen_reference(config)
        if ref:
            output_file = self.gen_output_dir() / f"{config['name']}.ref"
            with open(output_file, "w") as f:
                f.write(self.gen_reference(config))

    def gen_array_asm(self, array, config, index) -> str:
        # 验证参数
        if config["width"] not in self.directive_map:
            raise ValueError(
                f"Unsupported data width: {config['width']}. Use 8, 16, 32, or 64"
            )

        # 选择正确的汇编指令和格式化函数
        directive = self.directive_map[config["width"]]
        formatter = self.format_func[config["width"]]

        # 选择正确的汇编指令和格式化函数
        directive = self.directive_map[config["width"]]
        formatter = self.format_func[config["width"]]
        data_per_line = 128 // config["width"]

        current_line = []
        # 数组开始标签
        segment = f"array_{index}:\n"
        for i, value in enumerate(array):
            # 验证数据范围
            # max_val = (1 << config["width"]) - 1
            # if value < 0 or value > max_val:
            #     raise ValueError(
            #         f"Value {value} out of range for {config['width']}-bit"
            #     )

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
        return segment

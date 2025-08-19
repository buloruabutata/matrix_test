import json
from pathlib import Path


class BaseTest:
    directive_map = {8: ".byte", 16: ".half", 32: ".word", 64: ".dword"}  # 或 .quad
    # 格式化函数映射
    format_func = {
        8: lambda x: f"0x{x:02X}",
        16: lambda x: f"0x{x:04X}",
        32: lambda x: f"0x{x:08X}",
        64: lambda x: f"0x{x:016X}",
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

    def gen_one_asm(self, config):
        # 创建输出目录
        script_dir = Path(__file__).parent.parent
        output_path = script_dir / "build" / self.output_folder
        output_path.mkdir(exist_ok=True)  # 创建目录（如果不存在）

        # 生成文件名
        output_file = output_path / f"{config['name']}.s"

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

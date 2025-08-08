import json
import svgwrite
import os

def draw_instruction(instruction_index=0, json_path='instruction.json', output_dir='pic'):
    # 确保输出目录存在
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 从json文件中读取指令
    with open(json_path, 'r') as file:
        instructions = json.load(file)['instructions']
        fields_dict = instructions[instruction_index]['fields']  # 获取指定索引的指令的字段部分
        fields = [{'name': name, 'bits': bits} for name, bits in fields_dict.items()]

    padding = 20  # 内边距大小
    field_color = "lightblue"
    font_size = 14
    bit_width = 30  # 每个比特的宽度（像素）

    total_bits = sum(field['bits'] for field in fields)
    width = total_bits * bit_width + 2 * padding
    height = 80  # 调整高度以容纳字段名称和索引

    instruction_name = instructions[instruction_index]['name']
    output_path = os.path.join(output_dir, f'instruction_{instruction_name}.svg')
    dwg = svgwrite.Drawing(output_path, size=(f'{width}px', f'{height}px'))

    current_bit = 0
    for field in fields:
        width_field = field['bits'] * bit_width
        x_start = padding + current_bit * bit_width
        y_top = padding

        dwg.add(dwg.rect((x_start, y_top), (width_field, height - 2*padding), fill=field_color, stroke='black', stroke_width=1))

        text_y_position = y_top + (height - 2*padding) / 2
        dwg.add(dwg.text(field['name'], insert=(x_start + width_field/2, text_y_position),
                         font_size=str(font_size) + 'px', fill='black',
                         text_anchor="middle", dominant_baseline="central"))

        start_index = total_bits - current_bit - field['bits']
        end_index = total_bits - current_bit - 1

        inner_margin = field['bits'] * 2
        if field['bits'] >= 3:
            dwg.add(dwg.text(str(start_index), insert=(x_start + (inner_margin - 1) * width_field / inner_margin, text_y_position),
                            font_size=str(font_size) + 'px', fill='black',
                            text_anchor="middle", dominant_baseline="central"))
            
            dwg.add(dwg.text(str(end_index), insert=(x_start + width_field / inner_margin, text_y_position),
                            font_size=str(font_size) + 'px', fill='black',
                            text_anchor="middle", dominant_baseline="central"))

        current_bit += field['bits']

    dwg.save()
    print(f"具有内边距、高位在左、低位在右且索引位于字段内部并垂直居中的SVG图像已生成，对应于指令 {instructions[instruction_index]['name']}")

# 示例调用，绘制第一个指令
draw_instruction(2)
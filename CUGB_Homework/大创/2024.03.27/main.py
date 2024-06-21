import PySimpleGUI as sg
import cv2
import io
import numpy as np
import copy
from PIL import Image

# 四个方向
def direction(i):
    if i == 0:
        return "Up"
    if i == 1:
        return "Down"
    if i == 2:
        return "Right"
    if i == 3:
        return "Left"

# 主窗口
def main():

    layout = [
        [sg.Text("Please upload some images")]
    ]
    layout += [
        # 按钮名称，定义key，文件类型，按钮大小 # 空白文本，大小，定义key
        [sg.FileBrowse("Select " + direction(i) + " Image", key = "-FILE-" + str(i) + "-", file_types = (("Image Files", "*.png;*.jpg;*.jpeg"),), size = (15, 1)),
         sg.Text("", size = (40, 1), key = "-FILE_PATH-" + direction(i) + "-")] for i in range(4)
    ]
    layout += [
        [sg.Button("Process"), sg.Button("Exit")]
    ]

    window = sg.Window('Select Images', layout)

    while True:
        event, values = window.read()

        if event == sg.WINDOW_CLOSED or event == "Exit":
            break
        elif event == "Process":
            image_paths = [values["-FILE-" + str(i) + "-"] for i in range(4) if values["-FILE-" + str(i) + "-"]]
            if not image_paths:
                sg.popup("Please upload at least one image")
            else:
                process_image(values["-FILE-0-"], values["-FILE-1-"], values["-FILE-2-"], values["-FILE-3-"])

    window.close()

# 把.open读入的图片同意转化成RGBA的jpg格式
def get_jpg(input_path):
    with Image.open(input_path) as img:
        img = img.convert("RGBA")
        return img

# mse函数功能：将不同方向的图片合并
# 鲁棒性考虑：有一些图片的方向是自带在jpg格式里的，操作者旋转过后虽然图片显示的方向改变了，但是jpg的内容并没有改变
# 将用户上传的四张图片，每个方向都尝试一遍，找到契合度最高的合成图返回，避免一些错误
def mse(images, temp_images, dep, num):
    if dep == num:
        for img in temp_images:
            if img.shape != images[0].shape:
                # 尺寸对不上，相似度为0，随意返回图片，不会被调用，mse越小越相似
                return (100000, images[0])
        # 每一张图片贡献最黑的部分
        final_image_np = temp_images[0]
        for i in range(1, num):
            final_image_np = np.minimum(final_image_np, temp_images[i])
        Similarity = 100000
        # mse 越小越相似
        tmp_mse = np.mean((final_image_np - images[0]) ** 2)
        if tmp_mse < Similarity:
            Similarity = tmp_mse
        # 相似度，最终图片，和最终图片最相似的原始图片
        return (Similarity, final_image_np)
    
    Similarity = 100000
    final_image_np = images[0]

    final_arry = []
    
    # 将每张图片的四个方向都做一次尝试
    temp_images.append(images[dep])
    mse(images, temp_images, dep + 1, num)
    final_arry.append(mse(images, temp_images, dep + 1, num))

    temp_images[dep] = cv2.rotate(images[dep], cv2.ROTATE_90_CLOCKWISE)
    mse(images, temp_images, dep + 1, num)
    final_arry.append(mse(images, temp_images, dep + 1, num))

    temp_images[dep] = cv2.rotate(images[dep], cv2.ROTATE_180)
    mse(images, temp_images, dep + 1, num)
    final_arry.append(mse(images, temp_images, dep + 1, num))

    temp_images[dep] = cv2.rotate(images[dep], cv2.ROTATE_90_COUNTERCLOCKWISE)
    mse(images, temp_images, dep + 1, num)
    final_arry.append(mse(images, temp_images, dep + 1, num))

    # 找到最优合成图并返回
    for i in range(4):
        if Similarity < final_arry[i][0]:
            (Similarity, final_image_np) = final_arry[i]

    return (Similarity, final_image_np)

def line_in_image(img):

    return img

def process_image(up_path, down_path, right_path, left_path):

    final_images_np = []
    
    if up_path:
        up_image = get_jpg(up_path) 
        up_image_np = np.array(up_image)
        final_images_np.append(up_image_np)
    if down_path:
        down_image = get_jpg(down_path)
        down_image_np = np.array(down_image)
        final_images_np.append(down_image_np)
    if right_path:
        right_image = get_jpg(right_path)
        right_image_np = np.array(right_image)
        final_images_np.append(right_image_np)
    if left_path:
        left_image = get_jpg(left_path)
        left_image_np = np.array(left_image)
        final_images_np.append(left_image_np)
    
    # 得到处理方向后，合成的最终图
    (_, final_image_np) = mse(final_images_np, [final_images_np[0]], 1, len(final_images_np))

    # 深拷贝，保留副本
    temp_final_image = copy.deepcopy(final_image_np)
    # 将灰度图片处理为红白图片，仅展示孔隙区域
    final_image_gray = cv2.cvtColor(final_image_np, cv2.COLOR_BGR2GRAY)
    final_image_red = final_images_np[0]
    # 设置阈值，将较暗的区域转换为红色
    threshold = 30
    red_mask = final_image_gray < threshold
    # 将黑色和较暗的区域变为红色
    # opencv格式图片为BGR，PIL格式图片为RGB
    final_image_red[red_mask] = [0, 0, 255, 255]
    # 将其他区域变为白色
    final_image_red[~red_mask] = [255, 255, 255, 255]
    red_image_pil = Image.fromarray(cv2.cvtColor(final_image_red, cv2.COLOR_BGR2RGB))
    red_image_pil.thumbnail((600, 600))

    height, width, _ = final_image_red.shape
    # 计算灰色区域的像素占比
    total_pixels = final_image_gray.shape[0] * final_image_gray.shape[1]
    gray_pixels = np.sum(red_mask)
    gray_ratio = gray_pixels / total_pixels

    # 合成后的图片作为初始图片显示在界面
    gray_image_pil = Image.fromarray(temp_final_image)
    # gray_image_pil.save("gray.png", "PNG")
    gray_image_pil.thumbnail((600, 600))
    # 放在内存的字节流，加速
    with io.BytesIO() as output:
        gray_image_pil.save(output, format = "PNG")
        gray_bytes = output.getvalue()

    # 将图像转换为 HSV 颜色空间
    hsv_red_image = cv2.cvtColor(final_image_red, cv2.COLOR_BGR2HSV)
    lower_red = (0, 100, 100)
    upper_red = (10, 255, 255)
    # 创建红色的掩码
    temp_red_mask = cv2.inRange(hsv_red_image, lower_red, upper_red)
    # 查找红色区域的轮廓
    contours, _ = cv2.findContours(temp_red_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours = list(contours)
    contours.sort(key=lambda x: cv2.contourArea(x), reverse=True)

    content = []
    pores = []

    # 遍历每个轮廓并输出其面积和边界框
    for i, contour in enumerate(contours):
        # 计算轮廓的面积
        area = cv2.contourArea(contour)
        # 获取轮廓的边界框坐标和大小
        if area <= 0:
            break
        x, y, w, h = cv2.boundingRect(contour)
        button_text = f"Porosity {i}--{area}--({x}, {y})"
        content.append([sg.Button(button_text, key = f"-BUTTON-{i}-")])
        pores.append((x, y, area))

    file_list_column = [
        [sg.Text("Porosities id, size, coordinate")],
        [sg.Column(content, size=(200, 500), scrollable=True, key="-COLUMN-")],
        [sg.Text("This image's porosity is {:.2%}".format(gray_ratio))]
    ]

    image_viewer_column = [
        [sg.Button("Init image"), sg.Button("Only pore")],
        [sg.Image(data = gray_bytes, key = "-IMAGE-")]
    ]

    layout = [
        [
            sg.Column(file_list_column),
            sg.VSeparator(),
            sg.Column(image_viewer_column),
        ],
    ]

    init_or_red = 0

    window = sg.Window("Image product", layout)

    while True:
        event, values = window.read()
        if event == "Exit" or event == sg.WIN_CLOSED:
            break
        elif event == "Init image":
            init_or_red = 0
            with io.BytesIO() as output:
                gray_image_pil.save(output, format = "PNG")
                gray_bytes = output.getvalue()
            window["-IMAGE-"].update(data = gray_bytes)
        elif event == "Only pore":
            init_or_red = 1
            with io.BytesIO() as output:
                red_image_pil.save(output, format = "PNG")
                red_bytes = output.getvalue()
            window["-IMAGE-"].update(data = red_bytes)
        elif event.startswith("-BUTTON-"):
            pore_number = int(event.split("-")[2])
            if init_or_red == 0:
                temp_image = copy.deepcopy(final_image_gray)
            else:
                temp_image = copy.deepcopy(final_image_red)
            cv2.line(temp_image, (0, pores[pore_number][1]), (width, pores[pore_number][1]), (0, 0, 0), thickness = 2)
            cv2.line(temp_image, (pores[pore_number][0], 0), (pores[pore_number][0], height), (0, 0, 0), thickness = 2)
            temp_image_pil = Image.fromarray(cv2.cvtColor(temp_image, cv2.COLOR_BGR2RGB))
            temp_image_pil.thumbnail((600, 600))
            with io.BytesIO() as output:
                temp_image_pil.save(output, format = "PNG")
                temp_bytes = output.getvalue()
            window["-IMAGE-"].update(data = temp_bytes)
            text = '''pore_number: {}\npore_size: {}\nCentroid coordinates: ({}, {})'''.format(pore_number, pores[pore_number][2], pores[pore_number][0], pores[pore_number][1])
            sg.popup(text, title = "detail")

    window.close()
    

if __name__ == "__main__":
    main()



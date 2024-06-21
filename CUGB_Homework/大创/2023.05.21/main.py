import cv2
import numpy as np

# 读取图像，解决imread不能读取中文路径的问题
def cv_imread(file_path):
    cv_img = cv2.imdecode(np.fromfile(file_path, dtype=np.uint8), -1)
    # im decode读取的是rgb，如果后续需要opencv处理的话，需要转换成bgr，转换后图片颜色会变化
    cv_img = cv2.cvtColor(cv_img, cv2.COLOR_RGB2BGR)
    return cv_img


if __name__ == '__main__':
    path = 'D:\ME\Images\input.jpg'
    img = cv_imread(path)


# 读取图像
image = img
print(image)

# 将图像转换为灰度图像
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# 设置阈值，将较暗的区域转换为红色
threshold = 5
red_mask = gray < threshold

# 将黑色和较暗的区域变为红色
image[red_mask] = [0, 0, 255]

# 将其他区域变为白色
white_mask = ~red_mask
image[white_mask] = [255, 255, 255]

# 计算灰色区域的像素占比
total_pixels = gray.shape[0] * gray.shape[1]
gray_pixels = np.sum(red_mask)
gray_ratio = gray_pixels / total_pixels

# 显示结果
cv2.imshow('Result', image)
cv2.waitKey(0)
cv2.destroyAllWindows()
cv2.imwrite('D:\ME\Images\output.jpg', image)

# 打印灰色区域的像素占比
print("灰色区域的像素占比：{:.2%}".format(gray_ratio))

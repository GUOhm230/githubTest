import cv2
import numpy as np

# 增加高斯噪声
def gauss_noise(img):
    #读取图片
    img = img / 255.
    h, w, c = img.shape
    print(h, w, c)
    # 设置高斯分布的均值和方差
    mean = 0
    # 设置高斯分布的标准差
    sigma = random.randint(0, 9) * 0.03
    print("sigma=", sigma)
    #根据均值和标准差生成符合高斯分布的噪声
    gauss = np.random.normal(mean,sigma,(h, w, c))
    #给图片添加高斯噪声
    noise_img = img + gauss
    #设置图片添加高斯噪声之后的像素值的范围
    noise_img = np.clip(noise_img, 0, 1)
    #保存图片
    noise_img = np.uint8(noise_img*255)
    return noise_img

# 高斯噪声和椒盐噪声添加
def sp_gauss_noise():
    from PIL import Image
    from skimage import util, img_as_float, io  # 导入所需要的 skimage 库
    import os

    old_path = r"/home/lq4/SSDProject/ImageGAN/image_superResolution/NAFNet/data2add_noise"  # 原始文件路径
    save_path = r"/home/lq4/SSDProject/ImageGAN/image_superResolution/NAFNet/data_after_add_noise"  # 需要存储的文件路径

    file_list = os.walk(old_path)

    for root, dirs, files in file_list:
        for file in files:
            pic_path = os.path.join(root, file)  # 每一个图片的绝对路径
            # 读取图像
            img_org = Image.open(pic_path)
            # 转换为 skimage 可操作的格式
            img = img_as_float(img_org)

            image_gaussian = util.random_noise(img, mode="gaussian")  # 加高斯噪声
            image_sp = util.random_noise(img, mode="s&p")  # 加椒盐噪声

            # 存储文件到新的路径中，并修改文件名
            io.imsave(os.path.join(save_path, file[:-4] + "-guassian.jpg"), image_gaussian)
            io.imsave(os.path.join(save_path, file[:-4] + "-sp.jpg"), image_sp)


def sp_noise(img):

    # 设置添加椒盐噪声的数目比例
    s_vs_p = 1 # 只取0和1
    # 设置添加噪声图像像素的数目

    amount = random.randint(0, 3) * 0.03 # 到0.03
    print("amount=", amount)
    noisy_img = np.copy(img)
    # 添加salt噪声
    num_salt = np.ceil(amount * img.size * s_vs_p)
    # 设置添加噪声的坐标位置
    coords = [np.random.randint(0, i - 1, int(num_salt)) for i in img.shape]
    noisy_img[coords[0], coords[1], :] = [255, 255, 255]
    # 添加pepper噪声
    num_pepper = np.ceil(amount * img.size * (1. - s_vs_p))
    # 设置添加噪声的坐标位置
    coords = [np.random.randint(0, i - 1, int(num_pepper)) for i in img.shape]
    noisy_img[coords[0], coords[1], :] = [0, 0, 0]
    # 保存图片
    return noisy_img


def saltpepper_noise(image, proportion):
    '''
    此函数用于给图片添加椒盐噪声
    image       : 原始图片
    proportion  : 噪声比例
    '''
    image_copy = image.copy()
    # 求得其高宽
    img_Y, img_X = image.shape
    # 噪声点的 X 坐标
    X = np.random.randint(img_X, size=(int(proportion * img_X * img_Y),))
    # 噪声点的 Y 坐标
    Y = np.random.randint(img_Y, size=(int(proportion * img_X * img_Y),))
    # 噪声点的坐标赋值
    image_copy[Y, X] = np.random.choice([0, 255], size=(int(proportion * img_X * img_Y),))

    # 噪声容器
    sp_noise_plate = np.ones_like(image_copy) * 127
    # 将噪声给噪声容器
    sp_noise_plate[Y, X] = image_copy[Y, X]
    return image_copy  # 这里也会返回噪声，注意返回值



# 对上文高斯模糊的方法阐释的相关代码  -xlxw
from PIL import Image as p
# import numpy as np
# from time import clock
import math

# define
sizepic = [0, 0]
timer = [0, 0, 0, 0]
PI = 3.1415926


def getrgb(path):  # 得到图像中各个点像素的RGB三通道值
    # timer[0] = clock()
    pd = p.open(path)
    sizepic[0] = pd.size[0]
    sizepic[1] = pd.size[1]
    nr = np.empty((sizepic[0], sizepic[1]))
    ng = np.empty((sizepic[0], sizepic[1]))
    nb = np.empty((sizepic[0], sizepic[1]))
    for i in range(0, sizepic[0]):
        for j in range(0, sizepic[1]):
            nr[i][j] = pd.getpixel((i, j))[0]
            ng[i][j] = pd.getpixel((i, j))[1]
            nb[i][j] = pd.getpixel((i, j))[2]
    # print("已经得到所有像素的R,G,B的值，所花时间为{:.3f}s".format(clock() - timer[0]))
    return nr, ng, nb


def Matrixmaker(r):  # 通过半径和坐标计算高斯函数矩阵
    summat = 0
    # timer[1] = clock()
    ma = np.empty((2 * r + 1, 2 * r + 1))
    for i in range(0, 2 * r + 1):
        for j in range(0, 2 * r + 1):
            gaussp = (1 / (2 * PI * (r ** 2))) * math.e ** (-((i - r) ** 2 + (j - r) ** 2) / (2 * (r ** 2)))
            ma[i][j] = gaussp
            summat += gaussp
    # print(ma)
    print(summat)
    for i in range(0, 2 * r + 1):
        for j in range(0, 2 * r + 1):
            ma[i][j] = ma[i][j] / summat
    # print("已经计算出高斯函数矩阵，所花时间为{:.3f}s".format(clock() - timer[1]))
    print("矩阵如下:")
    return ma


def newrgb(ma, nr, ng, nb, r):  # 生成新的像素rgb矩阵
    # timer[2] = clock()
    newr = np.empty((sizepic[0], sizepic[1]))
    newg = np.empty((sizepic[0], sizepic[1]))
    newb = np.empty((sizepic[0], sizepic[1]))
    for i in range(r + 1, sizepic[0] - r):
        for j in range(r + 1, sizepic[1] - r):
            o = 0
            for x in range(i - r, i + r + 1):
                p = 0
                for y in range(j - r, j + r + 1):
                    # print("x{},y{},o{},p{}".format(x,y,o,p))
                    newr[i][j] += nr[x][y] * ma[o][p]
                    newg[i][j] += ng[x][y] * ma[o][p]
                    newb[i][j] += nb[x][y] * ma[o][p]
                    p += 1
                o += 1
    # print("已经计算出新的三通道矩阵，所花时间为{:.3f}s".format(timer[2]))
    return newr, newg, newb


def cpic(r, g, b, path, rd, save_path):
    # timer[3] = clock()
    pd = p.open(path)
    for i in range(rd + 1, sizepic[0] - rd + 1):
        for j in range(rd + 1, sizepic[1] - rd + 1):
            pd.putpixel((i, j), (int(r[i][j]), int(g[i][j]), int(b[i][j])))
    # print("已经完成生成，所花时间为{:.3f}s".format(timer[3]))
    print("正在导出图片..")
    pd.save(save_path)

def gauss_blur(rd, img_path, save_path):
    # rd = eval(input("请输入模糊的半径："))
    # path = input("请输入图片的地址(包括后缀)：")
    nr, ng, nb = getrgb(img_path)
    matx = Matrixmaker(rd)
    # print(matx)
    print("正在转换..")
    newr, newg, newb = newrgb(matx, nr, ng, nb, rd)
    print("正在准备输出..")
    cpic(newr, newg, newb, img_path, rd, save_path)
    print("{} - >> {}".format(img_path.split('/')[-1], "blurred.png"))
    # print("总计耗时:{:.3f}s,感谢您的使用.".format(timer[0] + timer[1] + timer[2] + timer[3]))

def cv2_gauss_blur(img):
    rand = random.randint(1, 6)
    print("rand=", rand)
    blur_img = cv2.GaussianBlur(img, (2*rand + 1, 2 * rand + 1), 2.5)
    return blur_img


if __name__ == "__main__":
    """
    需求：
    对图片进行随机增强：高斯白噪声，椒盐噪声以及高斯模糊
    """
    import os
    import random
    img_path = "./data2add_noise/guita_25_R.jpg"
    cv_img1 = cv2.imread(img_path)
    # rand = random.randint(1, 6)
    # dst = cv2.GaussianBlur(cv_img1, (2 * rand + 1, 2 * rand + 1), 2.5)
    # gauss_noise_img = gauss_noise(cv_img1) # 设置0-9
    # sp_noise_img = sp_noise(img_path)
    # sp_noise_img = saltpepper_noise(cv_img1, 0)
    # print(sp_noise_img.shape)
    # print("rand=", rand)
    # cv2.imshow("gauss_noise", dst)
    # cv2.waitKey()
    blur_save_path = "./data_after_add_blur/guita_25_R_5.jpg"
    img_dir = r"./data2blur"
    save_dir = r"./data_after_add_blur"
    img_num = 0
    # pre_file = ["jpg", "png"]
    method = {1: "gauss_noise",
              2: "sp_noise",
              3: "cv2_gauss_blur"}
    for root, dir, files in os.walk(img_dir):
        for file in files:
            print("---------------------------------no.{}------------------------------".format(img_num))
            img_path = os.path.join(root, file)
            print(img_path)
            cv_img = cv2.imread(img_path)
            suffix_name = os.path.basename(img_path)
            # new_suffix_name = str(img_num).zfill(4) + "_" + suffix_name[-5:]
            save_path_L = os.path.join(save_dir, suffix_name)
            save_path_R = save_path_L.replace("_L", "_R")
            print(save_path_L)
            print(save_path_R)
            img_num += 1
            function_choice = random.randint(1, 3)
            print("选定的处理方法：", method[function_choice])
            noise_img = eval(method[function_choice])(cv_img)
            cv2.imwrite(save_path_L, noise_img)
            cv2.imwrite(save_path_R, noise_img)
            # cv2.imshow("noise", noise_img)
            # cv2.waitKey()


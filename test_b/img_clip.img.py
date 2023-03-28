import cv2
import os
from PIL import  Image
def clip():
    img_path = r"/home/lq4/SSDProject/ImageGAN/image_superResolution/NAFNet/blurData"
    for root, dir, files in os.walk(img_path):
        for file in files:
            img_path = os.path.join(root, file)
            out_path = img_path.replace("blurData", "deblurResult")
            PIL_img = Image.open(img_path)
            w, h = PIL_img.size
            min_x = int(1/4*w)
            min_y = int(1/4*h)
            max_x = int(3/4*w)
            max_y = int(3/4*h)
            clip_img = PIL_img.crop((min_x, min_y, max_x, max_y))
            clip_img.save(out_path)
            print(w, h)


def dict_test():
    dict = {"det": "jkjakjska",
            "rec": "hkajskjsll"}
    for key, value in dict.items():
        print(key, "\t", value)
    print("----------------添加键值对以后----------------------------")
    dict["guo"] = "hkjkasjk"
    function = "chifan"
    function_value = "吃鸡腿"
    dict[function] = function_value
    for key, value in dict.items():
        print(key, "\t", value)

def resize_128():
    img_path = r"/home/lq4/SSDProject/ImageGAN/image_superResolution/NAFNet/blurData"
    for root, dir, files in os.walk(img_path):
        for file in files:
            img_path = os.path.join(root, file)
            out_path = img_path.replace("blurData", "blurData_resize")
            cv_img = cv2.imread(img_path)
            resize_img = cv2.resize(cv_img, (128, 128))
            cv2.imwrite(out_path, resize_img)


if __name__ == "__main__":
    clip()
# print(dict["det"])
# print(dict[function])

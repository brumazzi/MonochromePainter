import PIL, sys, os
from PIL import Image

folder = sys.argv[1]

files = os.listdir(folder)
files.sort()
count = len(files)

image = Image.open(f"{folder}/{files[0]}", "r")
width = image.width
height = image.height
image.close()

newImage = Image.new(mode = "RGBA", size = (width*count, height))

for index in range(len(files)):
    image = Image.open(f"{folder}/{files[index]}", "r")
    newImage.paste(image, (width*index, 0))
    image.close()

newImage.save(f"{folder}.png")
newImage.close()

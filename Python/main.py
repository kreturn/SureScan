import os, os.path
from PIL import Image

def batch_Image_Process_Dir( path ="D:/apps/Qt6/yc/images"):
    files = os.listdir(path)
    imageExts = [".jpg", ".png", ".gif"]
    for file in files:
        ext = os.path.splitext(file)[1]
        if ext.lower() not in imageExts:
            continue

        img = Image.open( os.path.join(path,file) )
        print(f'\nProcessing {file}')
        flipped_img = flip_upsidedown( img, file )
        grey_scale( flipped_img, file )
    print(f'\nAll files processed')

def flip_upsidedown( image, file, saveToDir="D:/apps/Qt6/yc/ImageProcesPython/processedImage" ):
    width, height = image.size
    flipped_image = Image.new("RGB", (width, height))

    for y in range(height):
        for x in range(width):
            pixel = image.getpixel((x, y))
            flipped_image.putpixel((width-1-x, height - 1 - y), pixel)

    flipped_image.save( os.path.join(saveToDir, "flipped_" + file) )
    return flipped_image


def grey_scale( image, file, saveToDir="D:/apps/Qt6/yc/ImageProcesPython/processedImage" ):
    width, height = image.size
    sum = 0.
    for y in range(height):
        for x in range(width):
            r, g, b = image.getpixel((x, y))
            gray = int(0.2989 * r + 0.5870 * g + 0.1140 * b)
            sum += gray
            image.putpixel((x, y), (gray, gray, gray))
    image.save( os.path.join(saveToDir, "grayScaled_" + file) )
    sum = sum/(height*width)
    print(f'Gray scaled file {file} has average pixel value {sum}')


if __name__ == '__main__':
    batch_Image_Process_Dir()


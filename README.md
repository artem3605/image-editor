# Image Editor

## Compilation

To compile the utility, follow these steps:
1. Make sure you have `make` and a C compiler (e.g., gcc) installed.
2. In the command line, run:

    ```bash
    make
    ```
## Usage
This command-line tool provides a set of image processing functionalities. It allows users to perform the following operations:


## 1. Display Help Information

- **Functionality**: Display help information.
- **Options**:
  - `--help` or `-h`: Display information about available options and usage.

### Usage Example:

```bash
./image_edtior -h
```

---

## 2. Print BMP File Information

- **Functionality**: Prints detailed information about a BMP file.
- **Options**:
  - `--info` or `-I`: Print BMP file information. Specify the BMP file name as an argument.

### Usage Example:

```bash
./image_edtior -I example.bmp
```
---


## 3. Invert Color in a Given Area

- **Functionality**: Inverts the color in a specified rectangular area of the image.
- **Options**:
  - `--invert` or `-i`: Invert color in the specified area.
  - `--start` or `-S`: Specify the starting coordinates (upper left) of the area as two numbers separated by a space.
  - `--end` or `-E`: Specify the ending coordinates (lower right) of the area as two numbers separated by a space.

### Usage Example:

```bash
./image_edtior -i -s x1 y1 -e x2 y2
```

---

## 4. Convert Image to Black and White

- **Functionality**: Converts the image to black and white in a specified rectangular area.
- **Options**:
  - `--convert` or `-c`: Convert to black and white in the specified area.
  - `--start` or `-S`: Specify the starting coordinates (upper left) of the area as two numbers separated by a space.
  - `--end` or `-E`: Specify the ending coordinates (lower right) of the area as two numbers separated by a space.

### Usage Example:

```bash
./image_edtior -c -s x1 y1 -e x2 y2
```

---

## 5. Resize Image

- **Functionality**: Resizes the image by either cropping it or expanding its background.
- **Options**:
  - `--resize` or `-r`: Resize the image based on specified parameters.
  - `--action` or `-a`: Specify the action (enlarge or reduce).
  - `--background` or `-b`: Specify the background color when enlarging the image.
  - `--point` or `-p`: Specify the point relative to which the action is performed (center, upper left, upper right, lower left, lower right corner).

### Usage Example:

```bash
./image_edtior -r -a enlarge -b white -p center
```

---

## 6. Draw a Segment

- **Functionality**: Draws a line segment on the image.
- **Options**:
  - `--draw` or `-d`: Draw a segment with specified parameters.
  - `--start` or `-S`: Specify the starting coordinates of the segment as two numbers separated by a space.
  - `--end` or `-E`: Specify the ending coordinates of the segment as two numbers separated by a space.
  - `--color` or `-C`: Specify the color of the segment.
  - `--thickness` or `-T`: Specify the thickness of the segment.

### Usage Example:

```bash
./image_edtior -d -s x1 y1 -e x2 y2 -clr red -t 2
```

---


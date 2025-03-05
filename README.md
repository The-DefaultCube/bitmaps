
```python
import numpy as np
import matplotlib.pyplot as plt

def read_yuv(filename, width, height, format="420", bit_depth=8):
    """
    Reads a YUV file and extracts Y, U, V planes based on format and bit depth.
    
    Parameters:
    - filename: Path to the YUV file.
    - width, height: Image resolution.
    - format: YUV format ("420", "422", "444").
    - bit_depth: Bit depth (8, 10, 12, etc.).
    
    Returns:
    - Y, U, V numpy arrays
    """
    # Compute bytes per pixel
    bytes_per_sample = 2 if bit_depth > 8 else 1
    frame_size = width * height * bytes_per_sample
    
    if format == "420":  # YUV420 (U, V at 1/4 resolution)
        uv_width, uv_height = width // 2, height // 2
    elif format == "422":  # YUV422 (U, V at 1/2 horizontal resolution)
        uv_width, uv_height = width // 2, height
    elif format == "444":  # YUV444 (U, V at full resolution)
        uv_width, uv_height = width, height
    else:
        raise ValueError("Unsupported format! Choose '420', '422', or '444'.")

    uv_size = uv_width * uv_height * bytes_per_sample  # Size of U or V plane

    with open(filename, "rb") as f:
        # Read Y plane
        y_plane = np.frombuffer(f.read(frame_size), dtype=np.uint16 if bit_depth > 8 else np.uint8)
        y_plane = y_plane.reshape((height, width))

        # Read U plane
        u_plane = np.frombuffer(f.read(uv_size), dtype=np.uint16 if bit_depth > 8 else np.uint8)
        u_plane = u_plane.reshape((uv_height, uv_width))

        # Read V plane
        v_plane = np.frombuffer(f.read(uv_size), dtype=np.uint16 if bit_depth > 8 else np.uint8)
        v_plane = v_plane.reshape((uv_height, uv_width))

    # Normalize for bit depths above 8-bit
    if bit_depth > 8:
        y_plane = (y_plane / (2 ** bit_depth - 1) * 255).astype(np.uint8)
        u_plane = (u_plane / (2 ** bit_depth - 1) * 255).astype(np.uint8)
        v_plane = (v_plane / (2 ** bit_depth - 1) * 255).astype(np.uint8)

    return y_plane, u_plane, v_plane, format

def yuv_to_rgb(y, u, v, format="420"):
    """
    Converts YUV planes to an RGB image.
    
    Parameters:
    - y, u, v: YUV planes as numpy arrays.
    - format: YUV format ("420", "422", "444").
    
    Returns:
    - RGB image as a numpy array.
    """
    height, width = y.shape

    # Upsample U and V to match Y's resolution
    if format == "420":
        u = u.repeat(2, axis=0).repeat(2, axis=1)  # Upsample 2x
        v = v.repeat(2, axis=0).repeat(2, axis=1)
    elif format == "422":
        u = u.repeat(2, axis=1)  # Upsample horizontally
        v = v.repeat(2, axis=1)

    # Convert YUV to RGB using standard formulas
    y = y.astype(np.float32)
    u = u.astype(np.float32) - 128
    v = v.astype(np.float32) - 128

    r = (y + 1.402 * v).clip(0, 255)
    g = (y - 0.344136 * u - 0.714136 * v).clip(0, 255)
    b = (y + 1.772 * u).clip(0, 255)

    rgb = np.stack([r, g, b], axis=-1).astype(np.uint8)
    
    return rgb

def display_image(image, title="YUV Image"):
    """
    Displays an RGB image using Matplotlib.
    
    Parameters:
    - image: RGB image as a numpy array.
    """
    plt.figure(figsize=(8, 6))
    plt.imshow(image)
    plt.axis("off")
    plt.title(title)
    plt.show()

# Example Usage
if __name__ == "__main__":
    yuv_filename = "input.yuv"  # Replace with your file
    width, height = 1920, 1080  # Set correct resolution
    yuv_format = "420"  # Choose from "420", "422", "444"
    bit_depth = 8  # Set bit depth (8, 10, 12, etc.)

    y, u, v, fmt = read_yuv(yuv_filename, width, height, format=yuv_format, bit_depth=bit_depth)
    rgb_image = yuv_to_rgb(y, u, v, format=fmt)
    display_image(rgb_image, title=f"YUV{yuv_format} to RGB")
```

import struct
import os

A = [ #god i have to do this for all of them
 "  xx  ",
 " x  x ",
 "x    x",
 "xxxxxx",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "      ",
 "      "
]

B = [ #god i have to do this for all of them
 "xxxxx ",
 "x   xx",
 "x    x",
 "xxxxx ",
 "x   x ",
 "x    x",
 "x    x ",
 "xxxxx ",
 "      ",
 "      "
]

C = [ #god i have to do this for all of them
 "   xxx",
 " xx   ",
 "x     ",
 "x     ",
 "x     ",
 "x     ",
 " xx   ",
 "   xxx",
 "      ",
 "      "
]

D = [ #god i have to do this for all of them
 "xxxx  ",
 "x   x ",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "x   x ",
 "xxxx  ",
 "      ",
 "      "
]

E = [ #god i have to do this for all of them
 "xxxxxx",
 "x     ",
 "x     ",
 "xxxxxx",
 "x     ",
 "x     ",
 "x     ",
 "xxxxxx",
 "      ",
 "      "
]

F = [ #god i have to do this for all of them
 "xxxxxx",
 "x     ",
 "x     ",
 "xxxxxx",
 "x     ",
 "x     ",
 "x     ",
 "x     ",
 "      ",
 "      "
]

G = [ #god i have to do this for all of them
 "   xxx",
 " xx  x",
 "x   xx",
 "xxxxxx",
 "x     ",
 "x     ",
 " xx   ",
 "   xxx",
 "      ",
 "      "
]

H = [ #god i have to do this for all of them
 "x    x",
 "x    x",
 "x    x",
 "xxxxxx",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "      ",
 "      "
]

I = [ #god i have to do this for all of them
 " xxxx ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 " xxxx ",
 "      ",
 "      "
]

J = [ #god i have to do this for all of them
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "   xx ",
 "xx xx ",
 " xxx  ",
 "      ",
 "      "
]

K = [ #god i have to do this for all of them
 "x    x",
 "x    x",
 "x  xx ",
 "xxx   ",
 "x  xx ",
 "x    x",
 "x    x",
 "x    x",
 "      ",
 "      "
]

L = [ #god i have to do this for all of them
 "x     ",
 "x     ",
 "x     ",
 "x     ",
 "x     ",
 "x     ",
 "x     ",
 "xxxxxx",
 "      ",
 "      "
]

M = [ #god i have to do this for all of them
 " x  x ",
 "x xx x",
 "x xx x",
 "x xx x",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "      ",
 "      "
]

N = [ #god i have to do this for all of them
 "xx   x",
 "x x  x",
 "x x  x",
 "x x  x",
 "x  x x",
 "x  x x",
 "x  x x",
 "x   xx",
 "      ",
 "      "
]

O = [ #god i have to do this for all of them
 "xxxxxx",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "xxxxxx",
 "      ",
 "      "
]

P = [ #god i have to do this for all of them
 "xxxxxx",
 "x    x",
 "x    x",
 "xxxxxx",
 "x     ",
 "x     ",
 "x     ",
 "x     ",
 "      ",
 "      "
]

Q = [ #god i have to do this for all of them
 "xxxxxx",
 "x    x",
 "x    x",
 "x    x",
 "x x  x",
 "x  x x",
 "x   xx",
 "xxxxxx",
 "      ",
 "      "
]

R = [ #god i have to do this for all of them
 "xxxxxx",
 "x    x",
 "x    x",
 "xxxxxx",
 "x   x ",
 "x    x",
 "x    x",
 "x    x",
 "      ",
 "      "
]

S = [ #god i have to do this for all of them
 "xxxxxx",
 "x     ",
 "x     ",
 "xx    ",
 "  xxx ",
 "     x",
 "     x",
 "xxxxxx",
 "      ",
 "      "
]

T = [ #god i have to do this for all of them
 "xxxxxx",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "      ",
 "      "
]

U = [ #god i have to do this for all of them
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "xxxxxx",
 "      ",
 "      "
]

V = [ #god i have to do this for all of them
 "x    x",
 "x    x",
 "x    x",
 " x  x ",
 " x  x ",
 " x  x ",
 " x  x ",
 "  xx  ",
 "      ",
 "      "
]

W = [ #god i have to do this for all of them
 "x xx x",
 "x xx x",
 "x xx x",
 "x xx x",
 "x xx x",
 " x  x ",
 " x  x ",
 " x  x ",
 "      ",
 "      "
]

X = [ #god i have to do this for all of them
 "x    x",
 "x    x",
 "x    x",
 "x    x",
 "x xx x",
 "x    x",
 "x    x",
 "x    x",
 "      ",
 "      "
]

Y = [ #god i have to do this for all of them
 "x    x",
 " x  x ",
 " x  x ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "  xx  ",
 "      ",
 "      "
]

Z = [ #god i have to do this for all of them
 "xxxxxx",
 "     x",
 "    x ",
 "   x  ",
 "  x   ",
 " x    ",
 "x     ",
 "xxxxxx",
 "      ",
 "      "
]

bitmap_list = [A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z]
def conv_to_bytes(char_map):
    char_rows = []
    for row in char_map:
        byte = 0
        for i, pixel in enumerate(row):
            if pixel == "x":
                byte |= (1 << (5 - i))
                char_rows.append(byte)
        return char_rows
def write_font(font_width, font_height, bitmap_list):
    font_data = []
    path = f"font.bin"
    for bitmap in bitmap_list:
        font_data.extend(conv_to_bytes(bitmap))


    with open(path, "wb") as file:
        file.write(struct.pack("BBBB", font_width, font_height, len(bitmap_list), 0))
        file.write(bytes(font_data))
    return 0

write_font(8, 8, bitmap_list)

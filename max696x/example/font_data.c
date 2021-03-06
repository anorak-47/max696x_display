#include "font_data.h"

prog_uint8_t font_data[END_CHAR-START_CHAR+1][CHAR_LEN] = {
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  SOH   1
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  STX   2
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  ETX   3
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  EOT   4
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  ENQ   5
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  ACK   6
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  BEL   7
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   BS   8
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   HT   9
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   LF  10
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   VT  11
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   FF  12
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   CR  13
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   SO  14
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   SI  15
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  DLE  16
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  DC1  17
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  DC2  18
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  DC3  19
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  DC4  20
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  NAK  21
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  SYN  22
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  ETB  23
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  CAN  24
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   EM  25
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  SUB  26
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  ESC  27
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   FS  28
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   GS  29
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   RS  30
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   US  31
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   SP  32
	{0x00, 0x00, 0x00, 0x5E, 0x00}, //    !  33
	{0x00, 0x06, 0x00, 0x06, 0x00}, //    "  34
	{0x14, 0x3E, 0x14, 0x3E, 0x14}, //    #  35
	{0x00, 0x24, 0x6A, 0x2B, 0x12}, //    $  36
	{0x4C, 0x2C, 0x10, 0x68, 0x64}, //    %  37
	{0x34, 0x4A, 0x52, 0x24, 0x10}, //    &  38
	{0x00, 0x00, 0x06, 0x00, 0x00}, //    '  39
	{0x00, 0x00, 0x3C, 0x42, 0x00}, //    (  40
	{0x00, 0x00, 0x42, 0x3C, 0x00}, //    )  41
	{0x00, 0x04, 0x0E, 0x04, 0x00}, //    *  42
	{0x08, 0x08, 0x3E, 0x08, 0x08}, //    +  43
	{0x00, 0x00, 0x40, 0x20, 0x00}, //    ,  44
	{0x00, 0x08, 0x08, 0x08, 0x08}, //    -  45
	{0x00, 0x00, 0x00, 0x20, 0x00}, //    .  46
	{0x00, 0x60, 0x18, 0x06, 0x00}, //    /  47
	{0x00, 0x3C, 0x42, 0x42, 0x3C}, //    0  48
	{0x00, 0x00, 0x04, 0x7E, 0x00}, //    1  49
	{0x00, 0x64, 0x52, 0x52, 0x4C}, //    2  50
	{0x00, 0x42, 0x4A, 0x4A, 0x34}, //    3  51
	{0x00, 0x18, 0x14, 0x7E, 0x10}, //    4  52
	{0x00, 0x4E, 0x4A, 0x4A, 0x32}, //    5  53
	{0x00, 0x3C, 0x4A, 0x4A, 0x32}, //    6  54
	{0x00, 0x02, 0x72, 0x0A, 0x06}, //    7  55
	{0x00, 0x34, 0x4A, 0x4A, 0x34}, //    8  56
	{0x00, 0x4C, 0x52, 0x52, 0x3C}, //    9  57
	{0x00, 0x00, 0x00, 0x24, 0x00}, //    :  58
	{0x00, 0x00, 0x40, 0x24, 0x00}, //    ;  59
	{0x00, 0x00, 0x08, 0x14, 0x22}, //    <  60
	{0x00, 0x14, 0x14, 0x14, 0x14}, //    =  61
	{0x00, 0x22, 0x14, 0x08, 0x00}, //    >  62
	{0x00, 0x04, 0x02, 0x52, 0x0C}, //    ?  63
	{0x00, 0x34, 0x54, 0x64, 0x38}, //    @  64
	{0x00, 0x7C, 0x0A, 0x0A, 0x7C}, //    A  65
	{0x00, 0x7E, 0x4A, 0x4A, 0x34}, //    B  66
	{0x00, 0x3C, 0x42, 0x42, 0x24}, //    C  67
	{0x00, 0x7E, 0x42, 0x42, 0x3C}, //    D  68
	{0x00, 0x3E, 0x4A, 0x4A, 0x42}, //    E  69
	{0x00, 0x7E, 0x0A, 0x0A, 0x02}, //    F  70
	{0x00, 0x3C, 0x42, 0x52, 0x32}, //    G  71
	{0x00, 0x7E, 0x08, 0x08, 0x7E}, //    H  72
	{0x00, 0x00, 0x00, 0x7E, 0x00}, //    I  73
	{0x00, 0x20, 0x40, 0x42, 0x3E}, //    J  74
	{0x00, 0x7E, 0x10, 0x28, 0x46}, //    K  75
	{0x00, 0x3E, 0x40, 0x40, 0x40}, //    L  76
	{0x7E, 0x04, 0x08, 0x04, 0x7E}, //    M  77
	{0x00, 0x7E, 0x04, 0x08, 0x7E}, //    N  78
	{0x00, 0x3C, 0x42, 0x42, 0x3C}, //    O  79
	{0x00, 0x7E, 0x0A, 0x0A, 0x04}, //    P  80
	{0x00, 0x3C, 0x42, 0x22, 0x5C}, //    Q  81
	{0x00, 0x7E, 0x12, 0x32, 0x4C}, //    R  82
	{0x00, 0x44, 0x4A, 0x4A, 0x32}, //    S  83
	{0x00, 0x02, 0x7E, 0x02, 0x00}, //    T  84
	{0x00, 0x3E, 0x40, 0x40, 0x3E}, //    U  85
	{0x00, 0x3E, 0x40, 0x20, 0x1E}, //    V  86
	{0x1E, 0x60, 0x18, 0x60, 0x1E}, //    W  87
	{0x00, 0x76, 0x08, 0x08, 0x76}, //    X  88
	{0x00, 0x46, 0x48, 0x48, 0x3E}, //    Y  89
	{0x00, 0x62, 0x52, 0x4A, 0x46}, //    Z  90
	{0x00, 0x7E, 0x42, 0x00, 0x00}, //    [  91
	{0x00, 0x06, 0x18, 0x60, 0x00}, //    \  92
	{0x00, 0x42, 0x7E, 0x00, 0x00}, //    ]  93
	{0x00, 0x04, 0x02, 0x04, 0x00}, //    ^  94
	{0x00, 0x80, 0x80, 0x80, 0x80}, //    _  95
	{0x00, 0x00, 0x01, 0x02, 0x00}, //    `  96
	{0x00, 0x30, 0x48, 0x28, 0x78}, //    a  97
	{0x00, 0x7E, 0x50, 0x48, 0x30}, //    b  98
	{0x00, 0x30, 0x48, 0x50, 0x00}, //    c  99
	{0x00, 0x30, 0x48, 0x50, 0x7E}, //    d 100
	{0x00, 0x30, 0x68, 0x58, 0x00}, //    e 101
	{0x00, 0x00, 0x7C, 0x0A, 0x00}, //    f 102
	{0x00, 0x18, 0xA4, 0xA8, 0x7C}, //    g 103
	{0x00, 0x7E, 0x10, 0x08, 0x70}, //    h 104
	{0x00, 0x00, 0x74, 0x00, 0x00}, //    i 105
	{0x00, 0x80, 0x88, 0x7A, 0x00}, //    j 106
	{0x00, 0x7E, 0x10, 0x28, 0x40}, //    k 107
	{0x00, 0x00, 0x3E, 0x40, 0x00}, //    l 108
	{0x78, 0x08, 0x70, 0x08, 0x70}, //    m 109
	{0x00, 0x78, 0x10, 0x08, 0x70}, //    n 110
	{0x00, 0x30, 0x48, 0x48, 0x30}, //    o 111
	{0x00, 0xFC, 0x28, 0x24, 0x18}, //    p 112
	{0x00, 0x18, 0x24, 0x28, 0xFC}, //    q 113
	{0x00, 0x78, 0x10, 0x08, 0x10}, //    r 114
	{0x00, 0x4C, 0x54, 0x64, 0x00}, //    s 115
	{0x00, 0x00, 0x3C, 0x48, 0x00}, //    t 116
	{0x00, 0x38, 0x40, 0x20, 0x78}, //    u 117
	{0x00, 0x38, 0x40, 0x20, 0x18}, //    v 118
	{0x38, 0x40, 0x30, 0x40, 0x38}, //    w 119
	{0x00, 0x68, 0x10, 0x68, 0x00}, //    x 120
	{0x00, 0x1C, 0xA0, 0x90, 0x7C}, //    y 121
	{0x00, 0x64, 0x54, 0x4C, 0x00}, //    z 122
	{0x00, 0x08, 0x36, 0x41, 0x00}, //    { 123
	{0x00, 0x00, 0x7F, 0x00, 0x00}, //    | 124
	{0x00, 0x41, 0x36, 0x08, 0x00}, //    } 125
	{0x00, 0x08, 0x04, 0x08, 0x04}, //    ~ 126
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  DEL 127
	{0x00, 0x10, 0x38, 0x54, 0x44}, //    ? 128
	{0x00, 0x00, 0xA0, 0x60, 0x00}, //    , 129
	{0x00, 0x00, 0xA0, 0x60, 0x00}, //    ? 130
	{0x20, 0x48, 0x3E, 0x09, 0x02}, //    ? 131
	{0xA0, 0x60, 0x00, 0xA0, 0x60}, //    ? 132
	{0x40, 0x00, 0x40, 0x00, 0x40}, //    ? 133
	{0x00, 0x04, 0x3E, 0x04, 0x00}, //    ? 134
	{0x00, 0x14, 0x3E, 0x14, 0x00}, //    ? 135
	{0x00, 0x04, 0x02, 0x04, 0x00}, //    ? 136
	{0x62, 0xB5, 0x4A, 0xA4, 0x42}, //    ? 137
	{0x00, 0x49, 0x56, 0x55, 0x24}, //    ? 138
	{0x00, 0x10, 0x28, 0x44, 0x00}, //    ? 139
	{0x38, 0x44, 0x3C, 0x54, 0x44}, //    ? 140
	{0x00, 0x00, 0x00, 0x00, 0x00}, //   RI 141
	{0x00, 0x65, 0x56, 0x55, 0x4C}, //    ? 142
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  SS3 143
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  DCS 144
	{0x00, 0x00, 0x0A, 0x06, 0x00}, //    ? 145
	{0x00, 0x00, 0x06, 0x0A, 0x00}, //    ? 146
	{0x0C, 0x0A, 0x00, 0x0C, 0x0A}, //    ? 147
	{0x0A, 0x06, 0x00, 0x0A, 0x06}, //    ? 148
	{0x00, 0x00, 0x18, 0x18, 0x00}, //    ? 149
	{0x00, 0x00, 0x10, 0x10, 0x00}, //    ? 150
	{0x00, 0x10, 0x10, 0x10, 0x10}, //    ? 151
	{0x00, 0x04, 0x02, 0x04, 0x02}, //    ? 152
	{0x04, 0x1C, 0x1C, 0x08, 0x1C}, //    ? 153
	{0x00, 0x00, 0x59, 0x6A, 0x01}, //    ? 154
	{0x00, 0x00, 0x44, 0x28, 0x10}, //    ? 155
	{0x30, 0x48, 0x30, 0x68, 0x58}, //    ? 156
	{0x00, 0x00, 0x00, 0x00, 0x00}, //  OSC 157
	{0x00, 0x01, 0x6A, 0x59, 0x00}, //    ? 158
	{0x00, 0x9A, 0xA0, 0xA2, 0x78}, //    ? 159
	{0x00, 0x00, 0x00, 0x00, 0x00}, // NBSP 160
	{0x00, 0x00, 0x00, 0x3A, 0x00}, //    � 161
	{0x00, 0x00, 0x1C, 0x36, 0x14}, //    � 162
	{0x00, 0x28, 0x3C, 0x2A, 0x22}, //    � 163
	{0x22, 0x1C, 0x14, 0x1C, 0x22}, //    � 164
	{0x14, 0x18, 0x70, 0x18, 0x14}, //    � 165
	{0x00, 0x00, 0x77, 0x00, 0x00}, //    � 166
	{0x00, 0x4A, 0x55, 0x55, 0x29}, //    � 167
	{0x00, 0x02, 0x00, 0x02, 0x00}, //    � 168
	{0x3C, 0x52, 0x6A, 0x42, 0x3C}, //    � 169
	{0x00, 0x00, 0x06, 0x0A, 0x0E}, //    � 170
	{0x08, 0x14, 0x2A, 0x14, 0x22}, //    � 171
	{0x00, 0x04, 0x04, 0x04, 0x1C}, //    � 172
	{0x00, 0x00, 0x08, 0x08, 0x00}, //    � 173
	{0x3E, 0x5D, 0x4B, 0x55, 0x3E}, //    � 174
	{0x00, 0x02, 0x02, 0x02, 0x02}, //    � 175
	{0x00, 0x00, 0x02, 0x05, 0x02}, //    � 176
	{0x00, 0x24, 0x2E, 0x24, 0x00}, //    � 177
	{0x00, 0x00, 0x0D, 0x0B, 0x00}, //    � 178
	{0x00, 0x00, 0x01, 0x0B, 0x0C}, //    � 179
	{0x00, 0x00, 0x02, 0x01, 0x00}, //    � 180
	{0x00, 0xF8, 0x40, 0x78, 0x00}, //    � 181
	{0x04, 0x0A, 0x7E, 0x02, 0x7E}, //    � 182
	{0x00, 0x00, 0x08, 0x00, 0x00}, //    � 183
	{0x00, 0x80, 0xA0, 0xE0, 0x00}, //    � 184
	{0x00, 0x00, 0x02, 0x0F, 0x00}, //    � 185
	{0x00, 0x00, 0x16, 0x15, 0x13}, //    � 186
	{0x44, 0x28, 0x54, 0x28, 0x10}, //    � 187
	{0x22, 0x17, 0x08, 0x34, 0x62}, //    � 188
	{0x22, 0x17, 0x18, 0x74, 0x42}, //    � 189
	{0x41, 0x2B, 0x14, 0x38, 0x66}, //    � 190
	{0x00, 0x30, 0x4A, 0x40, 0x20}, //    � 191
	{0x00, 0x78, 0x15, 0x16, 0x78}, //    � 192
	{0x00, 0x78, 0x16, 0x15, 0x78}, //    � 193
	{0x00, 0x7A, 0x15, 0x16, 0x78}, //    � 194
	{0x00, 0x7A, 0x15, 0x16, 0x79}, //    � 195
	{0x00, 0x79, 0x14, 0x15, 0x78}, //    � 196
	{0x00, 0x7A, 0x15, 0x15, 0x7A}, //    � 197
	{0x7C, 0x12, 0x3E, 0x4A, 0x42}, //    � 198
	{0x00, 0xBC, 0x42, 0x42, 0x24}, //    � 199
	{0x00, 0x3C, 0x55, 0x56, 0x44}, //    � 200
	{0x00, 0x3C, 0x56, 0x55, 0x44}, //    � 201
	{0x00, 0x3A, 0x55, 0x56, 0x44}, //    � 202
	{0x00, 0x3D, 0x54, 0x55, 0x44}, //    � 203
	{0x00, 0x00, 0x79, 0x02, 0x00}, //    � 204
	{0x00, 0x00, 0x7A, 0x01, 0x00}, //    � 205
	{0x00, 0x02, 0x79, 0x02, 0x00}, //    � 206
	{0x00, 0x01, 0x78, 0x01, 0x00}, //    � 207
	{0x08, 0x7E, 0x4A, 0x42, 0x3C}, //    � 208
	{0x00, 0x7A, 0x11, 0x22, 0x79}, //    � 209
	{0x00, 0x30, 0x49, 0x4A, 0x30}, //    � 210
	{0x00, 0x30, 0x4A, 0x49, 0x30}, //    � 211
	{0x00, 0x32, 0x49, 0x4A, 0x30}, //    � 212
	{0x00, 0x32, 0x49, 0x4A, 0x31}, //    � 213
	{0x00, 0x39, 0x44, 0x45, 0x38}, //    � 214
	{0x22, 0x14, 0x08, 0x14, 0x22}, //    � 215
	{0x38, 0x64, 0x54, 0x4C, 0x38}, //    � 216
	{0x00, 0x38, 0x41, 0x42, 0x38}, //    � 217
	{0x00, 0x38, 0x42, 0x41, 0x38}, //    � 218
	{0x00, 0x3A, 0x41, 0x42, 0x38}, //    � 219
	{0x00, 0x39, 0x40, 0x41, 0x38}, //    � 220
	{0x00, 0x98, 0xA2, 0xA1, 0x78}, //    � 221
	{0x00, 0x7F, 0x12, 0x12, 0x0C}, //    � 222
	{0x00, 0x7E, 0x01, 0x4D, 0x36}, //    � 223
	{0x00, 0x30, 0x49, 0x2A, 0x78}, //    � 224
	{0x00, 0x30, 0x4A, 0x29, 0x78}, //    � 225
	{0x00, 0x32, 0x49, 0x2A, 0x78}, //    � 226
	{0x00, 0x32, 0x49, 0x2A, 0x79}, //    � 227
	{0x00, 0x31, 0x48, 0x29, 0x78}, //    � 228
	{0x00, 0x32, 0x4D, 0x2A, 0x78}, //    � 229
	{0x30, 0x48, 0x38, 0x68, 0x58}, //    � 230
	{0x00, 0xB0, 0x48, 0x50, 0x00}, //    � 231
	{0x00, 0x31, 0x6A, 0x58, 0x00}, //    � 232
	{0x00, 0x32, 0x69, 0x58, 0x00}, //    � 233
	{0x00, 0x32, 0x69, 0x5A, 0x00}, //    � 234
	{0x00, 0x31, 0x68, 0x59, 0x00}, //    � 235
	{0x00, 0x00, 0x72, 0x04, 0x00}, //    � 236
	{0x00, 0x00, 0x74, 0x02, 0x00}, //    � 237
	{0x00, 0x04, 0x72, 0x04, 0x00}, //    � 238
	{0x00, 0x02, 0x70, 0x02, 0x00}, //    � 239
	{0x00, 0x30, 0x48, 0x52, 0x3C}, //    � 240
	{0x00, 0x7A, 0x11, 0x0A, 0x71}, //    � 241
	{0x00, 0x30, 0x49, 0x4A, 0x30}, //    � 242
	{0x00, 0x30, 0x4A, 0x49, 0x30}, //    � 243
	{0x00, 0x32, 0x49, 0x4A, 0x30}, //    � 244
	{0x00, 0x32, 0x49, 0x4A, 0x31}, //    � 245
	{0x00, 0x32, 0x48, 0x4A, 0x30}, //    � 246
	{0x08, 0x08, 0x2A, 0x08, 0x08}, //    � 247
	{0x00, 0x30, 0x68, 0x58, 0x30}, //    � 248
	{0x00, 0x38, 0x41, 0x22, 0x78}, //    � 249
	{0x00, 0x38, 0x42, 0x21, 0x78}, //    � 250
	{0x00, 0x3A, 0x41, 0x22, 0x78}, //    � 251
	{0x00, 0x3A, 0x40, 0x22, 0x78}, //    � 252
	{0x00, 0x18, 0xA2, 0xA1, 0x78}, //    � 253
	{0x00, 0xFE, 0x28, 0x24, 0x18}, //    � 254
	{0x00, 0x1A, 0xA0, 0xA2, 0x78}, //    � 255
};

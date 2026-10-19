// Rewritten from the target assembly (DWC release build Aug 27 2010).
#include <dwc/dwc_base64.h>

#include <string.h>

// inferred: memory copy helper of the DWC utilities (dst, src, size); no source yet
void fn_806A9CA0(void* dst, const void* src, u32 size);

static const char* sBase64Table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-";

int DWC_Base64Encode(const char* src, u32 srcLen, char* dst, u32 dstLen) {
    int n;
    u32 len = srcLen / 3 * 4 + ((srcLen % 3) != 0 ? 4 : 0);
    u32 chars;
    const char* p;
    const char* end;
    char* out;

    if (dst == NULL) {
        return len;
    }
    if (dstLen < len) {
        return -1;
    }

    p = src;
    out = dst;
    end = src + srcLen;
    while (p != end) {
        u8 buf[3];
        int rest = end - p;
        u32 bits = rest * 8;
        chars = bits / 6 + (bits % 6 != 0);
        n = (rest < 3) ? rest : 3;

        memset(buf, 0, 3);
        fn_806A9CA0(buf, p, n);

        out[0] = sBase64Table[buf[0] >> 2];
        if (chars >= 2) {
            out[1] = sBase64Table[((buf[0] << 4) & 0x30) | (buf[1] >> 4)];
        } else {
            out[1] = '*';
        }
        if (chars >= 3) {
            out[2] = sBase64Table[((buf[1] << 2) & 0x3C) | (buf[2] >> 6)];
        } else {
            out[2] = '*';
        }
        if (chars >= 4) {
            out[3] = sBase64Table[buf[2] & 0x3F];
        } else {
            out[3] = '*';
        }

        p += n;
        out += 4;
    }

    return out - dst;
}

// This file is built with -opt nostrength (see libs_ref.json): the original does not
// strength-reduce the src[j]/buf[j] indexing of the inner loop, and no source form
// reproduces that with the library's default flags (approved per-file setting).
int DWC_Base64Decode(const char* src, u32 srcLen, char* dst, u32 dstLen) {
    int len;
    int i;
    int written;
    char* out;

    if ((srcLen & 3) != 0) {
        return -1;
    }

    len = 0;
    for (i = 0; i < (int)srcLen; i++) {
        if (src[i] != '*') {
            len += 6;
        }
    }
    len /= 8;

    if (dst == NULL) {
        return len;
    }
    if (dstLen < len) {
        return -1;
    }
    if (srcLen == 0) {
        return 0;
    }

    out = dst;
    do {
        char buf[4];
        int j;
        for (j = 0; j < 4; j++) {
            switch (src[j]) {
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
                buf[j] = src[j] - 'A';
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
                buf[j] = src[j] - 'a' + 26;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                buf[j] = src[j] - '0' + 52;
                break;
            case '.':
                buf[j] = 62;
                break;
            case '-':
                buf[j] = 63;
                break;
            default:
                buf[j] = 0;
                break;
            }
        }
        src += 4;

        out[0] = (buf[0] << 2) | (buf[1] >> 4);
        written = out + 1 - dst;
        if (written >= len) {
            break;
        }
        out[1] = (buf[1] << 4) | (buf[2] >> 2);
        written = out + 2 - dst;
        if (written >= len) {
            break;
        }
        out[2] = (buf[2] << 6) | buf[3];
        out += 3;
        written = out - dst;
    } while (written < len);

    return written;
}

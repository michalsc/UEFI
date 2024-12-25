#include <efi/types.h>
#include <efi/system.h>
#include <stdarg.h>
#include <support.h>

#define LOG2_10 3.321928094887362

static double pow10_tab[] = {
    1e00, 1e01, 1e02, 1e03, 1e04, 1e05, 1e06, 1e07, 1e08,
    1e09, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17,
    1e18, 1e19, 1e20, 1e21, 1e22, 1e23, 1e24, 1e25, 1e26,
    1e27, 1e28, 1e29, 1e30, 1e31};

static double pow10_32tab[] = {
    1e00, 1e32, 1e64, 1e96, 1e128, 1e160, 1e192, 1e224, 1e256, 1e288};

static double pow10_neg32tab[] = {
    1e-00,
    1e-32,
    1e-64,
    1e-96,
    1e-128,
    1e-160,
    1e-192,
    1e-224,
    1e-256,
    1e-288,
    1e-320,
};

double my_pow10(int exp)
{
    if (exp >= 0 && exp < 308)
    {
        return (pow10_tab[exp % 32] * pow10_32tab[exp / 32]);
    }
    else if (exp >= -323 && exp < 0)
    {
        return (pow10_neg32tab[-exp / 32] / pow10_tab[(-exp) % 32]);
    }
    else
        return 0;
}

int my_log10(double v)
{
    const int maxp = 308;
    const int minp = -323;
    int min = minp;
    int max = maxp;
    int mid = (max + min) / 2;

    do
    {
        double p = my_pow10(mid);

        /* If 10^mid == v then return mid */
        if (v == p)
            return mid;
        else
        {
            /* If p > v then select lower half */
            if (p > v)
            {
                max = mid;
            }
            /* Otherwise select upper half */
            else
            {
                min = mid;
            }
            mid = (max + min) / 2;
        }
    } while ((max - min) > 1);

    return mid;
}

static int int_strlen(CHAR16 *buf)
{
    int len = 0;

    if (buf)
        while (*buf++)
            len++;

    return len;
}

static void int_itoa(CHAR16 *buf, char base, uintptr_t value, char zero_pad, int precision, int size_mod, char big, int alternate_form, int neg, char sign)
{
    int length = 0;

    do
    {
        CHAR16 c = value % base;

        if (c >= 10)
        {
            if (big)
                c += u'A' - 10;
            else
                c += u'a' - 10;
        }
        else
            c += u'0';

        value = value / base;
        buf[length++] = c;
    } while (value != 0);

    if (precision != 0)
    {
        while (length < precision)
            buf[length++] = u'0';
    }
    else if (size_mod != 0 && zero_pad)
    {
        int sz_mod = size_mod;
        if (alternate_form)
        {
            if (base == 16)
                sz_mod -= 2;
            else if (base == 8)
                sz_mod -= 1;
        }
        if (neg)
            sz_mod -= 1;

        while (length < sz_mod)
            buf[length++] = u'0';
    }
    if (alternate_form)
    {
        if (base == 8)
            buf[length++] = u'0';
        if (base == 16)
        {
            buf[length++] = big ? u'X' : u'x';
            buf[length++] = u'0';
        }
    }

    if (neg)
        buf[length++] = u'-';
    else
    {
        if (sign == u'+')
            buf[length++] = u'+';
        else if (sign == u' ')
            buf[length++] = u' ';
    }

    for (int i = 0; i < length / 2; i++)
    {
        char tmp = buf[i];
        buf[i] = buf[length - i - 1];
        buf[length - i - 1] = tmp;
    }

    buf[length] = 0;
}

static void int_ftoa(CHAR16 *buf, double value)
{
    int exp = 0;
    CHAR16 c;

    union
    {
        uint64_t u64;
        double d;
    } u;

    u.d = value;

    if ((u.u64 & 0x7ff0000000000000ULL) == 0x7ff0000000000000 && (u.u64 & 0x000fffffffffffff) != 0)
    {
        *buf++ = u'N';
        *buf++ = u'a';
        *buf++ = u'N';
        *buf++ = 0;
        return;
    }

    if ((u.u64 & 0x7fffffffffffffffULL) == 0x7ff0000000000000)
    {
        if (u.u64 & 0x8000000000000000)
            *buf++ = u'-';
        else
            *buf++ = u' ';
        *buf++ = u'I';
        *buf++ = u'n';
        *buf++ = u'f';
        *buf++ = 0;
        return;
    }

    if (value < 0)
    {
        *buf++ = u'-';
        value = -value;
    }

    if (value == 0.0)
    {
        exp = 0;
        value = 0;
    }
    else
    {
        exp = my_log10(value);
        value /= my_pow10(exp);
    }

    c = (int)value;
    *buf++ = u'0' + c;
    *buf++ = u'.';

    for (int i = 0; i < 5; i++)
    {
        value = (value - c) * 10;
        c = (int)value;
        *buf++ = u'0' + c;
    }

    *buf++ = u'E';
    if (exp < 0)
    {
        *buf++ = u'-';
        exp = -exp;
    }
    int_itoa(buf, 10, exp, 0, 0, 0, 0, 0, 0, 0);
}

void vkprintf_pc(putc_func putc_f, void *putc_data, const CHAR16 *restrict format, va_list args)
{
    CHAR16 tmpbuf[32];

    while (*format)
    {
        CHAR16 c;
        CHAR16 alternate_form = 0;
        int size_mod = 0;
        int length_mod = 0;
        int precision = 0;
        CHAR16 zero_pad = 0;
        CHAR16 *str;
        CHAR16 sign = 0;
        CHAR16 leftalign = 0;
        uintptr_t value = 0;
        intptr_t ivalue = 0;

        CHAR16 big = 0;

        c = *format++;

        if (c != u'%')
        {
            putc_f(putc_data, c);
        }
        else
        {
            c = *format++;

            if (c == u'#')
            {
                alternate_form = 1;
                c = *format++;
            }

            if (c == u'-')
            {
                leftalign = 1;
                c = *format++;
            }

            if (c == u' ' || c == u'+')
            {
                sign = c;
                c = *format++;
            }

            if (c == u'0')
            {
                zero_pad = 1;
                c = *format++;
            }

            while (c >= u'0' && c <= u'9')
            {
                size_mod = size_mod * 10;
                size_mod = size_mod + c - u'0';
                c = *format++;
            }

            if (c == u'.')
            {
                c = *format++;
                while (c >= u'0' && c <= u'9')
                {
                    precision = precision * 10;
                    precision = precision + c - u'0';
                    c = *format++;
                }
            }

            big = 0;

            if (c == u'h')
            {
                c = *format++;
                if (c == u'h')
                {
                    c = *format++;
                    length_mod = 1;
                }
                else
                    length_mod = 2;
            }
            else if (c == u'l')
            {
                c = *format++;
                if (c == u'l')
                {
                    c = *format++;
                }
                length_mod = 8;
            }
            else if (c == u'j')
            {
                c = *format++;
                length_mod = 9;
            }
            else if (c == u't')
            {
                c = *format++;
                length_mod = 10;
            }
            else if (c == u'z')
            {
                c = *format++;
                length_mod = 11;
            }
            switch (c)
            {
                case 0:
                    return;

                case u'%':
                    putc_f(putc_data, u'%');
                    break;

                case u'f':
                    int_ftoa(tmpbuf, va_arg(args, double));
                    str = tmpbuf;
                    size_mod -= int_strlen(str);
                    while (*str)
                    {
                        putc_f(putc_data, *str++);
                    }
                    break;

                case u'p':
                    value = va_arg(args, uintptr_t);
                    int_itoa(tmpbuf, 16, value, 1, 2 * sizeof(uintptr_t), 2 * sizeof(uintptr_t), big, 1, 0, sign);
                    str = tmpbuf;
                    size_mod -= int_strlen(str);
                    while (*str)
                    {
                        putc_f(putc_data, *str++);
                    }
                    break;

                case u'X':
                    big = 1;
                    /* fallthrough */
                case u'x':
                    switch (length_mod)
                    {
                        case 8:
                            value = va_arg(args, uint64_t);
                            break;
                        case 9:
                            value = va_arg(args, uintmax_t);
                            break;
                        case 10:
                            value = va_arg(args, uintptr_t);
                            break;
                        case 11:
                            value = va_arg(args, uint64_t);
                            break;
                        default:
                            value = va_arg(args, unsigned int);
                            break;
                    }
                    int_itoa(tmpbuf, 16, value, zero_pad, precision, size_mod, big, alternate_form, 0, sign);
                    str = tmpbuf;
                    size_mod -= int_strlen(str);
                    if (!leftalign)
                        while (size_mod-- > 0)
                            putc_f(putc_data, u' ');
                    while (*str)
                    {
                        putc_f(putc_data, *str++);
                    }
                    if (leftalign)
                        while (size_mod-- > 0)
                            putc_f(putc_data, u' ');
                    break;
                case u'u':
                    switch (length_mod)
                    {
                        case 8:
                            value = va_arg(args, uint64_t);
                            break;
                        case 9:
                            value = va_arg(args, uintmax_t);
                            break;
                        case 10:
                            value = va_arg(args, uintptr_t);
                            break;
                        case 11:
                            value = va_arg(args, uint64_t);
                            break;
                        default:
                            value = va_arg(args, unsigned int);
                            break;
                    }
                    int_itoa(tmpbuf, 10, value, zero_pad, precision, size_mod, 0, alternate_form, 0, sign);
                    str = tmpbuf;
                    size_mod -= int_strlen(str);
                    if (!leftalign)
                        while (size_mod-- > 0)
                            putc_f(putc_data, u' ');
                    while (*str)
                    {
                        putc_f(putc_data, *str++);
                    }
                    if (leftalign)
                        while (size_mod-- > 0)
                            putc_f(putc_data, u' ');
                    break;

                case u'd': /* fallthrough */
                case u'i':
                    switch (length_mod)
                    {
                        case 8:
                            ivalue = va_arg(args, int64_t);
                            break;
                        case 9:
                            ivalue = va_arg(args, intmax_t);
                            break;
                        case 10:
                            ivalue = va_arg(args, intptr_t);
                            break;
                        case 11:
                            ivalue = va_arg(args, uint64_t);
                            break;
                        default:
                            ivalue = va_arg(args, int);
                            break;
                    }
                    if (ivalue < 0)
                        int_itoa(tmpbuf, 10, -ivalue, zero_pad, precision, size_mod, 0, alternate_form, 1, sign);
                    else
                        int_itoa(tmpbuf, 10, ivalue, zero_pad, precision, size_mod, 0, alternate_form, 0, sign);
                    str = tmpbuf;
                    size_mod -= int_strlen(str);
                    if (!leftalign)
                        while (size_mod-- > 0)
                            putc_f(putc_data, u' ');
                    while (*str)
                    {
                        putc_f(putc_data, *str++);
                    }
                    if (leftalign)
                        while (size_mod-- > 0)
                            putc_f(putc_data, u' ');
                    break;
                case 'o':
                    switch (length_mod)
                    {
                        case 8:
                            value = va_arg(args, uint64_t);
                            break;
                        case 9:
                            value = va_arg(args, uintmax_t);
                            break;
                        case 10:
                            value = va_arg(args, uintptr_t);
                            break;
                        case 11:
                            value = va_arg(args, uint64_t);
                            break;
                        default:
                            value = va_arg(args, uint32_t);
                            break;
                    }
                    int_itoa(tmpbuf, 8, value, zero_pad, precision, size_mod, 0, alternate_form, 0, sign);
                    str = tmpbuf;
                    size_mod -= int_strlen(str);
                    if (!leftalign)
                        while (size_mod-- > 0)
                            putc_f(putc_data, u' ');
                    while (*str)
                    {
                        putc_f(putc_data, *str++);
                    }
                    if (leftalign)
                        while (size_mod-- > 0)
                            putc_f(putc_data, u' ');
                    break;

                case 'C': /* fallthrough */
                case 'c':
                    putc_f(putc_data, va_arg(args, int));
                    break;
                case 's':
                {
                    CHAR16 * str = va_arg(args, CHAR16 *);
                    do
                    {
                        if (*str == 0)
                            break;
                        else
                            putc_f(putc_data, *str);
                        size_mod--;
                    } while (*str++ && --precision);
                    while (size_mod-- > 0)
                        putc_f(putc_data, u' ');
                }
                case 'S':
                {
                    char *str = va_arg(args, char *);
                    do
                    {
                        if (*str == 0)
                            break;
                        else
                            putc_f(putc_data, *str);
                        size_mod--;
                    } while (*str++ && --precision);
                    while (size_mod-- > 0)
                        putc_f(putc_data, u' ');
                }
                break;

                default:
                    putc_f(putc_data, c);
                    break;
            }
        }
    }
    putc_f(putc_data, 0);
}

void kprintf_pc(putc_func putc_f, void *putc_data, const CHAR16 *restrict format, ...)
{
    va_list v;
    va_start(v, format);
    vkprintf_pc(putc_f, putc_data, format, v);
    va_end(v);
}

void putc_s(void *data, CHAR16 c)
{
    CHAR16 **ppchr = data;
    CHAR16 *pchr = *ppchr;
    *pchr++ = c;
    *pchr = 0;
    *ppchr = pchr;
}

void sprintf(CHAR16 *buf, const CHAR16 *restrict format, ...)
{
    va_list v;
    va_start(v, format);
    vkprintf_pc(putc_s, &buf, format, v);
    va_end(v);
}

void __sprintf_chk(char *buf, int flag, uint64_t strlen, const CHAR16 *format, ...)
{
    (void)flag;
    (void)strlen;
    va_list v;
    va_start(v, format);
    vkprintf_pc(putc_s, &buf, format, v);
    va_end(v);
}

volatile unsigned char print_lock = 0;
#define BUFSIZE 256
typedef struct {
    CHAR16 tmpbuf[BUFSIZE];
    int pos;
} PrintData;

static PrintData printData = { {0, }, 0};

extern EFI_SYSTEM_TABLE *gST;

static inline void putByte(void *data, CHAR16 chr)
{
    PrintData *pd = (PrintData *)data;
    if (chr == 0) {
        pd->tmpbuf[pd->pos] = 0;
        gST->ConOut->OutputString(gST->ConOut, pd->tmpbuf);
        pd->pos = 0;
    }
    else {
        if (pd->pos < BUFSIZE - 1) {
            pd->tmpbuf[pd->pos++] = chr;
            pd->tmpbuf[pd->pos] = 0;
        } else {
            gST->ConOut->OutputString(gST->ConOut, pd->tmpbuf);
            pd->pos = 0;
        }
    }
}

void kprintf(const CHAR16 *restrict format, ...)
{
    va_list v;
    va_start(v, format);

    while (__atomic_test_and_set(&print_lock, __ATOMIC_ACQUIRE))
        __asm__ volatile("yield");

    vkprintf_pc(putByte, (void *)&printData, format, v);

    __atomic_clear(&print_lock, __ATOMIC_RELEASE);

    va_end(v);
}

void vkprintf(const CHAR16 *restrict format, va_list args)
{
    while (__atomic_test_and_set(&print_lock, __ATOMIC_ACQUIRE))
        __asm__ volatile("yield");

    vkprintf_pc(putByte, (void *)&printData, format, args);

    __atomic_clear(&print_lock, __ATOMIC_RELEASE);
}

void *memcpy(void *restrict dst, const void *restrict src, int n)
{
    const char *s = src;
    char *d = dst;

    while (n--)
        *d++ = *s++;

    return dst;
}

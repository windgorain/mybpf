/******************************************************************************
* Copyright (C), LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2012-4-28
* Description: 
* History:     
******************************************************************************/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/num_utl.h"
#include "utl/rand_utl.h"
#include "utl/sprintf_utl.h"

const char hex_asc[] = "0123456789abcdef";

#define hex_asc_lo(x)	hex_asc[((x) & 0x0f)]
#define hex_asc_hi(x)	hex_asc[((x) & 0xf0) >> 4]


#define ZEROPAD	1		
#define SIGN	2		
#define PLUS	4		
#define SPACE	8		
#define LEFT	16		
#define SMALL	32		
#define SPECIAL	64		

#define ptrdiff_t ULONG

enum format_type {
	FORMAT_TYPE_NONE, 
	FORMAT_TYPE_WIDTH,
	FORMAT_TYPE_PRECISION,
	FORMAT_TYPE_CHAR,
	FORMAT_TYPE_STR,
	FORMAT_TYPE_PTR,
	FORMAT_TYPE_PERCENT_CHAR,
	FORMAT_TYPE_INVALID,
	FORMAT_TYPE_LONG_LONG,
	FORMAT_TYPE_ULONG,
	FORMAT_TYPE_LONG,
	FORMAT_TYPE_UBYTE,
	FORMAT_TYPE_BYTE,
	FORMAT_TYPE_USHORT,
	FORMAT_TYPE_SHORT,
	FORMAT_TYPE_UINT,
	FORMAT_TYPE_INT,
	FORMAT_TYPE_NRCHARS,
	FORMAT_TYPE_SIZE_T,
	FORMAT_TYPE_PTRDIFF
};



typedef struct {
	union {
		UCHAR		u6_addr8[16];
		USHORT		u6_addr16[8];
		UINT		u6_addr32[4];
	} in6_u;
#define sprintf_s6addr			in6_u.u6_addr8
#define sprintf_s6_addr16		in6_u.u6_addr16
#define sprintf_s6_addr32		in6_u.u6_addr32
}SPRINTF_IPV6_ADDR_S;

static inline int ipv6_addr_v4mapped(const SPRINTF_IPV6_ADDR_S *a)
{
	return (a->sprintf_s6_addr32[0] | a->sprintf_s6_addr32[1] |
		 (a->sprintf_s6_addr32[2] ^ htonl(0x0000ffff))) == 0;
}

static inline int ipv6_addr_is_isatap(const SPRINTF_IPV6_ADDR_S *addr)
{
	return (addr->sprintf_s6_addr32[2] | htonl(0x02000000)) == htonl(0x02005EFE);
}

static inline char *hex_byte_pack(char *buf, UCHAR byte)
{
	*buf++ = hex_asc_hi(byte);
	*buf++ = hex_asc_lo(byte);
	return buf;
}

static char *put_dec_trunc(char *buf, unsigned long q)
{
	unsigned d3, d2, d1, d0;
	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	d0 = 6*(d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0'; 
	d1 = q + 9*d3 + 5*d2 + d1;
	if (d1 != 0) {
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10*q;
		*buf++ = d1 + '0'; 

		d2 = q + 2*d2;
		if ((d2 != 0) || (d3 != 0)) {
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10*q;
			*buf++ = d2 + '0'; 

			d3 = q + 4*d3;
			if (d3 != 0) {
				q = (d3 * 0xcd) >> 11;
				d3 = d3 - 10*q;
				*buf++ = d3 + '0';  
				if (q != 0)
					*buf++ = (UCHAR)q + '0'; 
			}
		}
	}

	return buf;
}

static char *put_dec_full(char *buf, unsigned q)
{
	
	
	unsigned d3, d2, d1, d0;
	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	
	d0 = 6*(d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0';
	d1 = q + 9*d3 + 5*d2 + d1;
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10*q;
		*buf++ = d1 + '0';

		d2 = q + 2*d2;
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10*q;
			*buf++ = d2 + '0';

			d3 = q + 4*d3;
				q = (d3 * 0xcd) >> 11; 
				
				d3 = d3 - 10*q;
				*buf++ = d3 + '0';
					*buf++ = q + '0';

	return buf;
}

static inline char *put_dec(char *buf, unsigned long num)
{
	while (1) {
		unsigned rem;
		if (num < 100000)
			return put_dec_trunc(buf, num);
		rem = NUM_DoDiv(&num, 100000);
		buf = put_dec_full(buf, rem);
	}
}

static inline char bs_tolower(const char c)
{
	return c | 0x20;
}

static int skip_atoi(const char **s)
{
	int i = 0;

	while (isdigit(**s))
	{
		i = i*10 + *((*s)++) - '0';
	}

	return i;
}

static int format_decode(const char *fmt, struct printf_spec *spec)
{
	const char *start = fmt;

	
	if (spec->type == FORMAT_TYPE_WIDTH) {
		if (spec->field_width < 0) {
			spec->field_width = -spec->field_width;
			spec->flags |= LEFT;
		}
		spec->type = FORMAT_TYPE_NONE;
		goto precision;
	}

	
	if (spec->type == FORMAT_TYPE_PRECISION) {
		if (spec->precision < 0)
			spec->precision = 0;

		spec->type = FORMAT_TYPE_NONE;
		goto qualifier;
	}

	
	spec->type = FORMAT_TYPE_NONE;

	for (; *fmt ; ++fmt) {
		if (*fmt == '%')
			break;
	}

	
	if (fmt != start || !*fmt)
		return fmt - start;

	
	spec->flags = 0;

	while (1) { 
		BOOL_T found = TRUE;

		++fmt;

		switch (*fmt) {
		case '-': spec->flags |= LEFT;    break;
		case '+': spec->flags |= PLUS;    break;
		case ' ': spec->flags |= SPACE;   break;
		case '#': spec->flags |= SPECIAL; break;
		case '0': spec->flags |= ZEROPAD; break;
		default:  found = FALSE;
		}

		if (!found)
			break;
	}

	
	spec->field_width = -1;

	if (isdigit(*fmt))
		spec->field_width = skip_atoi(&fmt);
	else if (*fmt == '*') {
		
		spec->type = FORMAT_TYPE_WIDTH;
		return ++fmt - start;
	}

precision:
	
	spec->precision = -1;
	if (*fmt == '.') {
		++fmt;
		if (isdigit(*fmt)) {
			spec->precision = skip_atoi(&fmt);
			if (spec->precision < 0)
				spec->precision = 0;
		} else if (*fmt == '*') {
			
			spec->type = FORMAT_TYPE_PRECISION;
			return ++fmt - start;
		}
	}

qualifier:
	
	spec->qualifier = -1;
	if (*fmt == 'h' || bs_tolower(*fmt) == 'l' ||
	    bs_tolower(*fmt) == 'z' || *fmt == 't') {
		spec->qualifier = *fmt++;
		if (spec->qualifier == *fmt) {
			if (spec->qualifier == 'l') {
				spec->qualifier = 'L';
				++fmt;
			} else if (spec->qualifier == 'h') {
				spec->qualifier = 'H';
				++fmt;
			}
		}
	}

	
	spec->base = 10;
	switch (*fmt) {
	case 'c':
		spec->type = FORMAT_TYPE_CHAR;
		return ++fmt - start;

	case 's':
		spec->type = FORMAT_TYPE_STR;
		return ++fmt - start;

	case 'p':
		spec->type = FORMAT_TYPE_PTR;
		return fmt - start;
		

	case 'n':
		spec->type = FORMAT_TYPE_NRCHARS;
		return ++fmt - start;

	case '%':
		spec->type = FORMAT_TYPE_PERCENT_CHAR;
		return ++fmt - start;

	
	case 'o':
		spec->base = 8;
		break;

	case 'x':
		spec->flags |= SMALL;

	case 'X':
		spec->base = 16;
		break;

	case 'd':
	case 'i':
		spec->flags |= SIGN;
	case 'u':
		break;

	default:
		spec->type = FORMAT_TYPE_INVALID;
		return fmt - start;
	}

	if (spec->qualifier == 'L')
		spec->type = FORMAT_TYPE_LONG_LONG;
	else if (spec->qualifier == 'l') {
		if (spec->flags & SIGN)
			spec->type = FORMAT_TYPE_LONG;
		else
			spec->type = FORMAT_TYPE_ULONG;
	} else if (bs_tolower(spec->qualifier) == 'z') {
		spec->type = FORMAT_TYPE_SIZE_T;
	} else if (spec->qualifier == 't') {
		spec->type = FORMAT_TYPE_PTRDIFF;
	} else if (spec->qualifier == 'H') {
		if (spec->flags & SIGN)
			spec->type = FORMAT_TYPE_BYTE;
		else
			spec->type = FORMAT_TYPE_UBYTE;
	} else if (spec->qualifier == 'h') {
		if (spec->flags & SIGN)
			spec->type = FORMAT_TYPE_SHORT;
		else
			spec->type = FORMAT_TYPE_USHORT;
	} else {
		if (spec->flags & SIGN)
			spec->type = FORMAT_TYPE_INT;
		else
			spec->type = FORMAT_TYPE_UINT;
	}

	return ++fmt - start;
}

ULONG bs_strnlen(const char *s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		;
	return sc - s;
}

static char *string(char *buf, char *end, const char *s, struct printf_spec spec)
{
	int len, i;

	len = bs_strnlen(s, spec.precision);

	if (!(spec.flags & LEFT)) {
		while (len < spec.field_width--) {
			if (buf < end)
				*buf = ' ';
			++buf;
		}
	}
	for (i = 0; i < len; ++i) {
		if (buf < end)
			*buf = *s;
		++buf; ++s;
	}
	while (len < spec.field_width--) {
		if (buf < end)
			*buf = ' ';
		++buf;
	}

	return buf;
}

static char *number(char *buf, char *end, unsigned long long num, struct printf_spec spec)
{
	
	static const char digits[16] = "0123456789ABCDEF"; 

	char tmp[66];
	char sign;
	char locase;
	int need_pfx = ((spec.flags & SPECIAL) && spec.base != 10);
	int i;

	
	locase = (spec.flags & SMALL);
	if (spec.flags & LEFT)
		spec.flags &= ~ZEROPAD;
	sign = 0;
	if (spec.flags & SIGN) {
		if ((signed long long)num < 0) {
			sign = '-';
			num = -(signed long long)num;
			spec.field_width--;
		} else if (spec.flags & PLUS) {
			sign = '+';
			spec.field_width--;
		} else if (spec.flags & SPACE) {
			sign = ' ';
			spec.field_width--;
		}
	}
	if (need_pfx) {
		spec.field_width--;
		if (spec.base == 16)
			spec.field_width--;
	}

	
	i = 0;
	if (num == 0)
		tmp[i++] = '0';
	
	else if (spec.base != 10) { 
		int mask = spec.base - 1;
		int shift = 3;

		if (spec.base == 16)
			shift = 4;
		do {
			tmp[i++] = (digits[((unsigned char)num) & mask] | locase);
			num >>= shift;
		} while (num);
	} else { 
		i = (INT) (put_dec(tmp, (ULONG)num) - tmp);
	}

	
	if (i > spec.precision)
		spec.precision = i;
	
	spec.field_width -= spec.precision;
	if (!(spec.flags & (ZEROPAD+LEFT))) {
		while (--spec.field_width >= 0) {
			if (buf < end)
				*buf = ' ';
			++buf;
		}
	}
	
	if (sign) {
		if (buf < end)
			*buf = sign;
		++buf;
	}
	
	if (need_pfx) {
		if (buf < end)
			*buf = '0';
		++buf;
		if (spec.base == 16) {
			if (buf < end)
				*buf = ('X' | locase);
			++buf;
		}
	}
	
	if (!(spec.flags & LEFT)) {
		char c = (spec.flags & ZEROPAD) ? '0' : ' ';
		while (--spec.field_width >= 0) {
			if (buf < end)
				*buf = c;
			++buf;
		}
	}
	
	while (i <= --spec.precision) {
		if (buf < end)
			*buf = '0';
		++buf;
	}
	
	while (--i >= 0) {
		if (buf < end)
			*buf = tmp[i];
		++buf;
	}
	
	while (--spec.field_width >= 0) {
		if (buf < end)
			*buf = ' ';
		++buf;
	}

	return buf;
}

static char *mac_address_string(char *buf, char *end, UCHAR *addr,
			 struct printf_spec spec, const char *fmt)
{
	char mac_addr[sizeof("xx:xx:xx:xx:xx:xx")];
	char *p = mac_addr;
	int i;
	char separator;

	if (fmt[1] == 'F') {		
		separator = '-';
	} else {
		separator = ':';
	}

	for (i = 0; i < 6; i++) {
		p = hex_byte_pack(p, addr[i]);
		if (fmt[0] == 'M' && i != 5)
			*p++ = separator;
	}
	*p = '\0';

	return string(buf, end, mac_addr, spec);
}

static char * ip4_string(char *p, const UCHAR *addr, const char *fmt)
{
	int i;
	BOOL_T leading_zeros = (fmt[0] == 'i');
	int index;
	int step;

	switch (fmt[2]) {
	case 'h':
#if BS_BIG_ENDIAN
		index = 0;
		step = 1;
#else
		index = 3;
		step = -1;
#endif
		break;
	case 'l':
		index = 3;
		step = -1;
		break;
	case 'n':
	case 'b':
	default:
		index = 0;
		step = 1;
		break;
	}
	for (i = 0; i < 4; i++) {
		char temp[3];	
		int digits = (INT)(LONG)put_dec_trunc(temp, addr[index]) - (INT)(LONG)temp;
		if (leading_zeros) {
			if (digits < 3)
				*p++ = '0';
			if (digits < 2)
				*p++ = '0';
		}
		
		while (digits--)
			*p++ = temp[digits];
		if (i < 3)
			*p++ = '.';
		index += step;
	}
	*p = '\0';

	return p;
}

static char *ip6_compressed_string(char *p, CHAR *addr)
{
	int i, j, range;
	unsigned char zerolength[8];
	int longest = 1;
	int colonpos = -1;
	USHORT word;
	UCHAR hi, lo;
	BOOL_T needcolon = FALSE;
	BOOL_T useIPv4;
	SPRINTF_IPV6_ADDR_S in6;

	memcpy(&in6, addr, sizeof(SPRINTF_IPV6_ADDR_S));

	useIPv4 = ipv6_addr_v4mapped(&in6) || ipv6_addr_is_isatap(&in6);

	memset(zerolength, 0, sizeof(zerolength));

	if (useIPv4)
		range = 6;
	else
		range = 8;

	
	for (i = 0; i < range; i++) {
		for (j = i; j < range; j++) {
			if (in6.sprintf_s6_addr16[j] != 0)
				break;
			zerolength[i]++;
		}
	}
	for (i = 0; i < range; i++) {
		if (zerolength[i] > longest) {
			longest = zerolength[i];
			colonpos = i;
		}
	}
	if (longest == 1)		
		colonpos = -1;

	
	for (i = 0; i < range; i++) {
		if (i == colonpos) {
			if (needcolon || i == 0)
				*p++ = ':';
			*p++ = ':';
			needcolon = FALSE;
			i += longest - 1;
			continue;
		}
		if (needcolon) {
			*p++ = ':';
			needcolon = FALSE;
		}
		
		word = ntohs(in6.sprintf_s6_addr16[i]);
		hi = word >> 8;
		lo = word & 0xff;
		if (hi) {
			if (hi > 0x0f)
				p = hex_byte_pack(p, hi);
			else
				*p++ = hex_asc_lo(hi);
			p = hex_byte_pack(p, lo);
		}
		else if (lo > 0x0f)
			p = hex_byte_pack(p, lo);
		else
			*p++ = hex_asc_lo(lo);
		needcolon = TRUE;
	}

	if (useIPv4) {
		if (needcolon)
			*p++ = ':';
		p = ip4_string(p, &in6.sprintf_s6addr[12], "I4");
	}
	*p = '\0';

	return p;
}

static char *ip6_string(char *p, CHAR *addr, const char *fmt)
{
	int i;

	for (i = 0; i < 8; i++) {
		p = hex_byte_pack(p, *addr++);
		p = hex_byte_pack(p, *addr++);
		if (fmt[0] == 'I' && i != 7)
			*p++ = ':';
	}
	*p = '\0';

	return p;
}

static char *ip6_addr_string(char *buf, char *end, UCHAR *addr,
		      struct printf_spec spec, const char *fmt)
{
	char ip6_addr[sizeof("xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:255.255.255.255")];

	if (fmt[0] == 'I' && fmt[2] == 'c')
		ip6_compressed_string(ip6_addr, (CHAR*)addr);
	else
		ip6_string(ip6_addr, (CHAR*)addr, fmt);

	return string(buf, end, ip6_addr, spec);
}

static char *ip4_addr_string(char *buf, char *end, const UCHAR *addr,
		      struct printf_spec spec, const char *fmt)
{
	char ip4_addr[sizeof("255.255.255.255")];

	ip4_string(ip4_addr, addr, fmt);

	return string(buf, end, ip4_addr, spec);
}


static char *pointer(const char *fmt, char *buf, char *end, void *ptr, struct printf_spec spec)
{
	if (!ptr && *fmt != 'K') {
		
		if (spec.field_width == -1)
			spec.field_width = 2 * sizeof(void *);
		return string(buf, end, "(null)", spec);
	}

	switch (*fmt) {
	case 'M':			
	case 'm':			
					
		return mac_address_string(buf, end, (UCHAR*)ptr, spec, fmt);
	case 'I':			
	case 'i':			
		switch (fmt[1]) {
		case '6':
			return ip6_addr_string(buf, end, (UCHAR*)ptr, spec, fmt);
		case '4':
			return ip4_addr_string(buf, end, (UCHAR*)ptr, spec, fmt);
		}
		break;
	}
	spec.flags |= SMALL;
	if (spec.field_width == -1) {
		spec.field_width = 2 * sizeof(void *);
		spec.flags |= ZEROPAD;
	}
	spec.base = 16;

	return number(buf, end, (unsigned long) ptr, spec);
}

int BS_FormatCompile(FormatCompile_S *fc, char *fmt)
{
    int i = 0;
    int read;

	while ((*fmt) && (i < SPRINTF_COMPILE_ELE_NUM)){
        fc->ele[i].fmt = fmt;
		read = format_decode(fmt, &fc->ele[i].spec);
        fc->ele[i].read = read;
        fmt += read;
        i ++;
    }

    if (i >= SPRINTF_COMPILE_ELE_NUM) {
        BS_DBGASSERT(0);
        RETURN(BS_OUT_OF_RANGE);
    }

    return 0;
}

int BS_VFormat(FormatCompile_S *fc, char *buf, size_t size, va_list args)
{
	unsigned long long num;
	char *str, *end;
    char *fmt;
    int i=0;
    char *old_fmt;
    int read;
	struct printf_spec spec;

	if (size <= 0) {
		return 0;
	}

	str = buf;
	end = buf + size;

	
	if (end < buf) {
		end = (CHAR*)((void *)-1);
		size = end - buf;
	}

	while ((fc->ele[i].fmt != NULL) && (*fc->ele[i].fmt)) {
        old_fmt = fc->ele[i].fmt;
        read = fc->ele[i].read;
        fmt = old_fmt + read;
        spec = fc->ele[i].spec;
        i ++;

		switch (spec.type) {
		case FORMAT_TYPE_NONE: {
			int copy = read;
			if (str < end) {
				if (copy > end - str)
					copy = end - str;
				memcpy(str, old_fmt, copy);
			}
			str += read;
			break;
		}

		case FORMAT_TYPE_WIDTH:
			spec.field_width = va_arg(args, int);
			break;

		case FORMAT_TYPE_PRECISION:
			spec.precision = va_arg(args, int);
			break;

		case FORMAT_TYPE_CHAR: {
			char c;

			if (!(spec.flags & LEFT)) {
				while (--spec.field_width > 0) {
					if (str < end)
						*str = ' ';
					++str;

				}
			}
			c = (unsigned char) va_arg(args, int);
			if (str < end)
				*str = c;
			++str;
			while (--spec.field_width > 0) {
				if (str < end)
					*str = ' ';
				++str;
			}
			break;
		}

		case FORMAT_TYPE_STR:
			str = string(str, end, va_arg(args, char *), spec);
			break;

		case FORMAT_TYPE_PTR: {
            spec.flags |= SMALL;
            if (spec.field_width == -1) {
                spec.field_width = 2 * sizeof(void *);
                spec.flags |= ZEROPAD;
            }
            spec.base = 16;

            str = number(buf, end, (unsigned long)va_arg(args, void *), spec);
			while (isalnum(*fmt))
				fmt++;
			break;
        }

		case FORMAT_TYPE_PERCENT_CHAR:
			if (str < end)
				*str = '%';
			++str;
			break;

		case FORMAT_TYPE_INVALID:
			if (str < end)
				*str = '%';
			++str;
			break;

		case FORMAT_TYPE_NRCHARS: {
			UCHAR qualifier = spec.qualifier;

			if (qualifier == 'l') {
				long *ip = va_arg(args, long *);
				*ip = (str - buf);
			} else if (bs_tolower(qualifier) == 'z') {
				size_t *ip = va_arg(args, size_t *);
				*ip = (str - buf);
			} else {
				int *ip = va_arg(args, int *);
				*ip = (str - buf);
			}
			break;
		}

		default:
			switch (spec.type) {
			case FORMAT_TYPE_LONG_LONG:
				num = va_arg(args, long long);
				break;
			case FORMAT_TYPE_ULONG:
				num = va_arg(args, unsigned long);
				break;
			case FORMAT_TYPE_LONG:
				num = va_arg(args, long);
				break;
			case FORMAT_TYPE_SIZE_T:
				num = va_arg(args, size_t);
				break;
			case FORMAT_TYPE_PTRDIFF:
				num = va_arg(args, ptrdiff_t);
				break;
			case FORMAT_TYPE_UBYTE:
				num = (unsigned char) va_arg(args, int);
				break;
			case FORMAT_TYPE_BYTE:
				num = (signed char) va_arg(args, int);
				break;
			case FORMAT_TYPE_USHORT:
				num = (unsigned short) va_arg(args, int);
				break;
			case FORMAT_TYPE_SHORT:
				num = (short) va_arg(args, int);
				break;
			case FORMAT_TYPE_INT:
				num = (int) va_arg(args, int);
				break;
			default:
				num = va_arg(args, unsigned int);
			}

			str = number(str, end, num, spec);
		}
	}

	if (size > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
	}

	
	return str-buf;
}

int BS_Format(FormatCompile_S *fc, char * buf, ...)
{
    va_list args;
	INT iLen;

	va_start(args, buf);
	iLen = BS_VFormat(fc, buf, 0x7fffffff, args);
	va_end(args);

	return iLen;
}

int BS_FormatN(FormatCompile_S *fc, char *buf, int size, ...)
{
    va_list args;
	INT iLen;

	va_start(args, size);
	iLen = BS_VFormat(fc, buf, size, args);
	va_end(args);

	return iLen;
}


INT BS_Vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	unsigned long long num;
	char *str, *end;
	struct printf_spec spec = {0};

	if (size <= 0)
	{
		return 0;
	}

	str = buf;
	end = buf + size;

	
	if (end < buf) {
		end = (CHAR*)((void *)-1);
		size = end - buf;
	}

	while (*fmt) {
		const char *old_fmt = fmt;
		int read = format_decode(fmt, &spec);

		fmt += read;

		switch (spec.type) {
		case FORMAT_TYPE_NONE: {
			int copy = read;
			if (str < end) {
				if (copy > end - str)
					copy = end - str;
				memcpy(str, old_fmt, copy);
			}
			str += read;
			break;
		}

		case FORMAT_TYPE_WIDTH:
			spec.field_width = va_arg(args, int);
			break;

		case FORMAT_TYPE_PRECISION:
			spec.precision = va_arg(args, int);
			break;

		case FORMAT_TYPE_CHAR: {
			char c;

			if (!(spec.flags & LEFT)) {
				while (--spec.field_width > 0) {
					if (str < end)
						*str = ' ';
					++str;

				}
			}
			c = (unsigned char) va_arg(args, int);
			if (str < end)
				*str = c;
			++str;
			while (--spec.field_width > 0) {
				if (str < end)
					*str = ' ';
				++str;
			}
			break;
		}

		case FORMAT_TYPE_STR:
			str = string(str, end, va_arg(args, char *), spec);
			break;

		case FORMAT_TYPE_PTR:
			str = pointer(fmt+1, str, end, va_arg(args, void *), spec);
			while (isalnum(*fmt))
				fmt++;
			break;

		case FORMAT_TYPE_PERCENT_CHAR:
			if (str < end)
				*str = '%';
			++str;
			break;

		case FORMAT_TYPE_INVALID:
			if (str < end)
				*str = '%';
			++str;
			break;

		case FORMAT_TYPE_NRCHARS: {
			UCHAR qualifier = spec.qualifier;

			if (qualifier == 'l') {
				long *ip = va_arg(args, long *);
				*ip = (str - buf);
			} else if (bs_tolower(qualifier) == 'z') {
				size_t *ip = va_arg(args, size_t *);
				*ip = (str - buf);
			} else {
				int *ip = va_arg(args, int *);
				*ip = (str - buf);
			}
			break;
		}

		default:
			switch (spec.type) {
			case FORMAT_TYPE_LONG_LONG:
				num = va_arg(args, long long);
				break;
			case FORMAT_TYPE_ULONG:
				num = va_arg(args, unsigned long);
				break;
			case FORMAT_TYPE_LONG:
				num = va_arg(args, long);
				break;
			case FORMAT_TYPE_SIZE_T:
				num = va_arg(args, size_t);
				break;
			case FORMAT_TYPE_PTRDIFF:
				num = va_arg(args, ptrdiff_t);
				break;
			case FORMAT_TYPE_UBYTE:
				num = (unsigned char) va_arg(args, int);
				break;
			case FORMAT_TYPE_BYTE:
				num = (signed char) va_arg(args, int);
				break;
			case FORMAT_TYPE_USHORT:
				num = (unsigned short) va_arg(args, int);
				break;
			case FORMAT_TYPE_SHORT:
				num = (short) va_arg(args, int);
				break;
			case FORMAT_TYPE_INT:
				num = (int) va_arg(args, int);
				break;
			default:
				num = va_arg(args, unsigned int);
			}

			str = number(str, end, num, spec);
		}
	}

	if (size > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
	}

	
	return str-buf;

}


INT BS_Sprintf(char * buf, const char *fmt, ...)
{
    va_list args;
	INT iLen;

	va_start(args, fmt);
	iLen = BS_Vsnprintf(buf, 0x7fffffff, fmt,args);
	va_end(args);

	return iLen;
}

INT BS_Printf(const char *fmt, ...)
{
    va_list args;
	int n;
    CHAR szBuf[2048];

	va_start(args, fmt);
	n = BS_Vsnprintf(szBuf, sizeof(szBuf) - 1, fmt, args);
	va_end(args);

    szBuf[sizeof(szBuf) - 1] = '\0';

    printf("%s", szBuf);

	return n;
}

INT BS_Snprintf(char * buf, IN INT iSize, const char *fmt, ...)
{
    va_list args;
	INT iLenStored;

	va_start(args, fmt);
	iLenStored=BS_Vsnprintf(buf, iSize, fmt, args);
	va_end(args);

	return iLenStored;
}

int BS_Scnprintf(char *buf, int size, const char *fmt, ...)
{
	va_list args;
	int ret_len;

    if ((! buf) || (size <= 1)){
        return 0;
    }

	va_start(args, fmt);
	ret_len = vsnprintf(buf, size, fmt, args);
	va_end(args);

    if (ret_len >= size) {
        return size - 1;
    }

    return ret_len;
}


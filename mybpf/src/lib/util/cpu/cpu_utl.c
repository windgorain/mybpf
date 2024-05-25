/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"

#include "utl/txt_utl.h"
#include "utl/cpu_utl.h"

#ifdef __X86__
#ifdef IN_UNIXLIKE

#define cpuid(func,eax,ebx,ecx,edx)\
    __asm__ __volatile__ ("cpuid":\
    "=a" (eax),"=b" (ebx),"=c" (ecx),"=d" (edx):\
    "a" (func));

int CPU_HighestFunction()
{
    unsigned int a,b,c,d;

    cpuid(0,a,b,c,d);

    a &= 0xffff;

    return a;
}


int CPU_GetID(char *id, int size)
{
    unsigned int a,b,c,d;

    cpuid(3,a,b,c,d);
    snprintf(id, size, "%08x%08x%08x%08x", a, b, c, d);

    return 0;
}

int CPU_GetCompany(char *company, int size)
{
    char ComName[13];
    unsigned int a,b,c,d;
    int i,k;

    memset((void*)ComName,0,sizeof(ComName));
    cpuid(0,a,b,c,d);

    for(i = 0; b > 0; i++)
    {
        k = b;
        k = (k>>8);
        k = (k<<8);
        ComName[i]=b-k;
        b=(b>>8);
    }

    for(; d > 0; i++)
    {
        k = d;
        k = (k>>8);
        k = (k<<8);
        ComName[i]=d-k;
        d=(d>>8);
    }

    for(; c > 0; i++)
    {
        k = c;
        k = (k>>8);
        k = (k<<8);
        ComName[i]=c-k;
        c=(c>>8);
    }

    ComName[12]='\0';

    strlcpy(company, ComName, size);

    return 0;
}

int CPU_GetBrand(char *cBrand, int size)
{
    unsigned int a,b,c,d,i;
    unsigned int cpu_brand_buf[13];
    int k = 0;

    memset((void*)cpu_brand_buf,0,sizeof(cpu_brand_buf));

    
    cpuid(0x80000000,a,b,c,d);
    if(a < 0x80000004)
    {
        return -1;
    }

    for(i=0x80000002;i<=0x80000004;i++)
    {
        cpuid(i,a,b,c,d);
        cpu_brand_buf[k++]=a;
        cpu_brand_buf[k++]=b;
        cpu_brand_buf[k++]=c;
        cpu_brand_buf[k++]=d;
    }

    strlcpy(cBrand,(char*)cpu_brand_buf, size);

    return 0;
}

int CPU_GetBaseParam(char *baseparam, int size)
{
    unsigned int CPUBaseInfo;
    unsigned int a,b,c,d;

    cpuid(1,a,b,c,d);

    CPUBaseInfo = a;
    snprintf(baseparam, size, "Family:%d Model:%d Stepping ID:%d",
        (CPUBaseInfo & 0x0F00)>>8,(CPUBaseInfo & 0xF0)>>4,CPUBaseInfo & 0xF);

    return 0;
}

unsigned int CPU_GetModel()
{
	uint32_t a,b,c,d, family, model, ext_model, fam_mod_step;

	cpuid(0x1, a, b, c, d);
    fam_mod_step = a;

	family = (fam_mod_step >> 8) & 0xf;
	model = (fam_mod_step >> 4) & 0xf;

	if (family == 6 || family == 15) {
		ext_model = (fam_mod_step >> 16) & 0xf;
		model += (ext_model << 4);
	}

	return model;
}

#endif
#else
int CPU_GetCompany(char *company, int size)
{
    return -1;
}

int CPU_GetBrand(char *cBrand, int size)
{
    return -1;
}

int CPU_GetBaseParam(char *baseparam, int size)
{
    return -1;
}
#endif


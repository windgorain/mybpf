/*================================================================
*   Created by LiXingang
*   Description: 
*
================================================================*/
#include "bs.h"
#include "utl/file_func.h"
#include "utl/elf_utl.h"
#include "utl/elf_lib.h"
#include "utl/qsort_utl.h"
#include "utl/ulc_user.h"

#define KCONFIG_SEC ".kconfig"
#define KSYMS_SEC ".ksyms"
#define STRUCT_OPS_SEC ".struct_ops"

static int _elf_file_sec_info(ELF_S *elf, Elf64_Shdr *shdr, OUT ELF_SECTION_S *sec)
{
    if (! shdr) {
        return -1;
    }

    sec->shdr = shdr;
    sec->sec_id = ELFLIB_GetSecID(elf, shdr);
    sec->shname = ELFLIB_GetSecName(elf, shdr);
    sec->data = ELFLIB_GetSecData(elf, shdr);

    return 0;
}

static int _elf_get_sec_by_id(ELF_S *elf, int id, OUT ELF_SECTION_S *sec)
{
    Elf64_Shdr *shdr;
    shdr = ELFLIB_GetSecByID(elf, id);
    return _elf_file_sec_info(elf, shdr, sec);
}

int ELF_Open(char *path, OUT ELF_S *elf)
{
    int ret;

    BS_DBGASSERT(elf);
    BS_DBGASSERT(path);

    ret = FILE_Mem(path, elf);
    if (ret < 0) {
        return ret;
    }

    ret = ELFLIB_CheckHeader(elf);
    if (ret < 0) {
        FILE_FreeMem(elf);
    }

    return ret;
}

void ELF_Close(ELF_S *elf)
{
    FILE_FreeMem(elf);
}


void * ELF_GetNextSection(ELF_S *elf, void *iter, OUT ELF_SECTION_S *sec)
{
    Elf64_Shdr * shdr = ELFLIB_GetNextSec(elf, iter);
    _elf_file_sec_info(elf, shdr, sec);
    return shdr;
}


void * ELF_GetNextTypeSection(ELF_S *elf, int type, void *iter, OUT ELF_SECTION_S *sec)
{
    Elf64_Shdr * shdr = ELFLIB_GetNextTypeSec(elf, type, iter);
    _elf_file_sec_info(elf, shdr, sec);
    return shdr;
}


int ELF_GetSecByID(ELF_S *elf, int sec_id, OUT ELF_SECTION_S *sec)
{
    return _elf_get_sec_by_id(elf, sec_id, sec);
}

int ELF_GetSecByName(ELF_S *elf, char *sec_name, OUT ELF_SECTION_S *sec)
{
    sec->shname = sec->data = NULL;
    void *shdr = ELFLIB_GetSecByName(elf, sec_name);
    return _elf_file_sec_info(elf, shdr, sec);
}


int ELF_GetSecIDByName(ELF_S *elf, char *sec_name)
{
    return ELFLIB_GetSecIDByName(elf, sec_name);
}


Elf64_Sym * ELF_GetSymbolByID(ELF_S *elf, int id)
{
    Elf64_Shdr *sym_shdr = ELFLIB_GetSymSec(elf);
    Elf64_Sym *symbols = ELFLIB_GetSecData(elf, sym_shdr);

    if (! symbols) {
        return NULL;
    }

    int count = sym_shdr->sh_size / sizeof(Elf64_Sym);
    if (id >= count) {
        return NULL;
    }

	return &symbols[id];
}

char * ELF_GetSymbolName(ELF_S *elf, Elf64_Sym * sym)
{
    return ELFLIB_GetSymName(elf, sym);
}

BOOL_T ELF_IsProgSection(ELF_SECTION_S *sec)
{
    return ELFLIB_IsProgSection(sec->shdr);
}

BOOL_T ELF_IsDataSection(ELF_SECTION_S *sec)
{
    if (sec->shdr->sh_type != SHT_PROGBITS) {
        return FALSE;
    }

    if (sec->shdr->sh_flags & SHF_EXECINSTR) {
        return FALSE;
    }

#define DATA_SEC ".data"

    if (strcmp(sec->shname, DATA_SEC) != 0) {
        return FALSE;
    }

    return TRUE;
}

BOOL_T ELF_IsRoDataSection(ELF_SECTION_S *sec)
{
    if (sec->shdr->sh_type != SHT_PROGBITS) {
        return FALSE;
    }

    if (sec->shdr->sh_flags & SHF_EXECINSTR) {
        return FALSE;
    }

    char rodata_name[] = ".rodata";
    if (strncmp(sec->shname, rodata_name, STR_LEN(rodata_name)) != 0) {
        return FALSE;
    }

    return TRUE;
}

BOOL_T ELF_IsBssSection(ELF_SECTION_S *sec)
{
    if ((sec->shdr->sh_type == SHT_NOBITS) && (strcmp(sec->shname, ".bss") == 0)) {
        return TRUE;
    }
    return FALSE;
}


int ELF_GetGlobalData(ELF_S *elf, OUT ELF_GLOBAL_DATA_S *global_data)
{
    void *iter = NULL;
    ELF_SECTION_S the_sec;
    int count = 0;

    global_data->have_bss = 0;
    global_data->have_data = 0;
    global_data->rodata_count = 0;

    while ((iter = ELF_GetNextSection(elf, iter, &the_sec))) {
        if (ELF_IsBssSection(&the_sec)) {
            global_data->bss_sec = the_sec;
            global_data->have_bss = 1;
            count ++;
        } else if (ELF_IsDataSection(&the_sec)) {
            global_data->data_sec = the_sec;
            global_data->have_data = 1;
            count ++;
        } else if (ELF_IsRoDataSection(&the_sec)) {
            global_data->rodata_sec[global_data->rodata_count] = the_sec;
            global_data->rodata_count ++;
            count ++;
        }
    }
    
    global_data->sec_count = count;

    return count;
}


int ELF_CopyProgs(ELF_S *elf, OUT void *mem, int mem_size)
{
    ELF_SECTION_S sec;
    void *iter = NULL;
    int func_size;
    uint32_t offset = 0;
    char *out = mem;

    while ((iter = ELF_GetNextSection(elf, iter, &sec))) {
        if (! ELF_IsProgSection(&sec)) {
            continue;
        }

        func_size = sec.shdr->sh_size;
        if (offset + func_size > mem_size) {
            RETURN(BS_OUT_OF_RANGE);
        }

        memcpy(out + offset, sec.data, func_size);
        offset += func_size;
    }

    return offset;
}


void * ELF_DupProgs(ELF_S *elf)
{
    int size = ELFLIB_GetProgsSize(elf);

    if (size <= 0) {
        return NULL;
    }

    void *mem = MEM_Malloc(size);
    if (! mem) {
        return NULL;
    }

    ELF_CopyProgs(elf, mem, size);

    return mem;
}

int ELF_GetSecProgsInfoCount(ELF_PROG_INFO_S *info, int prog_count, char *sec_name)
{
    int i;
    int count = 0;

    for (i=0; i<prog_count; i++) {
        if (strcmp(info[i].sec_name, sec_name) == 0) {
            count ++;
        }
    }

    return count;
}

int ELF_WalkProgs(ELF_S *elf, PF_ELF_WALK_PROG walk_func, void *ud)
{
    ELF_SECTION_S sec;
    ELF_PROG_INFO_S info = {0};
    void *iter = NULL;
    char *func_name;
    int ret = 0;
    int i;
    int sec_offset = 0;

    while ((iter = ELF_GetNextSection(elf, iter, &sec))) {
        if (! ELF_IsProgSection(&sec)) {
            continue;
        }

        int func_count = ELFLIB_SecSymbolCount(elf, sec.sec_id, STT_FUNC);
        for (i=0; i<func_count; i++) {
            func_name = ELFLIB_GetSecSymNameByID(elf, sec.sec_id, STT_FUNC, i);
            Elf64_Sym *sym = ELFLIB_GetSecSymByID(elf, sec.sec_id, STT_FUNC, i);

            info.sec_offset = sec_offset;
            info.func_offset = sec_offset + sym->st_value;
            info.func_size = sym->st_size;
            info.sec_name = sec.shname;
            info.func_name = func_name;
            info.sec_id = sec.sec_id;

            ret = walk_func((char*)sec.data + sym->st_value, &info, ud);
            if (ret < 0) {
                break;
            }
        }

        sec_offset += sec.shdr->sh_size;
    }

    return ret;
}


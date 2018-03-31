#ifndef _SODEF_H
#define _SODEF_H

#include <elf.h>
#include <link.h>




#ifndef DT_INIT_ARRAY
#define DT_INIT_ARRAY      25
#endif

#ifndef DT_FINI_ARRAY
#define DT_FINI_ARRAY      26
#endif

#ifndef DT_INIT_ARRAYSZ
#define DT_INIT_ARRAYSZ    27
#endif

#ifndef DT_FINI_ARRAYSZ
#define DT_FINI_ARRAYSZ    28
#endif

#ifndef DT_PREINIT_ARRAY
#define DT_PREINIT_ARRAY   32
#endif

#ifndef DT_PREINIT_ARRAYSZ
#define DT_PREINIT_ARRAYSZ 33
#endif


#define SOINFO_NAME_LEN 128

typedef void (*linker_function_t)();
//================================================

typedef struct soinfo soinfo;

struct soinfo {
  char name[SOINFO_NAME_LEN];
  const ElfW(Phdr)* phdr;
  size_t phnum;
  ElfW(Addr) entry;
  ElfW(Addr) base;
  size_t size;

#ifndef __LP64__
  uint32_t unused1;  // DO NOT USE, maintained for compatibility.
#endif

  ElfW(Dyn)* dynamic;

#ifndef __LP64__
  uint32_t unused2; // DO NOT USE, maintained for compatibility
  uint32_t unused3; // DO NOT USE, maintained for compatibility
#endif

  soinfo* next;
  unsigned flags;

 private:
  const char* strtab;
 public:
  ElfW(Sym)* symtab;

  size_t nbucket;
  size_t nchain;
  unsigned* bucket;
  unsigned* chain;

#if defined(__mips__) || !defined(__LP64__)
  // This is only used by mips and mips64, but needs to be here for
  // all 32-bit architectures to preserve binary compatibility.
  ElfW(Addr)** plt_got;
#endif

#if defined(USE_RELA)
  ElfW(Rela)* plt_rela;
  size_t plt_rela_count;

  ElfW(Rela)* rela;
  size_t rela_count;
#else
  ElfW(Rel)* plt_rel;
  size_t plt_rel_count;

  ElfW(Rel)* rel;
  size_t rel_count;
#endif

  linker_function_t* preinit_array;
  size_t preinit_array_count;

  linker_function_t* init_array;
  size_t init_array_count;
  linker_function_t* fini_array;
  size_t fini_array_count;

  linker_function_t init_func;
  linker_function_t fini_func;

#if defined(__arm__)
  // ARM EABI section used for stack unwinding.
  unsigned* ARM_exidx;
  size_t ARM_exidx_count;
#elif defined(__mips__)
  unsigned mips_symtabno;
  unsigned mips_local_gotno;
  unsigned mips_gotsym;
#endif

  size_t ref_count;
  link_map link_map_head;

  bool constructors_called;

  // When you read a virtual address from the ELF file, add this
  // value to get the corresponding address in the process' address space.
  ElfW(Addr) load_bias;

#if !defined(__LP64__)
  bool has_text_relocations;
#endif
  bool has_DT_SYMBOLIC;


/*  // This part of the structure is only available
  // when FLAG_NEW_SOINFO is set in this->flags.
  uint32_t version;

  // version >= 0
  dev_t st_dev;
  ino_t st_ino;

  // dependency graph
  soinfo_list_t children;
  soinfo_list_t parents;

  // version >= 1
  off64_t file_offset;
  int rtld_flags;
  size_t strtab_size;*/
};

#endif

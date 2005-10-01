#ifndef _LD_HASH_H_
#define _LD_HASH_H_

#ifndef RTLD_NEXT
#define RTLD_NEXT	((void*)-1)
#endif

struct init_fini {
	struct elf_resolve **init_fini;
	unsigned long nlist; /* Number of entries in init_fini */
};

struct dyn_elf {
  struct elf_resolve * dyn;
  struct dyn_elf * next_handle;  /* Used by dlopen et al. */
  struct init_fini init_fini;
  struct dyn_elf * next;
  struct dyn_elf * prev;
};

struct elf_resolve {
  /* These entries must be in this order to be compatible with the interface
   * used by gdb to obtain the list of symbols. */
  ElfW(Addr) loadaddr;		/* Base address shared object is loaded at.  */
  char *libname;		/* Absolute file name object was found in.  */
  ElfW(Dyn) *dynamic_addr;	/* Dynamic section of the shared object.  */
  struct elf_resolve * next;
  struct elf_resolve * prev;
  /* Nothing after this address is used by gdb. */

#if USE_TLS
  /* Thread-local storage related info.  */

  /* Start of the initialization image.  */
  void *l_tls_initimage;
  /* Size of the initialization image.  */
  size_t l_tls_initimage_size;
  /* Size of the TLS block.  */
  size_t l_tls_blocksize;
  /* Alignment requirement of the TLS block.  */
  size_t l_tls_align;
  /* Offset of first byte module alignment.  */
  size_t l_tls_firstbyte_offset;
# ifndef NO_TLS_OFFSET
#  define NO_TLS_OFFSET	0
# endif
  /* For objects present at startup time: offset in the static TLS block.  */
  ptrdiff_t l_tls_offset;
  /* Index of the module in the dtv array.  */
  size_t l_tls_modid;
  /* Nonzero if _dl_init_static_tls should be called for this module */
  unsigned int l_need_tls_init:1;
#endif

  enum {elf_lib, elf_executable,program_interpreter, loaded_file} libtype;
  struct dyn_elf * symbol_scope;
  unsigned short usage_count;
  unsigned short int init_flag;
  unsigned long rtld_flags; /* RTLD_GLOBAL, RTLD_NOW etc. */
  Elf_Symndx nbucket;
  Elf_Symndx *elf_buckets;
  struct init_fini_list *init_fini;
  struct init_fini_list *rtld_local; /* keep tack of RTLD_LOCAL libs in same group */
  /*
   * These are only used with ELF style shared libraries
   */
  Elf_Symndx nchain;
  Elf_Symndx *chains;
  unsigned long dynamic_info[DYNAMIC_SIZE];

  unsigned long n_phent;
  ElfW(Phdr) * ppnt;

  ElfW(Addr) relro_addr;
  size_t relro_size;

  dev_t st_dev;      /* device */
  ino_t st_ino;      /* inode */

#ifdef __powerpc__
  /* this is used to store the address of relocation data words, so
   * we don't have to calculate it every time, which requires a divide */
  unsigned long data_words;
#endif
};

#define RELOCS_DONE	    0x000001
#define JMP_RELOCS_DONE	    0x000002
#define INIT_FUNCS_CALLED   0x000004
#define FINI_FUNCS_CALLED   0x000008
#define DL_OPENED	    0x000010

extern struct dyn_elf     * _dl_symbol_tables;
extern struct elf_resolve * _dl_loaded_modules;
extern struct dyn_elf 	  * _dl_handles;

extern struct elf_resolve * _dl_add_elf_hash_table(const char * libname, 
	char * loadaddr, unsigned long * dynamic_info, 
	unsigned long dynamic_addr, unsigned long dynamic_size);

extern char * _dl_find_hash(const char * name, struct dyn_elf * rpnt1, 
			    struct elf_resolve *mytpnt, int type_class);

extern int _dl_linux_dynamic_link(void);

extern char * _dl_library_path;
extern char * _dl_not_lazy;

static inline int _dl_symbol(char * name)
{
  if(name[0] != '_' || name[1] != 'd' || name[2] != 'l' || name[3] != '_')
    return 0;
  return 1;
}


#define LD_ERROR_NOFILE 1
#define LD_ERROR_NOZERO 2
#define LD_ERROR_NOTELF 3
#define LD_ERROR_NOTMAGIC 4
#define LD_ERROR_NOTDYN 5
#define LD_ERROR_MMAP_FAILED 6
#define LD_ERROR_NODYNAMIC 7
#define LD_ERROR_TLS_FAILED 8
#define LD_WRONG_RELOCS 9
#define LD_BAD_HANDLE 10
#define LD_NO_SYMBOL 11



#endif /* _LD_HASH_H_ */


#include <stdio.h>
#include <bfd.h>

/* extracted from libbfd.h */
const bfd_arch_info_type *bfd_default_compatible
   (const bfd_arch_info_type *a, const bfd_arch_info_type *b);

bfd_boolean bfd_default_scan
   (const struct bfd_arch_info *info, const char *string);

#define bfd_arch_stm8 1
#define bfd_mach_stm8 1

const bfd_arch_info_type bfd_stm8_arch =
{
  8,                           /* bits per word */
  24,                           /* bits per address */
  8,                            /* bits per byte */
  bfd_arch_stm8,                /* architecture */
  bfd_mach_stm8,                /* machine */
  "stm8",                       /* architecture name */
  "stm8",                       /* printable name */
  4,                            /* section align power */
  TRUE,                         /* the default ? */
  bfd_default_compatible,       /* architecture comparison fn */
  bfd_default_scan,             /* string to architecture convert fn */
  NULL                          /* next in list */
};

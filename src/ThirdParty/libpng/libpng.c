#include <png.c>
#include <pngerror.c>
#include <pngget.c>
#include <pngmem.c>
#include <pngpread.c>
#include <pngread.c>
#include <pngrio.c>
#include <pngrtran.c>
#include <pngrutil.c>
#include <pngset.c>
#include <pngtrans.c>

#ifdef __ARM_NEON__
#   include <arm/arm_init.c>
#   include <arm/filter_neon_intrinsics.c>
#endif

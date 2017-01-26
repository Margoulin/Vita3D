#ifndef UTILS_H
#define UTILS_H

#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/kernel/sysmem.h>

/* Misc utils */
#define ALIGN(x, a)	(((x) + ((a) - 1)) & ~((a) - 1))
#define	UNUSED(a)	(void)(a)
#define SCREEN_DPI	220

/* GPU utils */
void *gpu_alloc(SceKernelMemBlockType type, unsigned int size, unsigned int alignment, unsigned int attribs, SceUID *uid);
void gpu_free(SceUID uid);
void *vertex_usse_alloc(unsigned int size, SceUID *uid, unsigned int *usse_offset);
void vertex_usse_free(SceUID uid);
void *fragment_usse_alloc(unsigned int size, SceUID *uid, unsigned int *usse_offset);
void fragment_usse_free(SceUID uid);

#endif

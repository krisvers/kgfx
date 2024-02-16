#ifndef KRISVERS_KGFX_H
#define KRISVERS_KGFX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kgfx_types.h"
	
/*
	KGFX versions (32 bit)

	2 nibbles for major
	3 nibbles for minor
	3 nibbles for patch
	1.2.3 = 0x01002003
*/
#define KGFX_MAKE_VERSION(major, minor, patch) ((u32) (((major << 24) & 0xFF) | ((minor << 12) & 0xFFF) | (patch & 0xFFF)))
#define KGFX_ANY_MAJOR 0xFF
#define KGFX_ANY_MINOR 0xFFF
#define KGFX_ANY_PATCH 0xFFF
#define KGFX_MAJOR(version) ((version >> 24) & 0xFF)
#define KGFX_MINOR(version) ((version >> 12) & 0xFFF)
#define KGFX_PATCH(version) (version & 0xFFF)
#define KGFX_ANY_VERSION KGFX_MAKE_VERSION(KGFX_ANY_MAJOR, KGFX_ANY_MINOR, KGFX_ANY_PATCH)
#define KGFX_DEFINE_HANDLE(structure) typedef struct structure##_t* structure;
#define KGFX_MAGIC 0x8BF7E6A8
#define KGFX_HANDLE_NULL (0)

typedef enum {
	KGFX_SUCCESS = 0,
	KGFX_GENERIC_ERROR = 1,
	KGFX_VERSION_NOT_SUPPORTED = 2,
	KGFX_NULL_OUTPUT_ARGUMENT = 3,
} KGFXresult;

#define KGFX_IS_SUCCESSFUL(result) ((result) == KGFX_SUCCESS)

KGFX_DEFINE_HANDLE(KGFXcontext);

/*
	initializes a kgfx context with requested version
	any non KGFX_SUCCESS result is a failure

	the requested version is the oldest version the application requests
	future versions are compatible with the previous versions

	if the requested version is more recent than the supported version, this function will return a KGFX_VERSION_NOT_SUPPORTED error
	if the requested version is older than the supported version, this function will return a context with the implementation version
		the version is not guaranteed to be the requested version, but it will be equal or higher

	if the requested version has any of the major, minor or patch set to KGFX_ANY_MAJOR, KGFX_ANY_MINOR or KGFX_ANY_PATCH, the version that is equal or higher will be returned
*/
KGFXresult kgfxCreateContext(u32 version, KGFXcontext* context);

/* destroys a kgfx context */
void kgfxDestroyContext(KGFXcontext ctx);

/* returns implementation version */
u8 kgfxGetImplementationVersion();

#ifdef __cplusplus
}
#endif

#endif
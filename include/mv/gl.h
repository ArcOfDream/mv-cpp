#ifndef MV_USE_PURE_GLES2

#include <epoxy/gl.h>
#ifdef _WIN32
#include <epoxy/wgl.h>
#endif

#else

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#endif
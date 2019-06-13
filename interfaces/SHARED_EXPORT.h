#ifndef __SHARED_EXPORT_H__
#define __SHARED_EXPORT_H__

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define SHARED_DLL_IMPORT __declspec(dllimport)
  #define SHARED_DLL_EXPORT __declspec(dllexport)
  #define SHARED_DLL_LOCAL_VISIBILITY
#else
  #if __GNUC__ >= 4
    #define SHARED_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define SHARED_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define SHARED_DLL_LOCAL_VISIBILITY  __attribute__ ((visibility ("hidden")))
  #else
    #define SHARED_DLL_IMPORT
    #define SHARED_DLL_EXPORT
    #define SHARED_DLL_LOCAL_VISIBILITY
  #endif
#endif


#ifdef SHARED_BUILT_AS_STATIC
  #define SHARED_EXPORT
  #define SHARED_NO_EXPORT
#else /* NOT SHARED_BUILT_AS_STATIC */
  #ifndef SHARED_EXPORT
    #ifdef SHARED_EXPORTS
/* We are building this library */
      #define SHARED_EXPORT SHARED_DLL_EXPORT
    #else
/* We are using this library */
      #define SHARED_EXPORT SHARED_DLL_IMPORT
    #endif
  #endif
/* We hide code for library scope only */
  #define SHARED_LOCAL SHARED_DLL_LOCAL_VISIBILITY

  #ifndef SHARED_NO_EXPORT
    #define SHARED_NO_EXPORT
  #endif
#endif /* SHARED_BUILT_AS_STATIC */

#ifndef SHARED_DEPRECATED
  #define SHARED_DEPRECATED __declspec(deprecated)
#endif

#ifndef SHARED_DEPRECATED_EXPORT
  #define SHARED_DEPRECATED_EXPORT SHARED_EXPORT SHARED_DEPRECATED
#endif

#ifndef SHARED_DEPRECATED_NO_EXPORT
  #define SHARED_DEPRECATED_NO_EXPORT SHARED_NO_EXPORT SHARED_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
  #ifndef SHARED_NO_DEPRECATED
    #define SHARED_NO_DEPRECATED
  #endif
#endif

#endif

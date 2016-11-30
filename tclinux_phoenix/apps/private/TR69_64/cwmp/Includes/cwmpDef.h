#ifndef _CWMPDEF_H_
#define _CWMPDEF_H_


#ifndef _RPBOOLEAN_
#define _RPBOOLEAN_
#if (RpTargetOS == eRpTargetOSE)
    typedef enum { False = 0, True = 1 } Boolean;
#else
    typedef unsigned char   Boolean;
#endif
#endif

#ifndef True
    #define False           0
    #define True            1
#endif


typedef signed char         Signed8,    *Signed8Ptr;
typedef signed short        Signed16,   *Signed16Ptr;
typedef signed long         Signed32,   *Signed32Ptr;
typedef unsigned char       Unsigned8,  *Unsigned8Ptr;
typedef unsigned short      Unsigned16, *Unsigned16Ptr;
typedef unsigned long       Unsigned32, *Unsigned32Ptr;

#if AsUse64BitIntegers

/*
    The 64 bit definitions are compiler dependent.  Specific
    definitions have been provided for some compilers.  If the
    compiler being used is not recognized, the definitions
    from the ANSI C99 standard are used.

    If you are using a compiler that does not work with these
    definitions, you can add a definition specific to your
    compiler.  In this case, please notify Allegro of the
    change so that it can be included in future releases.
*/

#if defined(_MSC_VER)
typedef signed __int64      Signed64,   *Signed64Ptr;
typedef unsigned __int64    Unsigned64, *Unsigned64Ptr;
#define As64Bit(theInteger) theInteger

#elif defined(__GNUC__)
typedef signed long long    Signed64,   *Signed64Ptr;
typedef unsigned long long  Unsigned64, *Unsigned64Ptr;
#define As64Bit(theInteger) theInteger ## LL

#else
typedef signed long long    Signed64,   *Signed64Ptr;
typedef unsigned long long  Unsigned64, *Unsigned64Ptr;
#define As64Bit(theInteger) theInteger
#endif

#endif  /* AsUse64BitIntegers */

#define kWcMaxPathLength                (256)
#define kWcMaxNameLength                (64)

/*
    Server Port Constants
*/
#define kDefaultHttpPort        80
#define kDefaultTelnetPort      23
#define kDefaultIppPort         631
#define kDefaultTlsPort         443

#define SUCCESS 0
#define FAIL -1

#define RP_MEMCMP(theDestination, theSource, theLength) \
        (memcmp(theDestination, theSource, theLength))

#define RP_MEMCPY(theDestination, theSource, theLength) \
        (memcpy(theDestination, theSource, theLength))

#define RP_MEMSET(theDestination, theValue, theLength) \
        (memset(theDestination, theValue, theLength))

#define RP_STRCPY(theDestination, theSource) \
        (strcpy(theDestination, theSource))

#define RP_STRCAT(theDestination, theSource) \
        (strcat(theDestination, theSource))

#define RP_STRCMP(theFirstString, theSecondString) \
        (strcmp(theFirstString, theSecondString))

#define RP_STRNCMP(theFirstString, theSecondString, theLength) \
        (strncmp(theFirstString, theSecondString, theLength))

#define RP_STRLEN(theString) \
        (strlen(theString))

#define RP_ALLOC(theLength) \
        (os_malloc(theLength))

#define RP_FREE(theMemoryBlockPtr) \
        (os_free(theMemoryBlockPtr))

#define RP_ATOL(theStringPtr) \
        (atol(theStringPtr))

#define RP_ATOI(theStringPtr) \
        (atoi(theStringPtr))

#define RP_REALLOC(theMemoryBlockPtr, theNewLength) \
        (realloc(theMemoryBlockPtr, theNewLength))

#define RP_OFFSETOF(type, member) \
        (offsetof(type, member))

/*
    Web Client status codes.
*/
typedef enum {
    eWcRequestIdle,
    eWcRequestBusy,
    eWcRequestNotification,
    eWcRequestComplete,
    eWcRequestCompletePersistent,
    eWcRequestCancelled,
    eWcRequestFree,
    eWcProxySet,
    eWcUserInfoSet,
    eWcUserAgentSet,
    eWcHeadersSet,
    eWcInvalidRequest,
    eWcWorkBufferOverflow,
    eWcBufferOverflow,
    eWcAbortNoServer,
    eWcFileError,
    eWcAbortConnectionError,
    eWcReceiveIncomplete,
    eWcRequestAbortGeneral
} wcStatus;

#endif



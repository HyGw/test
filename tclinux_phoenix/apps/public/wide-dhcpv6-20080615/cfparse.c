/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 31 "cfparse.y"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/time.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"

extern int lineno;
extern int cfdebug;

extern void yywarn __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));
extern void yyerror __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));

#define MAKE_NAMELIST(l, n, p) do { \
	(l) = (struct cf_namelist *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (p) cleanup_cflist(p); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->name = (n); \
	l->params = (p); \
	} while (0)

#define MAKE_CFLIST(l, t, pp, pl) do { \
	(l) = (struct cf_list *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (pp) free(pp); \
		if (pl) cleanup_cflist(pl); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->type = (t); \
	l->ptr = (pp); \
	l->list = (pl); \
	l->tail = (l); \
	} while (0)

static struct cf_namelist *iflist_head, *hostlist_head, *iapdlist_head;
static struct cf_namelist *addrpoollist_head;
static struct cf_namelist *authinfolist_head, *keylist_head;
static struct cf_namelist *ianalist_head;
struct cf_list *cf_dns_list, *cf_dns_name_list, *cf_ntp_list;
struct cf_list *cf_sip_list, *cf_sip_name_list;
struct cf_list *cf_nis_list, *cf_nis_name_list;
struct cf_list *cf_nisp_list, *cf_nisp_name_list;
struct cf_list *cf_bcmcs_list, *cf_bcmcs_name_list;
long long cf_refreshtime = -1;

extern int yylex __P((void));
extern int cfswitch_buffer __P((char *));
static int add_namelist __P((struct cf_namelist *, struct cf_namelist **));
static void cleanup __P((void));
static void cleanup_namelist __P((struct cf_namelist *));
static void cleanup_cflist __P((struct cf_list *));


/* Line 268 of yacc.c  */
#line 145 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTERFACE = 258,
     IFNAME = 259,
     PREFIX_INTERFACE = 260,
     SLA_ID = 261,
     SLA_LEN = 262,
     DUID_ID = 263,
     ID_ASSOC = 264,
     IA_PD = 265,
     IAID = 266,
     IA_NA = 267,
     ADDRESS = 268,
     REQUEST = 269,
     SEND = 270,
     ALLOW = 271,
     PREFERENCE = 272,
     HOST = 273,
     HOSTNAME = 274,
     DUID = 275,
     OPTION = 276,
     RAPID_COMMIT = 277,
     DNS_SERVERS = 278,
     DNS_NAME = 279,
     VENDOR_OPTS = 280,
     DSLITE_NAME = 281,
     DSLITE_ADDRESS = 282,
     NTP_SERVERS = 283,
     REFRESHTIME = 284,
     SIP_SERVERS = 285,
     SIP_NAME = 286,
     NIS_SERVERS = 287,
     NIS_NAME = 288,
     NISP_SERVERS = 289,
     NISP_NAME = 290,
     BCMCS_SERVERS = 291,
     BCMCS_NAME = 292,
     INFO_ONLY = 293,
     SCRIPT = 294,
     DELAYEDKEY = 295,
     AUTHENTICATION = 296,
     PROTOCOL = 297,
     ALGORITHM = 298,
     DELAYED = 299,
     RECONFIG = 300,
     HMACMD5 = 301,
     MONOCOUNTER = 302,
     AUTHNAME = 303,
     RDM = 304,
     KEY = 305,
     KEYINFO = 306,
     REALM = 307,
     KEYID = 308,
     SECRET = 309,
     KEYNAME = 310,
     EXPIRE = 311,
     ADDRPOOL = 312,
     POOLNAME = 313,
     RANGE = 314,
     TO = 315,
     ADDRESS_POOL = 316,
     INCLUDE = 317,
     NUMBER = 318,
     SLASH = 319,
     EOS = 320,
     BCL = 321,
     ECL = 322,
     STRING = 323,
     QSTRING = 324,
     PREFIX = 325,
     INFINITY = 326,
     COMMA = 327
   };
#endif
/* Tokens.  */
#define INTERFACE 258
#define IFNAME 259
#define PREFIX_INTERFACE 260
#define SLA_ID 261
#define SLA_LEN 262
#define DUID_ID 263
#define ID_ASSOC 264
#define IA_PD 265
#define IAID 266
#define IA_NA 267
#define ADDRESS 268
#define REQUEST 269
#define SEND 270
#define ALLOW 271
#define PREFERENCE 272
#define HOST 273
#define HOSTNAME 274
#define DUID 275
#define OPTION 276
#define RAPID_COMMIT 277
#define DNS_SERVERS 278
#define DNS_NAME 279
#define VENDOR_OPTS 280
#define DSLITE_NAME 281
#define DSLITE_ADDRESS 282
#define NTP_SERVERS 283
#define REFRESHTIME 284
#define SIP_SERVERS 285
#define SIP_NAME 286
#define NIS_SERVERS 287
#define NIS_NAME 288
#define NISP_SERVERS 289
#define NISP_NAME 290
#define BCMCS_SERVERS 291
#define BCMCS_NAME 292
#define INFO_ONLY 293
#define SCRIPT 294
#define DELAYEDKEY 295
#define AUTHENTICATION 296
#define PROTOCOL 297
#define ALGORITHM 298
#define DELAYED 299
#define RECONFIG 300
#define HMACMD5 301
#define MONOCOUNTER 302
#define AUTHNAME 303
#define RDM 304
#define KEY 305
#define KEYINFO 306
#define REALM 307
#define KEYID 308
#define SECRET 309
#define KEYNAME 310
#define EXPIRE 311
#define ADDRPOOL 312
#define POOLNAME 313
#define RANGE 314
#define TO 315
#define ADDRESS_POOL 316
#define INCLUDE 317
#define NUMBER 318
#define SLASH 319
#define EOS 320
#define BCL 321
#define ECL 322
#define STRING 323
#define QSTRING 324
#define PREFIX 325
#define INFINITY 326
#define COMMA 327




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 126 "cfparse.y"

	long long num;
	char* str;
	struct cf_list *list;
	struct dhcp6_prefix *prefix;
	struct dhcp6_range *range;
	struct dhcp6_poolspec *pool;



/* Line 293 of yacc.c  */
#line 336 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 348 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   234

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  108
/* YYNRULES -- Number of states.  */
#define YYNSTATES  234

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   327

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    30,    37,    42,    47,    52,    57,    62,
      67,    72,    77,    82,    87,    92,    97,   105,   112,   120,
     127,   134,   141,   145,   152,   153,   156,   158,   159,   162,
     166,   170,   173,   177,   181,   185,   189,   193,   197,   201,
     205,   209,   211,   215,   217,   220,   223,   226,   228,   230,
     232,   234,   236,   238,   240,   242,   244,   246,   248,   250,
     252,   254,   256,   260,   263,   267,   272,   278,   281,   285,
     287,   289,   290,   293,   295,   299,   306,   307,   310,   314,
     318,   319,   322,   326,   327,   330,   334,   338,   342,   346,
     348,   350,   352,   354,   355,   358,   362,   366,   370
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      74,     0,    -1,    -1,    74,    75,    -1,    76,    -1,    77,
      -1,    78,    -1,    79,    -1,    80,    -1,    81,    -1,    83,
      -1,    82,    -1,     3,     4,    66,    86,    67,    65,    -1,
      18,    19,    66,    86,    67,    65,    -1,    21,    23,    84,
      65,    -1,    21,    24,    69,    65,    -1,    21,    28,    84,
      65,    -1,    21,    30,    84,    65,    -1,    21,    31,    69,
      65,    -1,    21,    32,    84,    65,    -1,    21,    33,    69,
      65,    -1,    21,    34,    84,    65,    -1,    21,    35,    69,
      65,    -1,    21,    36,    84,    65,    -1,    21,    37,    69,
      65,    -1,    21,    29,    63,    65,    -1,     9,    10,    11,
      66,    95,    67,    65,    -1,     9,    10,    66,    95,    67,
      65,    -1,     9,    12,    11,    66,   100,    67,    65,    -1,
       9,    12,    66,   100,    67,    65,    -1,    41,    48,    66,
     102,    67,    65,    -1,    51,    55,    66,   107,    67,    65,
      -1,    62,    69,    65,    -1,    57,    58,    66,    86,    67,
      65,    -1,    -1,    84,    85,    -1,    68,    -1,    -1,    86,
      87,    -1,    15,    88,    65,    -1,    14,    88,    65,    -1,
      38,    65,    -1,    16,    89,    65,    -1,    20,     8,    65,
      -1,    13,    91,    65,    -1,    70,    92,    65,    -1,    17,
      63,    65,    -1,    39,    69,    65,    -1,    40,    68,    65,
      -1,    59,    90,    65,    -1,    61,    93,    65,    -1,    89,
      -1,    89,    72,    88,    -1,    22,    -1,    41,    48,    -1,
      10,    63,    -1,    12,    63,    -1,    30,    -1,    31,    -1,
      23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,    -1,
      28,    -1,    29,    -1,    32,    -1,    33,    -1,    34,    -1,
      35,    -1,    36,    -1,    37,    -1,    68,    60,    68,    -1,
      68,    94,    -1,    68,    94,    94,    -1,    68,    64,    63,
      94,    -1,    68,    64,    63,    94,    94,    -1,    68,    94,
      -1,    68,    94,    94,    -1,    71,    -1,    63,    -1,    -1,
      95,    96,    -1,    97,    -1,    70,    92,    65,    -1,     5,
       4,    66,    98,    67,    65,    -1,    -1,    98,    99,    -1,
       6,    63,    65,    -1,     7,    63,    65,    -1,    -1,   100,
     101,    -1,    13,    91,    65,    -1,    -1,   102,   103,    -1,
      42,   104,    65,    -1,    43,   105,    65,    -1,    49,   106,
      65,    -1,    50,    68,    65,    -1,    44,    -1,    45,    -1,
      46,    -1,    47,    -1,    -1,   107,   108,    -1,    52,    69,
      65,    -1,    53,    63,    65,    -1,    54,    69,    65,    -1,
      56,    69,    65,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   149,   149,   151,   155,   156,   157,   158,   159,   160,
     161,   162,   166,   178,   190,   199,   214,   223,   232,   247,
     256,   271,   280,   295,   304,   319,   348,   357,   371,   380,
     397,   409,   421,   432,   444,   445,   463,   486,   487,   505,
     513,   521,   529,   537,   545,   553,   561,   570,   578,   586,
     594,   605,   609,   619,   627,   635,   643,   651,   659,   667,
     675,   683,   691,   699,   708,   716,   724,   732,   740,   748,
     756,   764,   775,   806,   833,   866,   893,   926,   951,   982,
     986,   993,   994,  1012,  1013,  1024,  1034,  1035,  1053,  1061,
    1072,  1073,  1091,  1102,  1103,  1121,  1129,  1137,  1145,  1156,
    1157,  1161,  1165,  1169,  1170,  1188,  1196,  1204,  1212
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTERFACE", "IFNAME",
  "PREFIX_INTERFACE", "SLA_ID", "SLA_LEN", "DUID_ID", "ID_ASSOC", "IA_PD",
  "IAID", "IA_NA", "ADDRESS", "REQUEST", "SEND", "ALLOW", "PREFERENCE",
  "HOST", "HOSTNAME", "DUID", "OPTION", "RAPID_COMMIT", "DNS_SERVERS",
  "DNS_NAME", "VENDOR_OPTS", "DSLITE_NAME", "DSLITE_ADDRESS",
  "NTP_SERVERS", "REFRESHTIME", "SIP_SERVERS", "SIP_NAME", "NIS_SERVERS",
  "NIS_NAME", "NISP_SERVERS", "NISP_NAME", "BCMCS_SERVERS", "BCMCS_NAME",
  "INFO_ONLY", "SCRIPT", "DELAYEDKEY", "AUTHENTICATION", "PROTOCOL",
  "ALGORITHM", "DELAYED", "RECONFIG", "HMACMD5", "MONOCOUNTER", "AUTHNAME",
  "RDM", "KEY", "KEYINFO", "REALM", "KEYID", "SECRET", "KEYNAME", "EXPIRE",
  "ADDRPOOL", "POOLNAME", "RANGE", "TO", "ADDRESS_POOL", "INCLUDE",
  "NUMBER", "SLASH", "EOS", "BCL", "ECL", "STRING", "QSTRING", "PREFIX",
  "INFINITY", "COMMA", "$accept", "statements", "statement",
  "interface_statement", "host_statement", "option_statement",
  "ia_statement", "authentication_statement", "key_statement",
  "include_statement", "addrpool_statement", "address_list",
  "address_list_ent", "declarations", "declaration", "dhcpoption_list",
  "dhcpoption", "rangeparam", "addressparam", "prefixparam", "poolparam",
  "duration", "iapdconf_list", "iapdconf", "prefix_interface", "ifparams",
  "ifparam", "ianaconf_list", "ianaconf", "authparam_list", "authparam",
  "authproto", "authalg", "authrdm", "keyparam_list", "keyparam", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    73,    74,    74,    75,    75,    75,    75,    75,    75,
      75,    75,    76,    77,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    79,    79,    79,    79,
      80,    81,    82,    83,    84,    84,    85,    86,    86,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    88,    88,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    90,    91,    91,    92,    92,    93,    93,    94,
      94,    95,    95,    96,    96,    97,    98,    98,    99,    99,
     100,   100,   101,   102,   102,   103,   103,   103,   103,   104,
     104,   105,   106,   107,   107,   108,   108,   108,   108
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     7,     6,     7,     6,
       6,     6,     3,     6,     0,     2,     1,     0,     2,     3,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     3,     4,     5,     2,     3,     1,
       1,     0,     2,     1,     3,     6,     0,     2,     3,     3,
       0,     2,     3,     0,     2,     3,     3,     3,     3,     1,
       1,     1,     1,     0,     2,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     3,     4,     5,     6,     7,     8,     9,    11,    10,
       0,     0,     0,     0,    34,     0,    34,     0,    34,     0,
      34,     0,    34,     0,    34,     0,     0,     0,     0,     0,
      37,     0,    81,     0,    90,    37,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,   103,
      37,    32,     0,    81,     0,    90,     0,     0,    14,    36,
      35,    15,    16,    25,    17,    18,    19,    20,    21,    22,
      23,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,    82,    83,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,   104,     0,     0,     0,     0,     0,    53,    59,    60,
      61,    62,    63,    64,    65,    57,    58,    66,    67,    68,
      69,    70,    71,     0,     0,    51,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,    12,     0,     0,
       0,     0,    27,     0,     0,     0,    29,    13,    99,   100,
       0,   101,     0,   102,     0,     0,    30,     0,     0,     0,
       0,    31,    33,    80,    79,    73,    44,    55,    56,    54,
      40,     0,    39,    42,    46,    43,    47,    48,     0,    49,
      77,    50,     0,    45,    26,    86,    84,    28,    92,    95,
      96,    97,    98,   105,   106,   107,   108,    74,    52,    72,
      78,     0,     0,    75,     0,     0,     0,    87,    76,     0,
       0,    85,    88,    89
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    46,    70,    62,    98,   144,   145,   154,   124,   159,
     156,   185,    64,   103,   104,   222,   227,    66,   108,    82,
     115,   170,   172,   174,    83,   121
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -155
static const yytype_int16 yypact[] =
{
    -155,     2,  -155,    14,     0,     9,   140,    -8,   -21,   -11,
     -13,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
      21,     5,     6,    24,  -155,   -12,  -155,    17,  -155,    -3,
    -155,    25,  -155,    26,  -155,    27,    31,    37,    39,   -10,
    -155,    40,  -155,    41,  -155,  -155,   -46,    28,   -41,    33,
     -24,    48,   -20,    52,   -16,    54,   -14,    60,  -155,  -155,
    -155,  -155,    22,  -155,    -2,  -155,    -9,    62,  -155,  -155,
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,   -17,    32,    95,    23,   114,   114,   114,    64,
     120,    65,    83,    63,    85,    89,    93,    91,  -155,     3,
     156,    96,    91,  -155,  -155,    -4,    23,   101,  -155,   102,
     -15,   132,   133,   111,   116,  -155,   113,   121,   117,   118,
     123,  -155,   124,   -56,   125,   122,   128,  -155,  -155,  -155,
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,  -155,   135,   127,   126,   129,   130,   131,   134,
    -155,   136,   137,   143,   139,   -56,   141,  -155,   144,   142,
     145,   146,  -155,   148,   149,   150,  -155,  -155,  -155,  -155,
     151,  -155,   152,  -155,   153,   154,  -155,   155,   157,   158,
     159,  -155,  -155,  -155,  -155,   -56,  -155,  -155,  -155,  -155,
    -155,   114,  -155,  -155,  -155,  -155,  -155,  -155,   160,  -155,
     -56,  -155,   162,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,   -56,     7,   -56,   163,   164,   165,  -155,  -155,   166,
     167,  -155,  -155,  -155
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,    88,  -155,   -39,  -155,   -87,   105,  -155,    94,   103,
    -155,  -154,   170,  -155,  -155,  -155,  -155,   169,  -155,  -155,
    -155,  -155,  -155,  -155,  -155,  -155
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
     146,   200,     2,   100,   106,     3,    67,   183,   100,   106,
      21,     4,    22,   224,   225,   184,    41,    43,    20,    68,
       5,    84,    69,     6,    72,   110,   111,    69,    23,   168,
     169,   217,   112,   113,    37,    85,    86,    87,    88,    89,
      36,    74,    90,     7,    69,    76,   220,    38,    69,    78,
     114,    80,    69,     8,    69,    61,    39,    47,   107,     9,
      91,    92,    93,   164,    10,   101,    51,   223,   102,   228,
     160,    42,    44,   102,   226,    85,    86,    87,    88,    89,
      49,    94,    90,    95,   116,   117,   118,    40,   119,    96,
      45,   123,    97,    71,    53,    55,    57,    58,    73,   120,
      91,    92,    93,    59,   218,    60,    63,    65,    85,    86,
      87,    88,    89,    75,    48,    90,    50,    77,    52,    79,
      54,    94,    56,    95,   125,    81,   126,   148,   149,   109,
     150,   152,    97,    91,    92,    93,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   151,   153,    94,   143,    95,   155,   157,   158,
     161,   162,   122,    24,    25,    97,   166,   167,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,   171,   175,
     173,   176,   177,   189,   178,   187,   179,   180,   181,   182,
     186,   188,   190,   147,   192,   193,   194,     0,   191,   195,
     165,   196,   197,   198,   199,   163,   201,   203,   202,     0,
     204,     0,   205,   206,   207,   208,   209,   210,   211,   212,
     213,     0,   214,   215,   216,   221,   229,   230,   219,     0,
     231,   232,   233,    99,   105
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-155))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      87,   155,     0,     5,    13,     3,    45,    63,     5,    13,
      10,     9,    12,     6,     7,    71,    11,    11,     4,    65,
      18,    60,    68,    21,    65,    42,    43,    68,    19,    44,
      45,   185,    49,    50,    55,    13,    14,    15,    16,    17,
      48,    65,    20,    41,    68,    65,   200,    58,    68,    65,
      67,    65,    68,    51,    68,    65,    69,    69,    67,    57,
      38,    39,    40,    67,    62,    67,    69,   221,    70,   223,
      67,    66,    66,    70,    67,    13,    14,    15,    16,    17,
      63,    59,    20,    61,    52,    53,    54,    66,    56,    67,
      66,    68,    70,    65,    69,    69,    69,    66,    65,    67,
      38,    39,    40,    66,   191,    66,    66,    66,    13,    14,
      15,    16,    17,    65,    26,    20,    28,    65,    30,    65,
      32,    59,    34,    61,    10,    65,    12,    63,     8,    67,
      65,    68,    70,    38,    39,    40,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    69,    68,    59,    41,    61,    68,    65,    68,
       4,    65,    67,    23,    24,    70,    65,    65,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    46,    68,
      47,    65,    69,    48,    63,    63,    69,    69,    65,    65,
      65,    63,    65,    88,    65,    65,    65,    -1,    72,    65,
     106,    65,    65,    60,    65,   102,    65,    65,    64,    -1,
      65,    -1,    66,    65,    65,    65,    65,    65,    65,    65,
      65,    -1,    65,    65,    65,    63,    63,    63,    68,    -1,
      65,    65,    65,    63,    65
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    74,     0,     3,     9,    18,    21,    41,    51,    57,
      62,    75,    76,    77,    78,    79,    80,    81,    82,    83,
       4,    10,    12,    19,    23,    24,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    48,    55,    58,    69,
      66,    11,    66,    11,    66,    66,    84,    69,    84,    63,
      84,    69,    84,    69,    84,    69,    84,    69,    66,    66,
      66,    65,    86,    66,    95,    66,   100,    86,    65,    68,
      85,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,   102,   107,    86,    13,    14,    15,    16,    17,
      20,    38,    39,    40,    59,    61,    67,    70,    87,    95,
       5,    67,    70,    96,    97,   100,    13,    67,   101,    67,
      42,    43,    49,    50,    67,   103,    52,    53,    54,    56,
      67,   108,    67,    68,    91,    10,    12,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    41,    88,    89,    88,    89,    63,     8,
      65,    69,    68,    68,    90,    68,    93,    65,    68,    92,
      67,     4,    65,    92,    67,    91,    65,    65,    44,    45,
     104,    46,   105,    47,   106,    68,    65,    69,    63,    69,
      69,    65,    65,    63,    71,    94,    65,    63,    63,    48,
      65,    72,    65,    65,    65,    65,    65,    65,    60,    65,
      94,    65,    64,    65,    65,    66,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    94,    88,    68,
      94,    63,    98,    94,     6,     7,    67,    99,    94,    63,
      63,    65,    65,    65
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 12:

/* Line 1806 of yacc.c  */
#line 167 "cfparse.y"
    {
		struct cf_namelist *ifl;

		MAKE_NAMELIST(ifl, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(ifl, &iflist_head))
			return (-1);
	}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 179 "cfparse.y"
    {
		struct cf_namelist *host;

		MAKE_NAMELIST(host, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(host, &hostlist_head))
			return (-1);
	}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 191 "cfparse.y"
    {
			if (cf_dns_list == NULL)
				cf_dns_list = (yyvsp[(3) - (4)].list);
			else {
				cf_dns_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_dns_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 200 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_dns_name_list == NULL) {
				cf_dns_name_list = l;
				cf_dns_name_list->tail = l;
				cf_dns_name_list->next = NULL;
			} else {
				cf_dns_name_list->tail->next = l;
				cf_dns_name_list->tail = l->tail;
			}
		}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 215 "cfparse.y"
    {
			if (cf_ntp_list == NULL)
				cf_ntp_list = (yyvsp[(3) - (4)].list);
			else {
				cf_ntp_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_ntp_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 224 "cfparse.y"
    {
			if (cf_sip_list == NULL)
				cf_sip_list = (yyvsp[(3) - (4)].list);
			else {
				cf_sip_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_sip_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 233 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_sip_name_list == NULL) {
				cf_sip_name_list = l;
				cf_sip_name_list->tail = l;
				cf_sip_name_list->next = NULL;
			} else {
				cf_sip_name_list->tail->next = l;
				cf_sip_name_list->tail = l->tail;
			}
		}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 248 "cfparse.y"
    {
			if (cf_nis_list == NULL)
				cf_nis_list = (yyvsp[(3) - (4)].list);
			else {
				cf_nis_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_nis_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 257 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_nis_name_list == NULL) {
				cf_nis_name_list = l;
				cf_nis_name_list->tail = l;
				cf_nis_name_list->next = NULL;
			} else {
				cf_nis_name_list->tail->next = l;
				cf_nis_name_list->tail = l->tail;
			}
		}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 272 "cfparse.y"
    {
			if (cf_nisp_list == NULL)
				cf_nisp_list = (yyvsp[(3) - (4)].list);
			else {
				cf_nisp_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_nisp_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 281 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_nisp_name_list == NULL) {
				cf_nisp_name_list = l;
				cf_nisp_name_list->tail = l;
				cf_nisp_name_list->next = NULL;
			} else {
				cf_nisp_name_list->tail->next = l;
				cf_nisp_name_list->tail = l->tail;
			}
		}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 296 "cfparse.y"
    {
			if (cf_bcmcs_list == NULL)
				cf_bcmcs_list = (yyvsp[(3) - (4)].list);
			else {
				cf_bcmcs_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_bcmcs_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 305 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_bcmcs_name_list == NULL) {
				cf_bcmcs_name_list = l;
				cf_bcmcs_name_list->tail = l;
				cf_bcmcs_name_list->next = NULL;
			} else {
				cf_bcmcs_name_list->tail->next = l;
				cf_bcmcs_name_list->tail = l->tail;
			}
		}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 320 "cfparse.y"
    {
			if (cf_refreshtime == -1) {
				cf_refreshtime = (yyvsp[(3) - (4)].num);
				if (cf_refreshtime < -1 ||
				    cf_refreshtime > 0xffffffff) {
					/*
					 * refresh time should not be negative
					 * according to the lex definition,
					 * but check it for safety.
					 */
					yyerror("refresh time is out of range");
				}
				if (cf_refreshtime < DHCP6_IRT_MINIMUM) {
					/*
					 * the value MUST NOT be smaller than
					 * IRT_MINIMUM.
					 */
					yyerror("refresh time is too small "
					    "(must not be smaller than %d)",
					    DHCP6_IRT_MINIMUM);
				}
			} else {
				yywarn("multiple refresh times (ignored)");
			}
		}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 349 "cfparse.y"
    {
			struct cf_namelist *iapd;

			MAKE_NAMELIST(iapd, (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 358 "cfparse.y"
    {
			struct cf_namelist *iapd;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iapd, zero, (yyvsp[(4) - (6)].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 372 "cfparse.y"
    {
			struct cf_namelist *iana;

			MAKE_NAMELIST(iana, (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 381 "cfparse.y"
    {
			struct cf_namelist *iana;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iana, zero, (yyvsp[(4) - (6)].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 398 "cfparse.y"
    {
		struct cf_namelist *authinfo;

		MAKE_NAMELIST(authinfo, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(authinfo, &authinfolist_head))
			return (-1);
	}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 410 "cfparse.y"
    {
		struct cf_namelist *key;

		MAKE_NAMELIST(key, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(key, &keylist_head))
			return (-1);
	}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 422 "cfparse.y"
    {
		if (cfswitch_buffer((yyvsp[(2) - (3)].str))) {
			free((yyvsp[(2) - (3)].str));
			return (-1);
		}
		free((yyvsp[(2) - (3)].str));
	}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 433 "cfparse.y"
    {
		struct cf_namelist *pool;

		MAKE_NAMELIST(pool, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(pool, &addrpoollist_head))
			return (-1);
	}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 444 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 446 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 464 "cfparse.y"
    {
		struct cf_list *l;
		struct in6_addr a0, *a;

		if (inet_pton(AF_INET6, (yyvsp[(1) - (1)].str), &a0) != 1) {
			yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (1)].str));
			free((yyvsp[(1) - (1)].str));
			return (-1);
		}
		if ((a = malloc(sizeof(*a))) == NULL) {
			yywarn("can't allocate memory");
			return (-1);
		}
		*a = a0;

		MAKE_CFLIST(l, CFLISTENT_GENERIC, a, NULL);

		(yyval.list) = l;
	}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 486 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 488 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 506 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SEND, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 514 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_REQUEST, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 522 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_INFO_ONLY, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 530 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ALLOW, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 538 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DUID, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 546 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESS, (yyvsp[(2) - (3)].prefix),NULL);

			(yyval.list) = l;
		}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 554 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFIX, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 562 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFERENCE, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);

			(yyval.list) = l;
		}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 571 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SCRIPT, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 579 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DELAYEDKEY, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 587 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_RANGE, (yyvsp[(2) - (3)].range), NULL);

			(yyval.list) = l;
		}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 595 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESSPOOL, (yyvsp[(2) - (3)].pool), NULL);

			(yyval.list) = l;
		}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 606 "cfparse.y"
    {
			(yyval.list) = (yyvsp[(1) - (1)].list);
		}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 610 "cfparse.y"
    {
			(yyvsp[(1) - (3)].list)->next = (yyvsp[(3) - (3)].list);
			(yyvsp[(1) - (3)].list)->tail = (yyvsp[(3) - (3)].list)->tail;

			(yyval.list) = (yyvsp[(1) - (3)].list);
		}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 620 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_RAPID_COMMIT, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 628 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_AUTHINFO, NULL, NULL);
			l->ptr = (yyvsp[(2) - (2)].str);
			(yyval.list) = l;
		}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 636 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_PD, NULL, NULL);
			l->num = (yyvsp[(2) - (2)].num);
			(yyval.list) = l;
		}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 644 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_NA, NULL, NULL);
			l->num = (yyvsp[(2) - (2)].num);
			(yyval.list) = l;
		}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 652 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 660 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 668 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 676 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 684 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_VENDOR_OPTS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 692 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DSLITENAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 700 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DSLITEADDRESS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 709 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NTP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 717 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_REFRESHTIME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 725 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NIS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 733 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 741 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 749 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 757 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 765 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 776 "cfparse.y"
    {
			struct dhcp6_range range0, *range;		

			memset(&range0, 0, sizeof(range0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (3)].str), &range0.min) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				free((yyvsp[(3) - (3)].str));
				return (-1);
			}
			if (inet_pton(AF_INET6, (yyvsp[(3) - (3)].str), &range0.max) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(3) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				free((yyvsp[(3) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));
			free((yyvsp[(3) - (3)].str));

			if ((range = malloc(sizeof(*range))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*range = range0;

			(yyval.range) = range;
		}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 807 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (2)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (2)].str));
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			free((yyvsp[(1) - (2)].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX this field is ignored */
			if ((yyvsp[(2) - (2)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(2) - (2)].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 834 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (3)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX */
			if ((yyvsp[(2) - (3)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(2) - (3)].num);
			if ((yyvsp[(3) - (3)].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[(3) - (3)].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 867 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (4)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (4)].str));
				free((yyvsp[(1) - (4)].str));
				return (-1);
			}
			free((yyvsp[(1) - (4)].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[(3) - (4)].num);
			if ((yyvsp[(4) - (4)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(4) - (4)].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 894 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (5)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (5)].str));
				free((yyvsp[(1) - (5)].str));
				return (-1);
			}
			free((yyvsp[(1) - (5)].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[(3) - (5)].num);
			if ((yyvsp[(4) - (5)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(4) - (5)].num);
			if ((yyvsp[(5) - (5)].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[(5) - (5)].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 927 "cfparse.y"
    {
			struct dhcp6_poolspec* pool;		

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[(1) - (2)].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			free((yyvsp[(1) - (2)].str));

			/* validate other parameters later */
			if ((yyvsp[(2) - (2)].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[(2) - (2)].num);
			pool->vltime = pool->pltime;

			(yyval.pool) = pool;
		}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 952 "cfparse.y"
    {
			struct dhcp6_poolspec* pool;		

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[(1) - (3)].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));

			/* validate other parameters later */
			if ((yyvsp[(2) - (3)].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[(2) - (3)].num);
			if ((yyvsp[(3) - (3)].num) < 0)
				pool->vltime = DHCP6_DURATION_INFINITE;
			else
				pool->vltime = (u_int32_t)(yyvsp[(3) - (3)].num);

			(yyval.pool) = pool;
		}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 983 "cfparse.y"
    {
			(yyval.num) = -1;
		}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 987 "cfparse.y"
    {
			(yyval.num) = (yyvsp[(1) - (1)].num);
		}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 993 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 995 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 1012 "cfparse.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 1014 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_PREFIX, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 1025 "cfparse.y"
    {
		struct cf_list *ifl;

		MAKE_CFLIST(ifl, IACONF_PIF, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));
		(yyval.list) = ifl;
	}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 1034 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 1036 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 1054 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_ID, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 1062 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_LEN, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 1072 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 1074 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1092 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_ADDR, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1102 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 1104 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 1122 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_PROTO, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 1130 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_ALG, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 1138 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_RDM, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1146 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_KEY, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1156 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHPROTO_DELAYED; }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1157 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHPROTO_RECONFIG; }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1161 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHALG_HMACMD5; }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1165 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHRDM_MONOCOUNTER; }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1169 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1171 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 1189 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_REALM, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 1197 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_KEYID, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 1205 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_SECRET, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 1213 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_EXPIRE, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;



/* Line 1806 of yacc.c  */
#line 3242 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1222 "cfparse.y"

/* supplement routines for configuration */
static int
add_namelist(new, headp)
	struct cf_namelist *new, **headp;
{
	struct cf_namelist *n;
	
	/* check for duplicated configuration */
	for (n = *headp; n; n = n->next) {
		if (strcmp(n->name, new->name) == 0) {
			yywarn("duplicated name: %s (ignored)",
			       new->name);
			cleanup_namelist(new);
			return (0);
		}
	}

	new->next = *headp;
	*headp = new;

	return (0);
}

/* free temporary resources */
static void
cleanup()
{
	cleanup_namelist(iflist_head);
	iflist_head = NULL;
	cleanup_namelist(hostlist_head);
	hostlist_head = NULL;
	cleanup_namelist(iapdlist_head);
	iapdlist_head = NULL;
	cleanup_namelist(ianalist_head);
	ianalist_head = NULL;
	cleanup_namelist(authinfolist_head);
	authinfolist_head = NULL;
	cleanup_namelist(keylist_head);
	keylist_head = NULL;
	cleanup_namelist(addrpoollist_head);
	addrpoollist_head = NULL;

	cleanup_cflist(cf_sip_list);
	cf_sip_list = NULL;
	cleanup_cflist(cf_sip_name_list);
	cf_sip_name_list = NULL;
	cleanup_cflist(cf_dns_list);
	cf_dns_list = NULL;
	cleanup_cflist(cf_dns_name_list);
	cf_dns_name_list = NULL;
	cleanup_cflist(cf_ntp_list);
	cf_ntp_list = NULL;
	cleanup_cflist(cf_nis_list);
	cf_nis_list = NULL;
	cleanup_cflist(cf_nis_name_list);
	cf_nis_name_list = NULL;
	cleanup_cflist(cf_nisp_list);
	cf_nisp_list = NULL;
	cleanup_cflist(cf_nisp_name_list);
	cf_nisp_name_list = NULL;
	cleanup_cflist(cf_bcmcs_list);
	cf_bcmcs_list = NULL;
	cleanup_cflist(cf_bcmcs_name_list);
	cf_bcmcs_name_list = NULL;
}

static void
cleanup_namelist(head)
	struct cf_namelist *head;
{
	struct cf_namelist *ifp, *ifp_next;

	for (ifp = head; ifp; ifp = ifp_next) {
		ifp_next = ifp->next;
		cleanup_cflist(ifp->params);
		free(ifp->name);
		free(ifp);
	}
}

static void
cleanup_cflist(p)
	struct cf_list *p;
{
	struct cf_list *n;

	if (p == NULL)
		return;

	n = p->next;
	if (p->type == DECL_ADDRESSPOOL) {
		free(((struct dhcp6_poolspec *)p->ptr)->name);
	}
	if (p->ptr)
		free(p->ptr);
	if (p->list)
		cleanup_cflist(p->list);
	free(p);

	cleanup_cflist(n);
}

#define config_fail() \
	do { cleanup(); configure_cleanup(); return (-1); } while(0)

int
cf_post_config()
{
	if (configure_keys(keylist_head))
		config_fail();

	if (configure_authinfo(authinfolist_head))
		config_fail();

	if (configure_ia(iapdlist_head, IATYPE_PD))
		config_fail();

	if (configure_ia(ianalist_head, IATYPE_NA))
		config_fail();

	if (configure_pool(addrpoollist_head))
		config_fail();

	if (configure_interface(iflist_head))
		config_fail();

	if (configure_host(hostlist_head))
		config_fail();

	if (configure_global_option())
		config_fail();

	configure_commit();
	cleanup();
	return (0);
}
#undef config_fail

void
cf_init()
{
	iflist_head = NULL;
}


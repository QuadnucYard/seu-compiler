/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     CONSTANT = 259,
     STRING_LITERAL = 260,
     SIZEOF = 261,
     PTR_OP = 262,
     INC_OP = 263,
     DEC_OP = 264,
     LEFT_OP = 265,
     RIGHT_OP = 266,
     LE_OP = 267,
     GE_OP = 268,
     EQ_OP = 269,
     NE_OP = 270,
     AND_OP = 271,
     OR_OP = 272,
     MUL_ASSIGN = 273,
     DIV_ASSIGN = 274,
     MOD_ASSIGN = 275,
     ADD_ASSIGN = 276,
     SUB_ASSIGN = 277,
     LEFT_ASSIGN = 278,
     RIGHT_ASSIGN = 279,
     AND_ASSIGN = 280,
     XOR_ASSIGN = 281,
     OR_ASSIGN = 282,
     TYPE_NAME = 283,
     HINCLUDE = 284,
     TYPEDEF = 285,
     EXTERN = 286,
     STATIC = 287,
     AUTO = 288,
     REGISTER = 289,
     INLINE = 290,
     RESTRICT = 291,
     CHAR = 292,
     SHORT = 293,
     INT = 294,
     LONG = 295,
     SIGNED = 296,
     UNSIGNED = 297,
     FLOAT = 298,
     DOUBLE = 299,
     CONST = 300,
     VOLATILE = 301,
     VOID = 302,
     BOOL = 303,
     COMPLEX = 304,
     IMAGINARY = 305,
     STRUCT = 306,
     UNION = 307,
     ENUM = 308,
     ELLIPSIS = 309,
     CASE = 310,
     DEFAULT = 311,
     IF = 312,
     ELSE = 313,
     SWITCH = 314,
     WHILE = 315,
     DO = 316,
     FOR = 317,
     GOTO = 318,
     CONTINUE = 319,
     BREAK = 320,
     RETURN = 321
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define CONSTANT 259
#define STRING_LITERAL 260
#define SIZEOF 261
#define PTR_OP 262
#define INC_OP 263
#define DEC_OP 264
#define LEFT_OP 265
#define RIGHT_OP 266
#define LE_OP 267
#define GE_OP 268
#define EQ_OP 269
#define NE_OP 270
#define AND_OP 271
#define OR_OP 272
#define MUL_ASSIGN 273
#define DIV_ASSIGN 274
#define MOD_ASSIGN 275
#define ADD_ASSIGN 276
#define SUB_ASSIGN 277
#define LEFT_ASSIGN 278
#define RIGHT_ASSIGN 279
#define AND_ASSIGN 280
#define XOR_ASSIGN 281
#define OR_ASSIGN 282
#define TYPE_NAME 283
#define HINCLUDE 284
#define TYPEDEF 285
#define EXTERN 286
#define STATIC 287
#define AUTO 288
#define REGISTER 289
#define INLINE 290
#define RESTRICT 291
#define CHAR 292
#define SHORT 293
#define INT 294
#define LONG 295
#define SIGNED 296
#define UNSIGNED 297
#define FLOAT 298
#define DOUBLE 299
#define CONST 300
#define VOLATILE 301
#define VOID 302
#define BOOL 303
#define COMPLEX 304
#define IMAGINARY 305
#define STRUCT 306
#define UNION 307
#define ENUM 308
#define ELLIPSIS 309
#define CASE 310
#define DEFAULT 311
#define IF 312
#define ELSE 313
#define SWITCH 314
#define WHILE 315
#define DO 316
#define FOR 317
#define GOTO 318
#define CONTINUE 319
#define BREAK 320
#define RETURN 321




/* Copy the first part of user declarations.  */
#line 1 "c.y"

	#include <stdio.h>
	#include <stdlib.h>
	extern char yytext[];
	extern int column,line,prevCol;
	int printed = 0;
	void yyerror(char const *s);
	int yylex();


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 248 "y.tab.c"

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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  60
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1532

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  71
/* YYNRULES -- Number of rules.  */
#define YYNRULES  245
/* YYNRULES -- Number of states.  */
#define YYNSTATES  415

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   321

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,     2,     2,     2,    82,    75,     2,
      67,    68,    76,    77,    74,    78,    71,    81,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    88,    90,
      83,    89,    84,    87,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    69,     2,    70,    85,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    72,    86,    73,    79,     2,     2,     2,
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
      65,    66
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    13,    15,    20,    24,
      29,    33,    37,    40,    43,    50,    58,    60,    64,    66,
      69,    72,    75,    78,    83,    85,    87,    89,    91,    93,
      95,    97,   102,   104,   108,   112,   116,   118,   122,   126,
     128,   132,   136,   138,   142,   146,   150,   154,   156,   160,
     164,   166,   170,   172,   176,   178,   182,   184,   188,   190,
     194,   196,   202,   204,   208,   210,   212,   214,   216,   218,
     220,   222,   224,   226,   228,   230,   232,   234,   238,   240,
     243,   247,   249,   252,   254,   257,   259,   262,   264,   267,
     269,   273,   275,   279,   281,   283,   285,   287,   289,   291,
     293,   295,   297,   299,   301,   303,   305,   307,   309,   311,
     313,   315,   317,   319,   325,   330,   333,   335,   337,   339,
     342,   346,   349,   351,   354,   356,   358,   362,   364,   367,
     371,   376,   382,   388,   395,   398,   400,   404,   406,   410,
     412,   414,   416,   418,   421,   423,   425,   429,   435,   440,
     445,   452,   459,   465,   470,   474,   479,   484,   488,   490,
     493,   496,   500,   502,   505,   507,   511,   513,   517,   520,
     523,   525,   527,   531,   533,   536,   538,   540,   543,   547,
     550,   554,   558,   563,   567,   572,   575,   579,   583,   588,
     590,   594,   599,   601,   604,   608,   613,   616,   618,   621,
     625,   628,   630,   632,   634,   636,   638,   640,   644,   649,
     653,   656,   660,   662,   665,   667,   669,   671,   674,   680,
     688,   694,   700,   708,   715,   723,   730,   738,   742,   745,
     748,   751,   755,   757,   760,   762,   770,   776,   780,   781,
     784,   787,   788,   793,   797,   799
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     157,     0,    -1,     3,    -1,     4,    -1,     5,    -1,    67,
     112,    68,    -1,    92,    -1,    93,    69,   112,    70,    -1,
      93,    67,    68,    -1,    93,    67,    94,    68,    -1,    93,
      71,     3,    -1,    93,     7,     3,    -1,    93,     8,    -1,
      93,     9,    -1,    67,   140,    68,    72,   144,    73,    -1,
      67,   140,    68,    72,   144,    74,    73,    -1,   109,    -1,
      94,    74,   109,    -1,    93,    -1,     8,    95,    -1,     9,
      95,    -1,    96,    97,    -1,     6,    95,    -1,     6,    67,
     140,    68,    -1,    75,    -1,    76,    -1,    77,    -1,    78,
      -1,    79,    -1,    80,    -1,    95,    -1,    67,   140,    68,
      97,    -1,    97,    -1,    98,    76,    97,    -1,    98,    81,
      97,    -1,    98,    82,    97,    -1,    98,    -1,    99,    77,
      98,    -1,    99,    78,    98,    -1,    99,    -1,   100,    10,
      99,    -1,   100,    11,    99,    -1,   100,    -1,   101,    83,
     100,    -1,   101,    84,   100,    -1,   101,    12,   100,    -1,
     101,    13,   100,    -1,   101,    -1,   102,    14,   101,    -1,
     102,    15,   101,    -1,   102,    -1,   103,    75,   102,    -1,
     103,    -1,   104,    85,   103,    -1,   104,    -1,   105,    86,
     104,    -1,   105,    -1,   106,    16,   105,    -1,   106,    -1,
     107,    17,   106,    -1,   107,    -1,   107,    87,   112,    88,
     108,    -1,   108,    -1,    95,   111,   109,    -1,   107,    -1,
      89,    -1,    18,    -1,    19,    -1,    20,    -1,    21,    -1,
      22,    -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,
      27,    -1,   109,    -1,   112,    74,   109,    -1,   108,    -1,
     115,    90,    -1,   115,   116,    90,    -1,   118,    -1,   118,
     115,    -1,   119,    -1,   119,   115,    -1,   130,    -1,   130,
     115,    -1,   131,    -1,   131,   115,    -1,   117,    -1,   116,
      74,   117,    -1,   132,    -1,   132,    89,   143,    -1,    30,
      -1,    31,    -1,    32,    -1,    33,    -1,    34,    -1,    47,
      -1,    37,    -1,    38,    -1,    39,    -1,    40,    -1,    43,
      -1,    44,    -1,    41,    -1,    42,    -1,    48,    -1,    49,
      -1,    50,    -1,   120,    -1,   127,    -1,    28,    -1,   121,
       3,    72,   122,    73,    -1,   121,    72,   122,    73,    -1,
     121,     3,    -1,    51,    -1,    52,    -1,   123,    -1,   122,
     123,    -1,   124,   125,    90,    -1,   119,   124,    -1,   119,
      -1,   130,   124,    -1,   130,    -1,   126,    -1,   125,    74,
     126,    -1,   132,    -1,    88,   113,    -1,   132,    88,   113,
      -1,    53,    72,   128,    73,    -1,    53,     3,    72,   128,
      73,    -1,    53,    72,   128,    74,    73,    -1,    53,     3,
      72,   128,    74,    73,    -1,    53,     3,    -1,   129,    -1,
     128,    74,   129,    -1,     3,    -1,     3,    89,   113,    -1,
      45,    -1,    36,    -1,    46,    -1,    35,    -1,   134,   133,
      -1,   133,    -1,     3,    -1,    67,   132,    68,    -1,   133,
      69,   135,   109,    70,    -1,   133,    69,   135,    70,    -1,
     133,    69,   109,    70,    -1,   133,    69,    32,   135,   109,
      70,    -1,   133,    69,   135,    32,   109,    70,    -1,   133,
      69,   135,    76,    70,    -1,   133,    69,    76,    70,    -1,
     133,    69,    70,    -1,   133,    67,   136,    68,    -1,   133,
      67,   139,    68,    -1,   133,    67,    68,    -1,    76,    -1,
      76,   135,    -1,    76,   134,    -1,    76,   135,   134,    -1,
     130,    -1,   135,   130,    -1,   137,    -1,   137,    74,    54,
      -1,   138,    -1,   137,    74,   138,    -1,   115,   132,    -1,
     115,   141,    -1,   115,    -1,     3,    -1,   139,    74,     3,
      -1,   124,    -1,   124,   141,    -1,   134,    -1,   142,    -1,
     134,   142,    -1,    67,   141,    68,    -1,    69,    70,    -1,
      69,   109,    70,    -1,   142,    69,    70,    -1,   142,    69,
     109,    70,    -1,    69,    76,    70,    -1,   142,    69,    76,
      70,    -1,    67,    68,    -1,    67,   136,    68,    -1,   142,
      67,    68,    -1,   142,    67,   136,    68,    -1,   109,    -1,
      72,   144,    73,    -1,    72,   144,    74,    73,    -1,   143,
      -1,   145,   143,    -1,   144,    74,   143,    -1,   144,    74,
     145,   143,    -1,   146,    89,    -1,   147,    -1,   146,   147,
      -1,    69,   113,    70,    -1,    71,     3,    -1,   149,    -1,
     150,    -1,   153,    -1,   154,    -1,   155,    -1,   156,    -1,
       3,    88,   148,    -1,    55,   113,    88,   148,    -1,    56,
      88,   148,    -1,    72,    73,    -1,    72,   151,    73,    -1,
     152,    -1,   151,   152,    -1,   114,    -1,   148,    -1,    90,
      -1,   112,    90,    -1,    57,    67,   110,    68,   148,    -1,
      57,    67,   110,    68,   148,    58,   148,    -1,    59,    67,
     110,    68,   148,    -1,    60,    67,   112,    68,   148,    -1,
      61,   148,    60,    67,   112,    68,    90,    -1,    62,    67,
     153,   153,    68,   148,    -1,    62,    67,   153,   153,   112,
      68,   148,    -1,    62,    67,   114,   153,    68,   148,    -1,
      62,    67,   114,   153,   112,    68,   148,    -1,    63,     3,
      90,    -1,    64,    90,    -1,    65,    90,    -1,    66,    90,
      -1,    66,   112,    90,    -1,   159,    -1,   157,   159,    -1,
     158,    -1,    29,    83,     3,    71,     3,    84,   158,    -1,
      29,    83,     3,    84,   158,    -1,    29,     5,   158,    -1,
      -1,   160,   159,    -1,   114,   159,    -1,    -1,   115,   132,
     161,   150,    -1,   115,   132,   150,    -1,   114,    -1,   161,
     114,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    33,    33,    34,    35,    36,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    53,    54,    58,    59,
      60,    61,    62,    63,    67,    68,    69,    70,    71,    72,
      76,    77,    81,    82,    83,    84,    88,    89,    90,    94,
      95,    96,   100,   101,   102,   103,   104,   108,   109,   110,
     114,   115,   119,   120,   124,   125,   129,   130,   134,   135,
     139,   140,   144,   145,   148,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   166,   167,   171,   175,
     176,   180,   181,   182,   183,   184,   185,   186,   187,   191,
     192,   196,   197,   201,   202,   203,   204,   205,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   227,   228,   229,   233,   234,   238,   239,
     243,   247,   248,   249,   250,   254,   255,   259,   260,   261,
     265,   266,   267,   268,   269,   273,   274,   278,   279,   283,
     284,   285,   289,   293,   294,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   315,   316,
     317,   318,   322,   323,   328,   329,   333,   334,   338,   339,
     340,   344,   345,   349,   350,   354,   355,   356,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   374,
     375,   376,   380,   381,   382,   383,   387,   391,   392,   396,
     397,   401,   402,   403,   404,   405,   406,   410,   411,   412,
     416,   417,   421,   422,   426,   427,   431,   432,   436,   437,
     438,   442,   443,   444,   445,   446,   447,   451,   452,   453,
     454,   455,   460,   461,   462,   465,   466,   467,   468,   472,
     473,   474,   478,   479,   483,   484
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "CONSTANT",
  "STRING_LITERAL", "SIZEOF", "PTR_OP", "INC_OP", "DEC_OP", "LEFT_OP",
  "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "AND_OP", "OR_OP",
  "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN",
  "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN",
  "TYPE_NAME", "HINCLUDE", "TYPEDEF", "EXTERN", "STATIC", "AUTO",
  "REGISTER", "INLINE", "RESTRICT", "CHAR", "SHORT", "INT", "LONG",
  "SIGNED", "UNSIGNED", "FLOAT", "DOUBLE", "CONST", "VOLATILE", "VOID",
  "BOOL", "COMPLEX", "IMAGINARY", "STRUCT", "UNION", "ENUM", "ELLIPSIS",
  "CASE", "DEFAULT", "IF", "ELSE", "SWITCH", "WHILE", "DO", "FOR", "GOTO",
  "CONTINUE", "BREAK", "RETURN", "'('", "')'", "'['", "']'", "'.'", "'{'",
  "'}'", "','", "'&'", "'*'", "'+'", "'-'", "'~'", "'!'", "'/'", "'%'",
  "'<'", "'>'", "'^'", "'|'", "'?'", "':'", "'='", "';'", "$accept",
  "primary_expression", "postfix_expression", "argument_expression_list",
  "unary_expression", "unary_operator", "cast_expression",
  "multiplicative_expression", "additive_expression", "shift_expression",
  "relational_expression", "equality_expression", "and_expression",
  "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_or_expression",
  "conditional_expression", "assignment_expression", "if_expression",
  "assignment_operator", "expression", "constant_expression",
  "declaration", "declaration_specifiers", "init_declarator_list",
  "init_declarator", "storage_class_specifier", "type_specifier",
  "struct_or_union_specifier", "struct_or_union",
  "struct_declaration_list", "struct_declaration",
  "specifier_qualifier_list", "struct_declarator_list",
  "struct_declarator", "enum_specifier", "enumerator_list", "enumerator",
  "type_qualifier", "function_specifier", "declarator",
  "direct_declarator", "pointer", "type_qualifier_list",
  "parameter_type_list", "parameter_list", "parameter_declaration",
  "identifier_list", "type_name", "abstract_declarator",
  "direct_abstract_declarator", "initializer", "initializer_list",
  "designation", "designator_list", "designator", "statement",
  "labeled_statement", "compound_statement", "block_item_list",
  "block_item", "expression_statement", "selection_statement",
  "iteration_statement", "jump_statement", "translation_unit",
  "control_line", "external_declaration", "function_definition",
  "declaration_list", 0
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
     315,   316,   317,   318,   319,   320,   321,    40,    41,    91,
      93,    46,   123,   125,    44,    38,    42,    43,    45,   126,
      33,    47,    37,    60,    62,    94,   124,    63,    58,    61,
      59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    91,    92,    92,    92,    92,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    94,    94,    95,    95,
      95,    95,    95,    95,    96,    96,    96,    96,    96,    96,
      97,    97,    98,    98,    98,    98,    99,    99,    99,   100,
     100,   100,   101,   101,   101,   101,   101,   102,   102,   102,
     103,   103,   104,   104,   105,   105,   106,   106,   107,   107,
     108,   108,   109,   109,   110,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   112,   112,   113,   114,
     114,   115,   115,   115,   115,   115,   115,   115,   115,   116,
     116,   117,   117,   118,   118,   118,   118,   118,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   120,   120,   120,   121,   121,   122,   122,
     123,   124,   124,   124,   124,   125,   125,   126,   126,   126,
     127,   127,   127,   127,   127,   128,   128,   129,   129,   130,
     130,   130,   131,   132,   132,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   134,   134,
     134,   134,   135,   135,   136,   136,   137,   137,   138,   138,
     138,   139,   139,   140,   140,   141,   141,   141,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   143,
     143,   143,   144,   144,   144,   144,   145,   146,   146,   147,
     147,   148,   148,   148,   148,   148,   148,   149,   149,   149,
     150,   150,   151,   151,   152,   152,   153,   153,   154,   154,
     154,   155,   155,   155,   155,   155,   155,   156,   156,   156,
     156,   156,   157,   157,   157,   158,   158,   158,   158,   159,
     159,   159,   160,   160,   161,   161
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     1,     4,     3,     4,
       3,     3,     2,     2,     6,     7,     1,     3,     1,     2,
       2,     2,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     5,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     2,
       3,     1,     2,     1,     2,     1,     2,     1,     2,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     4,     2,     1,     1,     1,     2,
       3,     2,     1,     2,     1,     1,     3,     1,     2,     3,
       4,     5,     5,     6,     2,     1,     3,     1,     3,     1,
       1,     1,     1,     2,     1,     1,     3,     5,     4,     4,
       6,     6,     5,     4,     3,     4,     4,     3,     1,     2,
       2,     3,     1,     2,     1,     3,     1,     3,     2,     2,
       1,     1,     3,     1,     2,     1,     1,     2,     3,     2,
       3,     3,     4,     3,     4,     2,     3,     3,     4,     1,
       3,     4,     1,     2,     3,     4,     2,     1,     2,     3,
       2,     1,     1,     1,     1,     1,     1,     3,     4,     3,
       2,     3,     1,     2,     1,     1,     1,     2,     5,     7,
       5,     5,     7,     6,     7,     6,     7,     3,     2,     2,
       2,     3,     1,     2,     1,     7,     5,     3,     0,     2,
       2,     0,     4,     3,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     238,   112,     0,    93,    94,    95,    96,    97,   142,   140,
      99,   100,   101,   102,   105,   106,   103,   104,   139,   141,
      98,   107,   108,   109,   116,   117,     0,   241,     0,    81,
      83,   110,     0,   111,    85,    87,     0,   234,   232,   241,
     238,     0,   134,     0,   240,   145,     0,   158,    79,     0,
      89,    91,   144,     0,    82,    84,   115,     0,    86,    88,
       1,   233,   239,   237,     0,     0,   137,     0,   135,     0,
     162,   160,   159,     0,    80,     0,     0,   244,     0,   243,
       0,     0,     0,   143,     0,   122,     0,   118,     0,   124,
       0,   238,     0,     0,   130,     0,   146,   163,   161,    90,
      91,     2,     3,     4,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   210,
      24,    25,    26,    27,    28,    29,   216,     6,    18,    30,
       0,    32,    36,    39,    42,    47,    50,    52,    54,    56,
      58,    60,    62,    76,     0,   214,   215,   201,   202,     0,
     212,   203,   204,   205,   206,     2,     0,   189,    92,   245,
     242,   171,   157,   170,     0,   164,   166,     0,     0,   154,
      25,     0,     0,     0,   121,   114,   119,     0,     0,   125,
     127,   123,     0,   236,   131,     0,    30,    78,   138,   132,
     136,     0,     0,    22,     0,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,   228,   229,   230,     0,     0,
     173,     0,     0,    12,    13,     0,     0,     0,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    65,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   217,   211,   213,     0,     0,   192,     0,     0,     0,
     197,     0,     0,   168,   175,   169,   176,   155,     0,   156,
       0,     0,   153,   149,     0,   148,    25,     0,   113,   128,
       0,   120,     0,   238,   133,   207,     0,     0,     0,   209,
      64,     0,     0,     0,     0,     0,     0,   227,   231,     5,
       0,   175,   174,     0,    11,     8,     0,    16,     0,    10,
      63,    33,    34,    35,    37,    38,    40,    41,    45,    46,
      43,    44,    48,    49,    51,    53,    55,    57,    59,     0,
      77,     0,   200,   190,     0,   193,   196,   198,   185,     0,
       0,   179,    25,     0,   177,     0,     0,   165,   167,   172,
       0,     0,   152,   147,   126,   129,   235,    23,     0,   208,
       0,     0,     0,     0,     0,     0,     0,    31,     9,     0,
       7,     0,   199,   191,   194,     0,   186,   178,   183,   180,
     187,     0,   181,    25,     0,   150,   151,   218,   220,   221,
       0,     0,     0,     0,     0,     0,    17,    61,   195,   188,
     184,   182,     0,     0,   225,     0,   223,     0,    14,     0,
     219,   222,   226,   224,    15
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   127,   128,   306,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   291,
     229,   144,   188,    27,    78,    49,    50,    29,    30,    31,
      32,    86,    87,    88,   178,   179,    33,    67,    68,    34,
      35,    69,    52,    53,    72,   339,   165,   166,   167,   211,
     340,   266,   256,   257,   258,   259,   260,   146,   147,   148,
     149,   150,   151,   152,   153,   154,    36,    37,    38,    39,
      80
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -319
static const yytype_int16 yypact[] =
{
    1427,  -319,     9,  -319,  -319,  -319,  -319,  -319,  -319,  -319,
    -319,  -319,  -319,  -319,  -319,  -319,  -319,  -319,  -319,  -319,
    -319,  -319,  -319,  -319,  -319,  -319,    22,  1453,     3,  1453,
    1453,  -319,    25,  -319,  1453,  1453,   985,  -319,  -319,  1453,
      16,    63,     6,    93,  -319,  -319,    23,    -3,  -319,    69,
    -319,  1134,   129,    38,  -319,  -319,    29,  1479,  -319,  -319,
    -319,  -319,  -319,  -319,   -24,    93,    15,   201,  -319,     1,
    -319,  -319,    -3,    23,  -319,   316,   230,  -319,     3,  -319,
    1318,  1095,   670,   129,  1479,  1479,  1236,  -319,    19,  1479,
     157,    16,   215,   913,  -319,    37,  -319,  -319,  -319,  -319,
      82,    26,  -319,  -319,   973,   991,   991,   913,    57,   132,
     154,   164,   523,   175,   250,   165,   167,   550,   644,  -319,
    -319,  -319,  -319,  -319,  -319,  -319,  -319,  -319,   262,   984,
     913,  -319,   146,   221,   301,    52,   299,   172,   178,   190,
     265,    44,  -319,  -319,    77,  -319,  -319,  -319,  -319,   394,
    -319,  -319,  -319,  -319,  -319,  -319,   756,  -319,  -319,  -319,
    -319,  -319,  -319,    56,   222,   219,  -319,   -20,   128,  -319,
     234,   253,   722,  1274,  -319,  -319,  -319,   913,    78,  -319,
     240,  -319,   246,  -319,  -319,    65,  -319,  -319,  -319,  -319,
    -319,   523,   644,  -319,   644,  -319,  -319,   248,   523,   913,
     913,   913,   272,   472,   252,  -319,  -319,  -319,    88,    59,
      70,   266,   337,  -319,  -319,   782,   913,   367,  -319,  -319,
    -319,  -319,  -319,  -319,  -319,  -319,  -319,  -319,  -319,   913,
    -319,   913,   913,   913,   913,   913,   913,   913,   913,   913,
     913,   913,   913,   913,   913,   913,   913,   913,   913,   913,
     913,  -319,  -319,  -319,   913,   371,  -319,   242,   230,   -18,
    -319,  1044,   412,  -319,    53,  -319,   163,  -319,  1400,  -319,
     387,   801,  -319,  -319,   913,  -319,   331,   334,  -319,  -319,
      19,  -319,   913,    16,  -319,  -319,   339,   340,   523,  -319,
     388,   341,   342,    66,   344,   566,   566,  -319,  -319,  -319,
    1194,   174,  -319,   835,  -319,  -319,   111,  -319,   121,  -319,
    -319,  -319,  -319,  -319,   146,   146,   221,   221,   301,   301,
     301,   301,    52,    52,   299,   172,   178,   190,   265,   -16,
    -319,   343,  -319,  -319,   588,  -319,  -319,  -319,  -319,   346,
     351,  -319,   353,   378,   163,  1361,   861,  -319,  -319,  -319,
     382,   392,  -319,  -319,  -319,  -319,  -319,   391,   391,  -319,
     523,   523,   523,   913,   879,   895,   756,  -319,  -319,   913,
    -319,   913,  -319,  -319,  -319,   230,  -319,  -319,  -319,  -319,
    -319,   396,  -319,   395,   398,  -319,  -319,   354,  -319,  -319,
     113,   523,   151,   523,   155,   244,  -319,  -319,  -319,  -319,
    -319,  -319,   523,   393,  -319,   523,  -319,   523,  -319,   704,
    -319,  -319,  -319,  -319,  -319
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -319,  -319,  -319,  -319,   -30,  -319,  -120,    92,   102,    46,
     142,   241,   249,   247,   239,   251,   187,   -88,   -74,   295,
    -319,   -86,   -89,   -31,     0,  -319,   423,  -319,    64,  -319,
    -319,   413,   -71,   -68,  -319,   218,  -319,   436,   -82,    72,
    -319,   -21,   -44,   -35,   -59,   -78,  -319,   267,  -319,    54,
    -125,  -218,   -72,   168,  -318,  -319,   271,  -111,  -319,     4,
    -319,   384,  -179,  -319,  -319,  -319,  -319,   -29,   153,  -319,
    -319
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      28,   202,   157,   164,   158,   187,    45,    51,   171,    83,
     230,    63,    71,   190,    40,   176,   375,   174,   197,   187,
      77,   181,    45,   172,   296,    42,    45,    28,    56,    54,
      55,   208,   209,     9,    58,    59,    28,    98,   265,    28,
      66,    45,    18,    19,   145,     2,   344,    90,   269,   159,
     210,   254,   100,   255,   270,    79,    45,   100,   250,    45,
      91,   248,   183,   186,   238,   239,    64,   180,    66,    96,
      46,   336,   371,    47,   193,   195,   196,   186,    65,    47,
     285,   163,   157,   344,   160,   302,    46,   289,   279,   187,
      46,   375,    41,    48,    43,    47,    66,    57,   277,    47,
     186,    84,   176,   190,    93,    46,   209,   177,   209,   271,
     189,   311,   312,   313,   191,   293,   364,   365,   145,    70,
     261,    85,   262,   261,   210,   262,   210,   299,   264,    89,
     308,   249,    47,   250,   362,   240,   241,   300,   284,   262,
     250,   307,   263,    73,    97,   198,    47,   186,    85,    85,
      85,   250,   280,    85,    70,   310,    89,    89,    89,    74,
     182,    89,   250,   329,     9,   331,   187,   251,   281,   186,
     186,    76,   295,    18,    19,   301,   330,   359,   298,   368,
      44,   403,    85,   367,   157,   369,   335,   250,   343,    61,
      89,   370,    62,   355,   187,   250,    81,   350,    82,   199,
     351,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   405,
      83,   200,   231,   407,   186,   250,   264,   232,   233,   250,
     345,   201,   346,   155,   102,   103,   104,    85,   105,   106,
      70,   300,   203,   262,    97,    89,   286,   244,   287,   387,
     388,   389,   186,   204,   356,   205,    85,   206,    85,   180,
     157,   163,   374,   245,    89,   301,    89,   381,   163,   212,
     213,   214,   384,   186,    94,    95,   246,   390,   392,   394,
     404,   247,   406,   397,   318,   319,   320,   321,   184,   185,
     267,   410,   157,   268,   412,   396,   413,   118,   234,   235,
     163,   157,   156,   398,   272,   120,   121,   122,   123,   124,
     125,   236,   237,   242,   243,   333,   334,   408,   409,   101,
     102,   103,   104,   273,   105,   106,   314,   315,   282,   215,
     283,   216,   294,   217,   303,   157,   288,   374,   316,   317,
     304,   186,   297,    97,     1,   163,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     309,   107,   108,   109,   332,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   322,   323,   290,   290,    75,   119,
     349,   120,   121,   122,   123,   124,   125,   101,   102,   103,
     104,   352,   105,   106,   353,   248,   126,   357,   358,   360,
     361,   363,   402,   372,   376,   155,   102,   103,   104,   377,
     105,   106,     1,   378,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   379,   107,
     108,   109,   385,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   386,   366,   399,   400,    75,   252,   401,   120,
     121,   122,   123,   124,   125,   155,   102,   103,   104,   118,
     105,   106,   341,   411,   126,   324,   327,   120,   342,   122,
     123,   124,   125,   326,   325,   292,    99,   173,   354,   328,
       1,    92,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   101,   102,   103,   104,
     337,   105,   106,   253,   395,   348,     0,     0,     0,   118,
       0,     0,     0,     0,     0,     0,     0,   120,   121,   122,
     123,   124,   125,   155,   102,   103,   104,     0,   105,   106,
       0,     0,   126,     0,     0,     0,     0,     0,     0,   155,
     102,   103,   104,     0,   105,   106,     0,     0,   107,   108,
     109,     0,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   155,   102,   103,   104,    75,   105,   106,   120,   121,
     122,   123,   124,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   126,     0,     0,     0,   118,     0,     0,
       0,     0,     0,     0,     0,   120,   121,   122,   123,   124,
     125,     0,     0,   118,     0,     0,     0,     0,     0,     0,
     207,   120,   121,   122,   123,   124,   125,   155,   102,   103,
     104,     0,   105,   106,     0,   118,   126,   254,     0,   255,
     156,   373,     0,   120,   121,   122,   123,   124,   125,     0,
       0,     0,     1,   155,   102,   103,   104,     0,   105,   106,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
       0,     0,   168,     0,     0,     0,     9,   155,   102,   103,
     104,   118,   105,   106,     0,    18,    19,     0,     0,   120,
     121,   122,   123,   124,   125,   155,   102,   103,   104,     0,
     105,   106,     0,     0,     0,     0,     0,   118,     0,     0,
     169,     0,     0,     0,     0,   120,   170,   122,   123,   124,
     125,     0,     0,     0,   274,     0,     0,     0,     9,   155,
     102,   103,   104,     0,   105,   106,     0,    18,    19,     0,
       0,   118,     0,   254,     0,   255,   156,   414,     0,   120,
     121,   122,   123,   124,   125,   155,   102,   103,   104,   118,
     105,   106,   275,     0,     0,     0,     0,   120,   276,   122,
     123,   124,   125,     0,   155,   102,   103,   104,     0,   105,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   118,     0,   254,     0,   255,   156,     0,
       0,   120,   121,   122,   123,   124,   125,     9,   155,   102,
     103,   104,     0,   105,   106,     0,    18,    19,     0,   118,
     305,     0,     0,     0,     0,     0,     0,   120,   121,   122,
     123,   124,   125,     0,   155,   102,   103,   104,   118,   105,
     106,     0,     0,     0,     0,     0,   120,   121,   122,   123,
     124,   125,   155,   102,   103,   104,     0,   105,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   155,   102,
     103,   104,   118,   105,   106,     0,     0,   366,     0,     0,
     120,   121,   122,   123,   124,   125,   155,   102,   103,   104,
       0,   105,   106,     0,     0,     0,     0,     0,   118,     0,
       0,   382,     0,     0,     0,     0,   120,   383,   122,   123,
     124,   125,     0,     0,     0,     0,   118,   391,     0,     0,
       0,     0,     0,     0,   120,   121,   122,   123,   124,   125,
       0,     0,   118,   393,     0,     0,     0,     0,     0,     0,
     120,   121,   122,   123,   124,   125,   155,   102,   103,   104,
     118,   105,   106,     0,     0,    60,     0,     0,   120,   121,
     122,   123,   124,   125,   155,   102,   103,   104,     0,   105,
     106,     0,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,     0,     1,     0,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
     192,     0,     0,     0,     0,     0,     0,    45,   120,   121,
     122,   123,   124,   125,     0,     0,     0,     0,   194,     0,
       0,     0,     0,     0,     0,     0,   120,   121,   122,   123,
     124,   125,     1,   228,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   261,   338,   262,     0,     0,     0,     0,     0,     0,
      47,     0,     0,     1,     0,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,   162,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,    76,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   300,   338,   262,     1,     0,     0,     0,     0,     0,
      47,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     0,     0,   175,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     1,   278,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
      75,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,   380,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   347,     1,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     1,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     1,     0,     0,
       0,     0,     0,     0,     0,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26
};

static const yytype_int16 yycheck[] =
{
       0,   112,    76,    81,    76,    93,     3,    28,    82,    53,
     130,    40,    47,    95,     5,    86,   334,    85,   107,   107,
      51,    89,     3,    82,   203,     3,     3,    27,     3,    29,
      30,   117,   118,    36,    34,    35,    36,    72,   163,    39,
       3,     3,    45,    46,    75,    29,   264,    71,    68,    80,
     118,    69,    73,    71,    74,    51,     3,    78,    74,     3,
      84,    17,    91,    93,    12,    13,     3,    88,     3,    68,
      67,    89,    88,    76,   104,   105,   106,   107,    72,    76,
     191,    81,   156,   301,    80,   210,    67,   198,   177,   177,
      67,   409,    83,    90,    72,    76,     3,    72,   172,    76,
     130,    72,   173,   185,    89,    67,   192,    88,   194,   168,
      73,   231,   232,   233,    88,   201,   295,   296,   149,    47,
      67,    57,    69,    67,   192,    69,   194,    68,   163,    57,
     216,    87,    76,    74,    68,    83,    84,    67,    73,    69,
      74,   215,   163,    74,    72,    88,    76,   177,    84,    85,
      86,    74,    74,    89,    82,   229,    84,    85,    86,    90,
       3,    89,    74,   249,    36,   254,   254,    90,    90,   199,
     200,    89,   203,    45,    46,   210,   250,   288,    90,    68,
      27,    68,   118,   303,   258,    74,   258,    74,   262,    36,
     118,    70,    39,   282,   282,    74,    67,   271,    69,    67,
     274,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,    68,
     264,    67,    76,    68,   254,    74,   261,    81,    82,    74,
      67,    67,    69,     3,     4,     5,     6,   173,     8,     9,
     168,    67,    67,    69,   172,   173,   192,    75,   194,   360,
     361,   362,   282,     3,   283,    90,   192,    90,   194,   280,
     334,   261,   334,    85,   192,   300,   194,   345,   268,     7,
       8,     9,   346,   303,    73,    74,    86,   363,   364,   365,
     391,    16,   393,   371,   238,   239,   240,   241,    73,    74,
      68,   402,   366,    74,   405,   369,   407,    67,    77,    78,
     300,   375,    72,   375,    70,    75,    76,    77,    78,    79,
      80,    10,    11,    14,    15,    73,    74,    73,    74,     3,
       4,     5,     6,    70,     8,     9,   234,   235,    88,    67,
      84,    69,    60,    71,    68,   409,    88,   409,   236,   237,
       3,   371,    90,   271,    28,   345,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
       3,    55,    56,    57,     3,    59,    60,    61,    62,    63,
      64,    65,    66,    67,   242,   243,   199,   200,    72,    73,
       3,    75,    76,    77,    78,    79,    80,     3,     4,     5,
       6,    70,     8,     9,    70,    17,    90,    68,    68,    68,
      68,    67,    58,    70,    68,     3,     4,     5,     6,    68,
       8,     9,    28,    70,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    70,    55,
      56,    57,    70,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    70,    72,    68,    70,    72,    73,    70,    75,
      76,    77,    78,    79,    80,     3,     4,     5,     6,    67,
       8,     9,    70,    90,    90,   244,   247,    75,    76,    77,
      78,    79,    80,   246,   245,   200,    73,    84,   280,   248,
      28,    65,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,     3,     4,     5,     6,
     259,     8,     9,   149,   366,   268,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      78,    79,    80,     3,     4,     5,     6,    -1,     8,     9,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,     8,     9,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,     3,     4,     5,     6,    72,     8,     9,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,
      80,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    75,    76,    77,    78,    79,    80,     3,     4,     5,
       6,    -1,     8,     9,    -1,    67,    90,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    28,     3,     4,     5,     6,    -1,     8,     9,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    36,     3,     4,     5,
       6,    67,     8,     9,    -1,    45,    46,    -1,    -1,    75,
      76,    77,    78,    79,    80,     3,     4,     5,     6,    -1,
       8,     9,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    32,    -1,    -1,    -1,    36,     3,
       4,     5,     6,    -1,     8,     9,    -1,    45,    46,    -1,
      -1,    67,    -1,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,     3,     4,     5,     6,    67,
       8,     9,    70,    -1,    -1,    -1,    -1,    75,    76,    77,
      78,    79,    80,    -1,     3,     4,     5,     6,    -1,     8,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    71,    72,    -1,
      -1,    75,    76,    77,    78,    79,    80,    36,     3,     4,
       5,     6,    -1,     8,     9,    -1,    45,    46,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      78,    79,    80,    -1,     3,     4,     5,     6,    67,     8,
       9,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    78,
      79,    80,     3,     4,     5,     6,    -1,     8,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    67,     8,     9,    -1,    -1,    72,    -1,    -1,
      75,    76,    77,    78,    79,    80,     3,     4,     5,     6,
      -1,     8,     9,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    75,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,    80,
      -1,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    78,    79,    80,     3,     4,     5,     6,
      67,     8,     9,    -1,    -1,     0,    -1,    -1,    75,    76,
      77,    78,    79,    80,     3,     4,     5,     6,    -1,     8,
       9,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    28,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,     3,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    78,
      79,    80,    28,    89,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    28,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    68,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    89,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    28,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    73,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      72,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    68,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    28,    -1,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,   114,   115,   118,
     119,   120,   121,   127,   130,   131,   157,   158,   159,   160,
       5,    83,     3,    72,   159,     3,    67,    76,    90,   116,
     117,   132,   133,   134,   115,   115,     3,    72,   115,   115,
       0,   159,   159,   158,     3,    72,     3,   128,   129,   132,
     130,   134,   135,    74,    90,    72,    89,   114,   115,   150,
     161,    67,    69,   133,    72,   119,   122,   123,   124,   130,
      71,    84,   128,    89,    73,    74,    68,   130,   134,   117,
     132,     3,     4,     5,     6,     8,     9,    55,    56,    57,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    73,
      75,    76,    77,    78,    79,    80,    90,    92,    93,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   112,   114,   148,   149,   150,   151,
     152,   153,   154,   155,   156,     3,    72,   109,   143,   114,
     150,     3,    68,   115,   136,   137,   138,   139,    32,    70,
      76,   109,   135,   122,   124,    73,   123,    88,   125,   126,
     132,   124,     3,   158,    73,    74,    95,   108,   113,    73,
     129,    88,    67,    95,    67,    95,    95,   113,    88,    67,
      67,    67,   148,    67,     3,    90,    90,    90,   112,   112,
     124,   140,     7,     8,     9,    67,    69,    71,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    89,   111,
      97,    76,    81,    82,    77,    78,    10,    11,    12,    13,
      83,    84,    14,    15,    75,    85,    86,    16,    17,    87,
      74,    90,    73,   152,    69,    71,   143,   144,   145,   146,
     147,    67,    69,   132,   134,   141,   142,    68,    74,    68,
      74,   135,    70,    70,    32,    70,    76,   109,    73,   113,
      74,    90,    88,    84,    73,   148,   140,   140,    88,   148,
     107,   110,   110,   112,    60,   114,   153,    90,    90,    68,
      67,   134,   141,    68,     3,    68,    94,   109,   112,     3,
     109,    97,    97,    97,    98,    98,    99,    99,   100,   100,
     100,   100,   101,   101,   102,   103,   104,   105,   106,   112,
     109,   113,     3,    73,    74,   143,    89,   147,    68,   136,
     141,    70,    76,   109,   142,    67,    69,    54,   138,     3,
     109,   109,    70,    70,   126,   113,   158,    68,    68,   148,
      68,    68,    68,    67,   153,   153,    72,    97,    68,    74,
      70,    88,    70,    73,   143,   145,    68,    68,    70,    70,
      68,   136,    70,    76,   109,    70,    70,   148,   148,   148,
     112,    68,   112,    68,   112,   144,   109,   108,   143,    68,
      70,    70,    58,    68,   148,    68,   148,    68,    73,    74,
     148,    90,   148,   148,    73
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
        case 241:
#line 474 "c.y"
    {if (printed == 0) printf ("Pass\n"); printed = 1;}
    break;


/* Line 1267 of yacc.c.  */
#line 2103 "y.tab.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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


#line 488 "c.y"



void yyerror(char const *s)
{
	printf("Error in line %d column %d : ", line,prevCol);
	printf("\"%s\"\n",s);
	printed = 1;
	exit (3);
}


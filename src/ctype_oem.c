/* THIS IS AN OEM VERSION */ 
/* ONLY TO BE COMPILED WITH THE OEM VERSION OF IB */

/* This file contains ctype tables and ranking functions */

#ifndef DOT_WORDS_SIGNATURE
# define DOT_WORDS_SIGNATURE 4
# undef  UNDERBAR_IS_CHAR
#endif

# define CTYPES_SIGNATURE   2

#ifndef UNDERBAR_IS_CHAR
# define UNDERBAR_IS_CHAR 0 /* 1 means _ is a term character */
#endif

#ifdef _IS_DOT_IN_WORD
# define DOT_WORDS_SIGNATURE 0
#else
# ifndef  DOT_WORDS_SIGNATURE 
#   define DOT_WORDS_SIGNATURE 4
# endif
#endif

#ifndef STANDALONE

int (* __Private_IRSET_Sort) (void *, int, void *, int, void *) = 0;
int (* __Private_RSET_Sort)  (void *, int, int) = 0;

// Private IB Error Messages to extend the standard Z codes
// Extend as needed. 
static const char *IB_ErrorMessage(int ErrorCode)
{
  switch (ErrorCode)
    {
      case -64: return "32-bit indexes are not compatible with 64-bit libs.";
      case -32: return "64-bit indexes are not compatible with 32-bit libs. ";
      default:  return "Unknown Error";
    }
}


const char *(*__Private_IB_ErrorMessage)(int) = IB_ErrorMessage;


/* Sanity check */
#ifdef UNDERBAR_IS_CHAR
# if (UNDERBAR_IS_CHAR > 0)
#   if (DOT_WORDS_SIGNATURE >= 2) && (DOT_WORDS_SIGNATURE <=5)
#     error "Should not define _ as both char and dot. 'Dot characters' should not be term characters!"
#   endif
# endif
#endif


int _ib_IsTermChr (const unsigned char *Buffer)
{
  return Buffer && (IsTermChar(Buffer[0]) || (IsDotInWord(Buffer[0]) &&
	(IsAfterDotChar(Buffer[1]) || (IsDotInWord(Buffer[1]) && IsAfterDotChar(Buffer[2])))));
}


int _ib_isdot(const int ch)
{
#if (DOT_WORDS_SIGNATURE == 7)
  /* Nothing: No dots in words */
# define _IS_DOT_IN_WORD(_x) 0
#elif (DOT_WORDS_SIGNATURE == 6)
# define _IS_DOT_IN_WORD(_x) ((_x) == '.' || (_x) == ';' || (_x) == ':' || (_x) == '+')
#elif (DOT_WORDS_SIGNATURE == 5)
  // AT&T edz@nonmonotonic.com Zimmermann_edward 1.B auto-mobile and URLs are all words
# define _IS_DOT_IN_WORD(_x) ((_x) == '.' || (_x) == '_' || (_x) == '&' || (_x) == '@' || \
	(_x) == '/' || (_x) = '-')
#elif (DOT_WORDS_SIGNATURE == 4) 
  /* AT&T edz@nonmonotonic.com Zimmermann_edward 1.B are all words */
# define _IS_DOT_IN_WORD(_x) ((_x) == '.' || (_x) == '_' || (_x) == '&' || (_x) == '@')
#elif (DOT_WORDS_SIGNATURE == 3)
  /* As above but edz@nonmonotonic.com is edz and nonmonotonic.com terms. edz@nonmonotonic.com via literal */
# define _IS_DOT_IN_WORD(_x) ((_x) == '.' || (_x) == '_' || (_x) == '&')
#elif (DOT_WORDS_SIGNATURE == 2)
  /* Just Zimmermann_Edward as term */
# define _IS_DOT_IN_WORD(_x) ((_x) == '.' || (_x) == '_')
#elif (DOT_WORDS_SIGNATURE == 1)
  /* Zimmermann_Edward as Zimmermann Edward and Zimmermann_Edward via literal */
# define _IS_DOT_IN_WORD(_x) ((_x) == '.'))
#endif
  return _IS_DOT_IN_WORD(ch);
}
#endif

/* -------------------------------------------------------------------------- */
/*

  CTYPE TABLES. 

  ----------------------------------------------------------------------------
*/
/*
Character Classification (8-bit sets)
	0x01  Upper case
	0x02  Lower case
	0x04  Numeral (digit)
	0x08  Spacing character
	0x10  Punctuation
	0x20  Control character
	0x40  Blank
	0x80  heXadecimal digit

Term characters are: Upper Case or Lower Case or Numerical.
Modifications to the table should, generally, be restricted to
punctuation characters for *very special* reasons.

To modify a table just change the value and recreate the shared library.

Using gcc:
  Solaris:
	gcc -fpic -G -o libIBctype.so.1 ctype.c
  Linux/BSD:
	gcc -fpic -shared -o libIBctype.so.1 ctype.c

Using the Sun Workshop/SunPro Compiler:
       cc -Kpic -G -o libIBctype.so.1 -h libIBctype.so.1 ctype.c

Consult handbook for other compilers...

Example:
  - if one wants to have _ as a term character:
    change the value '\020' (0x10 or a punctuation character) to '\003' 
    (0x03 or both an Upper and a Lower case letter)
  - if one wants to have @ as a term character:
    Just as above.. Change the value of @ from '\020' to '\003'
  - if one wants to consider a control character (e.g. ^B) as part of a
    term one changes its value from '\040' to '\003'
Note that we defined these characters as both upper and lower case as there
is no real case to these puntuation and control characters. 
*/
#if defined(UNDERBAR_IS_CHAR) && (UNDERBAR_IS_CHAR > 0)
# define _UBAR '\003'
#else
# define _UBAR '\020'
#endif

int _ib_ctype_signature_ = (CTYPES_SIGNATURE*100) +  (DOT_WORDS_SIGNATURE*2) + UNDERBAR_IS_CHAR;

const unsigned char _ib_ctype_8859_1[] = {
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\110', /*!*/'\020', /*"*/'\020', /*#*/'\020', /*$*/'\020', /*%*/'\020', /*&*/'\020', /*'*/'\020', 
  /*(*/'\020', /*)*/'\020', /***/'\020', /*+*/'\020', /*,*/'\020', /*-*/'\020', /*.*/'\020', /* / */'\020', 
  /*0*/'\204', /*1*/'\204', /*2*/'\204', /*3*/'\204', /*4*/'\204', /*5*/'\204', /*6*/'\204', /*7*/'\204', 
  /*8*/'\204', /*9*/'\204', /*:*/'\020', /*;*/'\020', /*<*/'\020', /*=*/'\020', /*>*/'\020', /*?*/'\020', 
  /*@*/'\020', /*A*/'\201', /*B*/'\201', /*C*/'\201', /*D*/'\201', /*E*/'\201', /*F*/'\201', /*G*/'\001', 
  /*H*/'\001', /*I*/'\001', /*J*/'\001', /*K*/'\001', /*L*/'\001', /*M*/'\001', /*N*/'\001', /*O*/'\001', 
  /*P*/'\001', /*Q*/'\001', /*R*/'\001', /*S*/'\001', /*T*/'\001', /*U*/'\001', /*V*/'\001', /*W*/'\001', 
  /*X*/'\001', /*Y*/'\001', /*Z*/'\001', /*[*/'\020', /*\*/'\020', /*]*/'\020', /*^*/'\020', /*_*/_UBAR, 
  /*`*/'\020', /*a*/'\202', /*b*/'\202', /*c*/'\202', /*d*/'\202', /*e*/'\202', /*f*/'\202', /*g*/'\002', 
  /*h*/'\002', /*i*/'\002', /*j*/'\002', /*k*/'\002', /*l*/'\002', /*m*/'\002', /*n*/'\002', /*o*/'\002', 
  /*p*/'\002', /*q*/'\002', /*r*/'\002', /*s*/'\002', /*t*/'\002', /*u*/'\002', /*v*/'\002', /*w*/'\002', 
  /*x*/'\002', /*y*/'\002', /*z*/'\002', /*{*/'\020', /*|*/'\020', /*}*/'\020', /*~*/'\020', /* */'\040', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\110', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', 
  /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', 
  /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', 
  /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\020', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\020', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002'
};

const unsigned char _ib_ctype_8859_2[] = {
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\110', /*!*/'\020', /*"*/'\020', /*#*/'\020', /*$*/'\020', /*%*/'\020', /*&*/'\020', /*'*/'\020', 
  /*(*/'\020', /*)*/'\020', /***/'\020', /*+*/'\020', /*,*/'\020', /*-*/'\020', /*.*/'\020', /* / */'\020', 
  /*0*/'\204', /*1*/'\204', /*2*/'\204', /*3*/'\204', /*4*/'\204', /*5*/'\204', /*6*/'\204', /*7*/'\204', 
  /*8*/'\204', /*9*/'\204', /*:*/'\020', /*;*/'\020', /*<*/'\020', /*=*/'\020', /*>*/'\020', /*?*/'\020', 
  /*@*/'\020', /*A*/'\201', /*B*/'\201', /*C*/'\201', /*D*/'\201', /*E*/'\201', /*F*/'\201', /*G*/'\001', 
  /*H*/'\001', /*I*/'\001', /*J*/'\001', /*K*/'\001', /*L*/'\001', /*M*/'\001', /*N*/'\001', /*O*/'\001', 
  /*P*/'\001', /*Q*/'\001', /*R*/'\001', /*S*/'\001', /*T*/'\001', /*U*/'\001', /*V*/'\001', /*W*/'\001', 
  /*X*/'\001', /*Y*/'\001', /*Z*/'\001', /*[*/'\020', /*\*/'\020', /*]*/'\020', /*^*/'\020', /*_*/_UBAR, 
  /*`*/'\020', /*a*/'\202', /*b*/'\202', /*c*/'\202', /*d*/'\202', /*e*/'\202', /*f*/'\202', /*g*/'\002', 
  /*h*/'\002', /*i*/'\002', /*j*/'\002', /*k*/'\002', /*l*/'\002', /*m*/'\002', /*n*/'\002', /*o*/'\002', 
  /*p*/'\002', /*q*/'\002', /*r*/'\002', /*s*/'\002', /*t*/'\002', /*u*/'\002', /*v*/'\002', /*w*/'\002', 
  /*x*/'\002', /*y*/'\002', /*z*/'\002', /*{*/'\020', /*|*/'\020', /*}*/'\020', /*~*/'\020', /* */'\040', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\110', /*�*/'\001', /* */'\000', /*�*/'\001', /* */'\000', /*�*/'\001', /*�*/'\001', /* */'\000', 
  /* */'\000', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /* */'\000', /*�*/'\001', /*�*/'\001', 
  /* */'\000', /*�*/'\002', /* */'\000', /*�*/'\002', /* */'\000', /*�*/'\002', /*�*/'\002', /* */'\000', 
  /* */'\000', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /* */'\000', /*�*/'\002', /*�*/'\002', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\020', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\020', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /* */'\000'
};

const unsigned char _ib_ctype_8859_3[] = {
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\110', /*!*/'\020', /*"*/'\020', /*#*/'\020', /*$*/'\020', /*%*/'\020', /*&*/'\020', /*'*/'\020', 
  /*(*/'\020', /*)*/'\020', /***/'\020', /*+*/'\020', /*,*/'\020', /*-*/'\020', /*.*/'\020', /* / */'\020', 
  /*0*/'\204', /*1*/'\204', /*2*/'\204', /*3*/'\204', /*4*/'\204', /*5*/'\204', /*6*/'\204', /*7*/'\204', 
  /*8*/'\204', /*9*/'\204', /*:*/'\020', /*;*/'\020', /*<*/'\020', /*=*/'\020', /*>*/'\020', /*?*/'\020', 
  /*@*/'\020', /*A*/'\201', /*B*/'\201', /*C*/'\201', /*D*/'\201', /*E*/'\201', /*F*/'\201', /*G*/'\001', 
  /*H*/'\001', /*I*/'\001', /*J*/'\001', /*K*/'\001', /*L*/'\001', /*M*/'\001', /*N*/'\001', /*O*/'\001', 
  /*P*/'\001', /*Q*/'\001', /*R*/'\001', /*S*/'\001', /*T*/'\001', /*U*/'\001', /*V*/'\001', /*W*/'\001', 
  /*X*/'\001', /*Y*/'\001', /*Z*/'\001', /*[*/'\020', /*\*/'\020', /*]*/'\020', /*^*/'\020', /*_*/_UBAR, 
  /*`*/'\020', /*a*/'\202', /*b*/'\202', /*c*/'\202', /*d*/'\202', /*e*/'\202', /*f*/'\202', /*g*/'\002', 
  /*h*/'\002', /*i*/'\002', /*j*/'\002', /*k*/'\002', /*l*/'\002', /*m*/'\002', /*n*/'\002', /*o*/'\002', 
  /*p*/'\002', /*q*/'\002', /*r*/'\002', /*s*/'\002', /*t*/'\002', /*u*/'\002', /*v*/'\002', /*w*/'\002', 
  /*x*/'\002', /*y*/'\002', /*z*/'\002', /*{*/'\020', /*|*/'\020', /*}*/'\020', /*~*/'\020', /* */'\040', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\110', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', 
  /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', 
  /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', 
  /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\020', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\020', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002'
};

const unsigned char _ib_ctype_8859_5[] = {
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\110', /*!*/'\020', /*"*/'\020', /*#*/'\020', /*$*/'\020', /*%*/'\020', /*&*/'\020', /*'*/'\020', 
  /*(*/'\020', /*)*/'\020', /***/'\020', /*+*/'\020', /*,*/'\020', /*-*/'\020', /*.*/'\020', /* / */'\020', 
  /*0*/'\204', /*1*/'\204', /*2*/'\204', /*3*/'\204', /*4*/'\204', /*5*/'\204', /*6*/'\204', /*7*/'\204', 
  /*8*/'\204', /*9*/'\204', /*:*/'\020', /*;*/'\020', /*<*/'\020', /*=*/'\020', /*>*/'\020', /*?*/'\020', 
  /*@*/'\020', /*A*/'\201', /*B*/'\201', /*C*/'\201', /*D*/'\201', /*E*/'\201', /*F*/'\201', /*G*/'\001', 
  /*H*/'\001', /*I*/'\001', /*J*/'\001', /*K*/'\001', /*L*/'\001', /*M*/'\001', /*N*/'\001', /*O*/'\001', 
  /*P*/'\001', /*Q*/'\001', /*R*/'\001', /*S*/'\001', /*T*/'\001', /*U*/'\001', /*V*/'\001', /*W*/'\001', 
  /*X*/'\001', /*Y*/'\001', /*Z*/'\001', /*[*/'\020', /*\*/'\020', /*]*/'\020', /*^*/'\020', /*_*/_UBAR, 
  /*`*/'\020', /*a*/'\202', /*b*/'\202', /*c*/'\202', /*d*/'\202', /*e*/'\202', /*f*/'\202', /*g*/'\002', 
  /*h*/'\002', /*i*/'\002', /*j*/'\002', /*k*/'\002', /*l*/'\002', /*m*/'\002', /*n*/'\002', /*o*/'\002', 
  /*p*/'\002', /*q*/'\002', /*r*/'\002', /*s*/'\002', /*t*/'\002', /*u*/'\002', /*v*/'\002', /*w*/'\002', 
  /*x*/'\002', /*y*/'\002', /*z*/'\002', /*{*/'\020', /*|*/'\020', /*}*/'\020', /*~*/'\020', /* */'\040', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\110', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\020', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\020', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /* */'\000', /*�*/'\002', /*�*/'\002'
};


const unsigned char _ib_ctype_8859_7[] = {
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\110', /*!*/'\020', /*"*/'\020', /*#*/'\020', /*$*/'\020', /*%*/'\020', /*&*/'\020', /*'*/'\020', 
  /*(*/'\020', /*)*/'\020', /***/'\020', /*+*/'\020', /*,*/'\020', /*-*/'\020', /*.*/'\020', /* / */'\020', 
  /*0*/'\204', /*1*/'\204', /*2*/'\204', /*3*/'\204', /*4*/'\204', /*5*/'\204', /*6*/'\204', /*7*/'\204', 
  /*8*/'\204', /*9*/'\204', /*:*/'\020', /*;*/'\020', /*<*/'\020', /*=*/'\020', /*>*/'\020', /*?*/'\020', 
  /*@*/'\020', /*A*/'\201', /*B*/'\201', /*C*/'\201', /*D*/'\201', /*E*/'\201', /*F*/'\201', /*G*/'\001', 
  /*H*/'\001', /*I*/'\001', /*J*/'\001', /*K*/'\001', /*L*/'\001', /*M*/'\001', /*N*/'\001', /*O*/'\001', 
  /*P*/'\001', /*Q*/'\001', /*R*/'\001', /*S*/'\001', /*T*/'\001', /*U*/'\001', /*V*/'\001', /*W*/'\001', 
  /*X*/'\001', /*Y*/'\001', /*Z*/'\001', /*[*/'\020', /*\*/'\020', /*]*/'\020', /*^*/'\020', /*_*/_UBAR, 
  /*`*/'\020', /*a*/'\202', /*b*/'\202', /*c*/'\202', /*d*/'\202', /*e*/'\202', /*f*/'\202', /*g*/'\002', 
  /*h*/'\002', /*i*/'\002', /*j*/'\002', /*k*/'\002', /*l*/'\002', /*m*/'\002', /*n*/'\002', /*o*/'\002', 
  /*p*/'\002', /*q*/'\002', /*r*/'\002', /*s*/'\002', /*t*/'\002', /*u*/'\002', /*v*/'\002', /*w*/'\002', 
  /*x*/'\002', /*y*/'\002', /*z*/'\002', /*{*/'\020', /*|*/'\020', /*}*/'\020', /*~*/'\020', /* */'\040', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\110', /*�*/'\020', /*�*/'\020', /*�*/'\020', /* */'\000', /* */'\000', /*�*/'\020', /*�*/'\020', 
  /*�*/'\020', /*�*/'\020', /* */'\000', /*�*/'\020', /*�*/'\020', /*�*/'\020', /* */'\000', /*�*/'\020', 
  /*�*/'\020', /*�*/'\020', /*�*/'\020', /*�*/'\020', /* */'\000', /* */'\000', /*�*/'\001', /*�*/'\020', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\020', /*�*/'\001', /*�*/'\020', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /* */'\000', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', 
  /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\001', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', 
  /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /*�*/'\002', /* */'\000'
};

unsigned char _ib_ctype_ASCII[] = {
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\050', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', /* */'\040', 
  /* */'\110', /*!*/'\020', /*"*/'\020', /*#*/'\020', /*$*/'\020', /*%*/'\020', /*&*/'\020', /*'*/'\020', 
  /*(*/'\020', /*)*/'\020', /***/'\020', /*+*/'\020', /*,*/'\020', /*-*/'\020', /*.*/'\020', /* / */'\020', 
  /*0*/'\204', /*1*/'\204', /*2*/'\204', /*3*/'\204', /*4*/'\204', /*5*/'\204', /*6*/'\204', /*7*/'\204', 
  /*8*/'\204', /*9*/'\204', /*:*/'\020', /*;*/'\020', /*<*/'\020', /*=*/'\020', /*>*/'\020', /*?*/'\020', 
  /*@*/'\020', /*A*/'\201', /*B*/'\201', /*C*/'\201', /*D*/'\201', /*E*/'\201', /*F*/'\201', /*G*/'\001', 
  /*H*/'\001', /*I*/'\001', /*J*/'\001', /*K*/'\001', /*L*/'\001', /*M*/'\001', /*N*/'\001', /*O*/'\001', 
  /*P*/'\001', /*Q*/'\001', /*R*/'\001', /*S*/'\001', /*T*/'\001', /*U*/'\001', /*V*/'\001', /*W*/'\001', 
  /*X*/'\001', /*Y*/'\001', /*Z*/'\001', /*[*/'\020', /*\*/'\020', /*]*/'\020', /*^*/'\020', /*_*/_UBAR, 
  /*`*/'\020', /*a*/'\202', /*b*/'\202', /*c*/'\202', /*d*/'\202', /*e*/'\202', /*f*/'\202', /*g*/'\002', 
  /*h*/'\002', /*i*/'\002', /*j*/'\002', /*k*/'\002', /*l*/'\002', /*m*/'\002', /*n*/'\002', /*o*/'\002', 
  /*p*/'\002', /*q*/'\002', /*r*/'\002', /*s*/'\002', /*t*/'\002', /*u*/'\002', /*v*/'\002', /*w*/'\002', 
  /*x*/'\002', /*y*/'\002', /*z*/'\002', /*{*/'\020', /*|*/'\020', /*}*/'\020', /*~*/'\020', /* */'\040', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', 
  /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000', /* */'\000'
};


#ifndef STANDALONE

/* -------------------------------------------------------------------------- */
/*

  RANKING TABLES

  These ranking function ONLY effect runtime behavior and may be changed at
  any time without effecting indexes.


*/
/* -------------------------------------------------------------------------- */

/* Newrank added here as a place to configure */

float _ib_Newsrank_weight_factor_hours(int hours)
{
  int   days;
  float weight = 1;

  if (hours <0 && hours > -18) hours = 2; // Correction for time zone glitches

  days = hours/24;

  if (days == -1)       weight = -7.00; // Slight penalty
  else if (days < 0)    weight = days * 6.0; // heavier penalty
  else if (days == 0)   weight = 10.00;
  if (days <= 1)   weight += 5.0;
  if (days <= 2)   weight += 4.0;
  if (days <= 3)   weight += 1.75;
  if (days <= 7)   weight += 0.75;
  if (days <= 14)  weight += 0.55;
  if (days <= 24)  weight += 0.43;
  if (days <= 60)  weight += 0.32;
  if (days <= 100) weight += 0.21;
  if (days <= 300) weight += 0.10;

  if (hours <= 2)
    weight *= 6;
  else if (hours <= 3)
    weight *= 5;
  else if (hours <= 4)
    weight *= 4.2;
  else if (hours <= 8)
    weight *= 3.6;
  else if (hours <= 12)
    weight *= 3.2;
  else if (hours <= 16)
    weight *= 2.6;
  else if (hours <= 23)
    weight *= 2.2;
  return weight;
}

float _ib_Newsrank_weight_factor(int days)
{
  return _ib_Newsrank_weight_factor_hours(days*24);
}



float _ib_Distrank_weight_factor(const int distance)
{
  if (distance <= 4)
    return 20;
  if (distance <= 10)
    return 8;
  if (distance <= 100)
    return 1.5;
  if (distance > 1000)
    return 0.98;
  return 1;
}

#endif

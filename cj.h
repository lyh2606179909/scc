
#ifndef  LVV_CODEJAM_H
#define  LVV_CODEJAM_H

#ifndef LVV_SIMPLE_H
#include <scc/simple.h>
#endif

#ifndef LVV_MATRIX_H
#include <scc/matrix.h>
#endif
///////////////////////////////////////////////////////////////////// SHORTCUTS

///// types
typedef		const	int			cint;
typedef		std::vector<int>		vint;
typedef		std::vector<unsigned int>	vuint;
typedef		std::vector<long>		vlong;
typedef		std::vector<unsigned long>	vulong;
typedef		std::vector<char>		vchar;
typedef		std::vector<float>		vfloat;
typedef		std::vector<double>		vdouble;

typedef		std::deque<int>			dint;
typedef		std::deque<int>			dlong;
typedef		std::deque<unsigned int>	duint;
typedef		std::deque<float>		dfloat;
typedef		std::deque<double>		ddouble;
typedef		std::deque<char>		dchar;

typedef		std::list<int>			lint;
typedef		std::list<unsigned int>		luint;
typedef		std::list<long>			llong;
typedef		std::list<unsigned long>	lulong;
typedef		std::list<char>			lchar;
typedef		std::list<float>		lfloat;
typedef		std::list<double>		ldouble;

typedef		std::string			str;
typedef		std::vector<std::string>	vstr;
typedef		std::deque<std::string>		dstr;
typedef		std::list<std::string>		lstr;

///// utils
#define		MT		std::make_tuple
#define		MP		std::make_pair

#define		GL(x)		std::getline(cin,x)
#define		NL		cin.ignore(numeric_limits<std::streamsize>::max(),'\n');


///////////////////////////////////////////////////////////////////// CLASSIC CODEJAM


// TODO #define FOR(i,a,b)  for(int i=(a),_##i=(b);i<_##i;++i)
#define		FOR(i,i0,N)	for (long i = (i0), max_##i=(N);   i < max_##i;   i++)
#define		ROF(i,a,b)	for (long i = (b)-1; i >= long(a); i--)
#define		REP(N)		for (long i_REP_ue923u=0, N_REP_2uf23f=(N);   i_REP_ue923u< N_REP_2uf23f;   i_REP_ue923u++)

#define		iFOR(N)		FOR(i,0,N)
#define		jFOR(N)		FOR(j,0,N)
#define		kFOR(N)		FOR(k,0,N)
#define		tFOR(N)		FOR(t,0,N)
#define		nFOR(N)		FOR(m,0,N)
#define		mFOR(N)		FOR(m,0,N)

#define		ALL(IT, C)	for (auto IT=begin(C);  IT != end(C);   IT++)
#define		LLA(IT, C)	for (auto IT=end(C)-1;  IT >= begin(C);   IT--)
#define		itALL(C)	ALL(it,C)
#define		pALL(C)		ALL(p,C)
#define		qALL(C)		ALL(q,C)
#define		pLLA(C)		LLA(p,C)
#define		iALL(C)		for (long i=0;  i<(long)(end(C)-begin(C));   i++)
#define		jALL(C)		for (long j=0;  j<(long)(end(C)-begin(C));   j++)
#define		kALL(C)		for (long k=0;  k<(long)(end(C)-begin(C));   k++)
#define		lALL(C)		for (long l=0;  l<(long)(end(C)-begin(C));   l++)
#define		mALL(C)		for (long m=0;  m<(long)(end(C)-begin(C));   m++)
#define		nALL(C)		for (long n=0;  n<(long)(end(C)-begin(C));   n++)
#define		cALL(C)		for (auto c:C) if(c == '\0') break;  else
#define		xALL(C)		for (auto x:C)




#endif  // LVV_CODEJAM_H

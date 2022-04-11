#include <immintrin.h>

__m256d _mm256_log_pd(__m256d x) {
	__m256i gDoubleExpMask = _mm256_set1_epi64x(0x7ffULL << 52);
	__m256i gDoubleExp0 = _mm256_set1_epi64x(1023ULL << 52);
	__m256i gTo32bitExp = _mm256_set_epi32(0, 0, 0, 0, 6, 4, 2, 0);
	__m128i gExpNormalizer = _mm_set1_epi32(1023);
	//TODO: some 128-bit variable or two 64-bit variables here?
	__m256d gCommMul = _mm256_set1_pd(2.0 / 0.693147180559945309417); // 2.0/ln(2)
	__m256d gCoeff1 = _mm256_set1_pd(1.0 / 3);
	__m256d gCoeff2 = _mm256_set1_pd(1.0 / 5);
	__m256d gCoeff3 = _mm256_set1_pd(1.0 / 7);
	__m256d gCoeff4 = _mm256_set1_pd(1.0 / 9);
	__m256d gVect1 = _mm256_set1_pd(1.0);


	__m256i exps64 = _mm256_srli_epi64(_mm256_and_si256(gDoubleExpMask, _mm256_castpd_si256(x)), 52);
	__m256i exps32_avx = _mm256_permutevar8x32_epi32(exps64, gTo32bitExp);
	__m128i exps32_sse = _mm256_castsi256_si128(exps32_avx);
	__m128i normExps = _mm_sub_epi32(exps32_sse, gExpNormalizer);
	__m256d expsPD = _mm256_cvtepi32_pd(normExps);
	__m256d y = _mm256_or_pd(_mm256_castsi256_pd(gDoubleExp0),
	_mm256_andnot_pd(_mm256_castsi256_pd(gDoubleExpMask), x));

	// Calculate t=(y-1)/(y+1) and t**2
	__m256d tNum = _mm256_sub_pd(y, gVect1);
	__m256d tDen = _mm256_add_pd(y, gVect1);
	__m256d t = _mm256_div_pd(tNum, tDen);
	__m256d t2 = _mm256_mul_pd(t, t); // t**2

	__m256d t3 = _mm256_mul_pd(t, t2); // t**3
	__m256d terms01 = _mm256_fmadd_pd(gCoeff1, t3, t);
	__m256d t5 = _mm256_mul_pd(t3, t2); // t**5
	__m256d terms012 = _mm256_fmadd_pd(gCoeff2, t5, terms01);
	__m256d t7 = _mm256_mul_pd(t5, t2); // t**7
	__m256d terms0123 = _mm256_fmadd_pd(gCoeff3, t7, terms012);
	__m256d t9 = _mm256_mul_pd(t7, t2); // t**9
	__m256d terms01234 = _mm256_fmadd_pd(gCoeff4, t9, terms0123);

	__m256d log2_y = _mm256_mul_pd(terms01234, gCommMul);
	__m256d log2_x = _mm256_add_pd(log2_y, expsPD);

	return log2_x;
}

__m256d abs_mask(){
	// with clang, this turns into a 16B load,
	// with every calling function getting its own copy of the mask
	__m256i minus1 = _mm256_set1_epi32(-1);
	return _mm256_castsi256_pd(_mm256_srli_epi32(minus1, 1));
}
// MSVC is BAD when inlining this into loops
__m256d __mm256_abs_pd(__m256d v) {
	return _mm256_and_pd(abs_mask(), v);
}

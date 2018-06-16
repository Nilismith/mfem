// Copyright (c) 2010, Lawrence Livermore National Security, LLC. Produced at
// the Lawrence Livermore National Laboratory. LLNL-CODE-443211. All Rights
// reserved. See file COPYRIGHT for details.
//
// This file is part of the MFEM library. For more information and source code
// availability see http://mfem.org.
//
// MFEM is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License (as published by the Free
// Software Foundation) version 2.1 dated February 1999.

#ifndef MFEM_TEMPLATE_CONFIG
#define MFEM_TEMPLATE_CONFIG

// the main MFEM config header
#include "config.hpp"

// --- MFEM_STATIC_ASSERT
#if (__cplusplus >= 201103L)
#define MFEM_STATIC_ASSERT(cond, msg) static_assert((cond), msg)
#else
#define MFEM_STATIC_ASSERT(cond, msg) if (cond) { }
#endif

// --- MFEM_ALWAYS_INLINE
#if !defined(MFEM_DEBUG) && (defined(__GNUC__) || defined(__clang__))
#define MFEM_ALWAYS_INLINE __attribute__((always_inline))
#else
#define MFEM_ALWAYS_INLINE
#endif

// --- MFEM_VECTORIZE_LOOP (disabled)
#if (__cplusplus >= 201103L) && !defined(MFEM_DEBUG) && defined(__GNUC__)
// #define MFEM_VECTORIZE_LOOP _Pragma("GCC ivdep")
#define MFEM_VECTORIZE_LOOP
#else
#define MFEM_VECTORIZE_LOOP
#endif

#define MFEM_TEMPLATE_BLOCK_SIZE 4
#define MFEM_SIMD_SIZE 32
#define MFEM_TEMPLATE_ENABLE_SERIALIZE

#ifdef MFEM_USE_X86INTRIN
#include "general/x86intrin.hpp"
#endif

// -- MFEM_ALIGN_AS
#if (__cplusplus >= 201103L)
#define MFEM_ALIGN_AS(bytes) alignas(bytes)
#elif !defined(MFEM_DEBUG) && (defined(__GNUC__) || defined(__clang__))
#define MFEM_ALIGN_AS(bytes) __attribute__ ((aligned (bytes)))
#else
#define MFEM_ALIGN_AS(bytes)
#endif

// #define MFEM_TEMPLATE_ELTRANS_HAS_NODE_DOFS
// #define MFEM_TEMPLATE_ELTRANS_RESULT_HAS_NODES
// #define MFEM_TEMPLATE_FIELD_EVAL_DATA_HAS_DOFS
#define MFEM_TEMPLATE_INTRULE_COEFF_PRECOMP

// derived macros
#define MFEM_ROUNDUP(val,base) ((((val)+(base)-1)/(base))*(base))
#define MFEM_ALIGN_SIZE(size,type) \
   MFEM_ROUNDUP(size,(MFEM_SIMD_SIZE)/sizeof(type))

namespace mfem
{
namespace internal
{
long long flop_count;
}
}

#ifdef MFEM_COUNT_FLOPS
#define MFEM_FLOPS_RESET() (mfem::internal::flop_count = 0)
#define MFEM_FLOPS_ADD(cnt) (mfem::internal::flop_count += (cnt))
#define MFEM_FLOPS_GET() (mfem::internal::flop_count)
#else
#define MFEM_FLOPS_RESET()
#define MFEM_FLOPS_ADD(cnt)
#define MFEM_FLOPS_GET() (0)
#endif

template <typename scalar_t, int S, int align_S = 1>
struct MFEM_ALIGN_AS(align_S*sizeof(scalar_t)) AutoSIMD
{
   typedef scalar_t scalar_type;
   static const int size = S;
   static const int align_size = align_S;

   scalar_t vec[size];

   scalar_t &operator[](int i) { return vec[i]; }
   const scalar_t &operator[](int i) const { return vec[i]; }

   AutoSIMD &operator=(const AutoSIMD &v)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] = v[i]; }
      return *this;
   }
   AutoSIMD &operator=(const scalar_t &e)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] = e; }
      return *this;
   }
   AutoSIMD &operator+=(const AutoSIMD &v)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] += v[i]; }
      return *this;
   }
   AutoSIMD &operator+=(const scalar_t &e)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] += e; }
      return *this;
   }
   AutoSIMD &operator-=(const AutoSIMD &v)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] -= v[i]; }
      return *this;
   }
   AutoSIMD &operator-=(const scalar_t &e)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] -= e; }
      return *this;
   }
   AutoSIMD &operator*=(const AutoSIMD &v)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] *= v[i]; }
      return *this;
   }
   AutoSIMD &operator*=(const scalar_t &e)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] *= e; }
      return *this;
   }
   AutoSIMD &operator/=(const AutoSIMD &v)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] /= v[i]; }
      return *this;
   }
   AutoSIMD &operator/=(const scalar_t &e)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] /= e; }
      return *this;
   }

   AutoSIMD operator-() const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = -vec[i]; }
      return r;
   }

   AutoSIMD operator+(const AutoSIMD &v) const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = vec[i] + v[i]; }
      return r;
   }
   AutoSIMD operator+(const scalar_t &e) const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = vec[i] + e; }
      return r;
   }
   AutoSIMD operator-(const AutoSIMD &v) const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = vec[i] - v[i]; }
      return r;
   }
   AutoSIMD operator-(const scalar_t &e) const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = vec[i] - e; }
      return r;
   }
   AutoSIMD operator*(const AutoSIMD &v) const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = vec[i] * v[i]; }
      return r;
   }
   AutoSIMD operator*(const scalar_t &e) const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = vec[i] * e; }
      return r;
   }
   AutoSIMD operator/(const AutoSIMD &v) const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = vec[i] / v[i]; }
      return r;
   }
   AutoSIMD operator/(const scalar_t &e) const
   {
      AutoSIMD r;
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { r[i] = vec[i] / e; }
      return r;
   }

   AutoSIMD &fma(const AutoSIMD &v, const AutoSIMD &w)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] += v[i] * w[i]; }
      return *this;
   }
   AutoSIMD &fma(const AutoSIMD &v, const scalar_t &e)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] += v[i] * e; }
      return *this;
   }
   AutoSIMD &fma(const scalar_t &e, const AutoSIMD &v)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] += e * v[i]; }
      return *this;
   }

   AutoSIMD &mul(const AutoSIMD &v, const AutoSIMD &w)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] = v[i] * w[i]; }
      return *this;
   }
   AutoSIMD &mul(const AutoSIMD &v, const scalar_t &e)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] = v[i] * e; }
      return *this;
   }
   AutoSIMD &mul(const scalar_t &e, const AutoSIMD &v)
   {
      MFEM_VECTORIZE_LOOP
      for (int i = 0; i < size; i++) { vec[i] = e * v[i]; }
      return *this;
   }
};

template <typename scalar_t, int S, int A>
AutoSIMD<scalar_t,S,A> operator+(const scalar_t &e,
                                 const AutoSIMD<scalar_t,S,A> &v)
{
   AutoSIMD<scalar_t,S,A> r;
   MFEM_VECTORIZE_LOOP
   for (int i = 0; i < S; i++) { r[i] = e + v[i]; }
   return r;
}

template <typename scalar_t, int S, int A>
AutoSIMD<scalar_t,S,A> operator-(const scalar_t &e,
                                 const AutoSIMD<scalar_t,S,A> &v)
{
   AutoSIMD<scalar_t,S,A> r;
   MFEM_VECTORIZE_LOOP
   for (int i = 0; i < S; i++) { r[i] = e - v[i]; }
   return r;
}

template <typename scalar_t, int S, int A>
AutoSIMD<scalar_t,S,A> operator*(const scalar_t &e,
                                 const AutoSIMD<scalar_t,S,A> &v)
{
   AutoSIMD<scalar_t,S,A> r;
   MFEM_VECTORIZE_LOOP
   for (int i = 0; i < S; i++) { r[i] = e * v[i]; }
   return r;
}

template <typename scalar_t, int S, int A>
AutoSIMD<scalar_t,S,A> operator/(const scalar_t &e,
                                 const AutoSIMD<scalar_t,S,A> &v)
{
   AutoSIMD<scalar_t,S,A> r;
   MFEM_VECTORIZE_LOOP
   for (int i = 0; i < S; i++) { r[i] = e / v[i]; }
   return r;
}


template<typename complex_t, typename real_t>
struct AutoImplTraits
{
   static const int block_size = MFEM_TEMPLATE_BLOCK_SIZE;
   static const int align_size = MFEM_SIMD_SIZE; // in bytes

   // static const int simd_size = MFEM_SIMD_SIZE/sizeof(complex_t);
   static const int simd_size = 1;
   static const int valign_size = simd_size;
   // static const int valign_size = 1;
   static const int batch_size = 1;
   typedef AutoSIMD<complex_t,simd_size,valign_size> vcomplex_t;
   typedef AutoSIMD<   real_t,simd_size,valign_size> vreal_t;
   typedef AutoSIMD<      int,simd_size,valign_size> vint_t;
};

#endif // MFEM_TEMPLATE_CONFIG

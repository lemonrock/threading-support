// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.


#ifndef LEMONROCK_C_COMPATIBILITY_H
	
	#define LEMONROCK_C_COMPATIBILITY_H
	
	#ifdef __cplusplus
		extern "C" {
	#endif
	
	
	
		#if defined(__APPLE__) && defined(__MACH__)
			#define IsMacOsX 1
		#endif


		#if defined(__GNUC__) && (__GNUC__ >= 4)
			#define IsCompilerGcc4OrLaterCompatible 1
		#endif


		#if defined(_MSC_VER) && (_MSC_VER >= 1700)
			#define IsCompilerMsvc1700OrLaterCompatible 1
		#endif


		#if defined(__clang__)
			#define IsCompilerClang
		#endif
		
			
		#ifndef __STDC__
			#define const
			#define volatile
		#endif


		#ifdef IsCompilerGcc4OrLaterCompatible
			#define WarnUnusuedResult __attribute__ ((warn_unused_result))
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#define WarnUnusuedResult _Check_return_
		#else
			#define WarnUnusuedResult
		#endif


		#if defined(IsCompilerMsvc1700OrLaterCompatible)
			#define ThreadLocal __declspec(thread)
		#elif defined(IsCompilerGcc4OrLaterCompatible)
			// Thread-Local storage is not supported in Mach-O binaries
			#ifdef IsMacOsX
				#define ThreadLocal
			#else
				#define ThreadLocal __thread
			#endif
		#else
			#define ThreadLocal
		#endif


		#ifdef IsCompilerGcc4OrLaterCompatible
			#define restrict restrict
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#define restrict __declspec(restrict)
		#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
			#define restrict restrict
		#else
			#define restrict
		#endif


		#ifdef IsCompilerGcc4OrLaterCompatible
			#define NoThrow __attribute__ ((nothrow))
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#define NoThrow __declspec(nothrow)
		#else
			#define NoThrow
		#endif


		#ifdef IsCompilerGcc4OrLaterCompatible
			#define NoReturn __attribute__ ((noreturn))
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#define NoReturn __declspec(noreturn)
		#else
			#define NoReturn
		#endif


		#ifdef IsCompilerGcc4OrLaterCompatible
			#define NoInline __attribute__ ((noinline))
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#define NoInline __declspec(noinline)
		#else
			#define NoInline
		#endif


		#ifdef IsCompilerGcc4OrLaterCompatible
			#define Deprecated(message) __attribute__ ((deprecated(message)))
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#define Deprecated(message) __declspec(deprecated(message))
		#else
			#define Deprecated
		#endif


		#ifdef IsCompilerGcc4OrLaterCompatible
			#define Aligned(x) __attribute__ ((aligned(x)))
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#define Aligned(x) __declspec(align(x))
		#else
			#define Aligned(x)
		#endif

		#ifdef IsCompilerGcc4OrLaterCompatible
			#define Hot __attribute__ ((hot))
		#else
			#define Hot
		#endif


		#ifdef IsCompilerGcc4OrLaterCompatible
			#define Cold __attribute__ ((cold))
		#else
			#define Cold
		#endif


		#if !defined(IsCompilerGcc4OrLaterCompatible)
			#define __builtin_expect(expr, expected) (expr)
		#endif

		// Useful for replacing asserts to assist the optimiser: https://msdn.microsoft.com/en-us/library/1b3fsfxw.aspx
		#ifdef IsCompilerClang
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#define __builtin_assume(x) __assume(x)
		#else
			#define __builtin_assume(x) (x)
		#endif

		#if !defined(IsCompilerGcc4OrLaterCompatible)
			#define __builtin_unreachable()
		#endif
		
		// We should move to the new GCC __atomic builtins, and probably to MSVC / Windows amp.h's atomic_fetch_add Function
		#ifdef IsCompilerGcc4OrLaterCompatible
			#define lemonrock_compatibility_sync_fetch_and_add_unsigned_short(pointer, increment) __sync_fetch_and_add(pointer, increment)
			#define lemonrock_compatibility_sync_increment_and_fetch_unsigned_short(pointer) __sync_add_and_fetch(pointer, 1)
		#elif defined(IsCompilerMsvc1700OrLaterCompatible)
			#include <intrin.h>
			#define lemonrock_compatibility_sync_fetch_and_add_unsigned_short(pointer, increment) _InterlockedExchangeAdd16(pointer, increment)
			#define lemonrock_compatibility_sync_increment_and_fetch_unsigned_short(pointer) _InterlockedIncrement16(pointer)
		#endif
	
	
	#ifdef __cplusplus
		}
	#endif

#endif

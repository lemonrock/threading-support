// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.


#include "compatibility.h"

#ifndef LEMONROCK_C_MACROS_H
	
	#define LEMONROCK_C_MACROS_H
	
	#ifdef __cplusplus
		extern "C" {
	#endif
	
	
	
	#ifndef likely
		#define likely(x) __builtin_expect(!!(x), 1)
	#endif

	#ifndef unlikely
		#define unlikely(x) __builtin_expect(!!(x), 0)
	#endif
	
	
	
	#ifdef __cplusplus
		}
	#endif
	
#endif

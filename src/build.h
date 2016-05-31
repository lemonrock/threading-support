// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.


#include "c/compatibility.h"
#include "c/macros.h"
#include "c/functions.h"


#ifndef LEMONROCK_THREADLOCAL_BUILD_H
	
	#define LEMONROCK_THREADLOCAL_BUILD_H
	
	C_START
	
	
			Hot extern inline unsigned short lemonrock_threading_support_currentMaximumThreadsWithRelativeThreadIds();

			Hot extern inline unsigned short lemonrock_threading_support_getRelativeThreadId();

			Cold extern unsigned short lemonrock_threading_support_setRelativeThreadId();
	
		
	C_END

#endif

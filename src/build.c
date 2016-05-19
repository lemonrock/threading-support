// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.


#include "build.h"
			
static unsigned short lemonrock_threading_support_relativeThreadIdCounter = 0;

Hot inline extern unsigned short lemonrock_threading_support_currentMaximumThreadsWithRelativeThreadIds()
{
	return lemonrock_threading_support_relativeThreadIdCounter + 1;
}

Hot inline static unsigned short lemonrock_threading_support_nextRelativeThreadId()
{
	// value '0' is reserved for the initial thread, hence we add before fetching
	return lemonrock_compatibility_sync_increment_and_fetch_unsigned_short(&lemonrock_threading_support_relativeThreadIdCounter);
}

// Mac OS X does not support thread local storage in Mach-O libs
#ifdef IsMacOsX

	#include <pthread.h>

	static pthread_key_t lemonrock_threading_support_relativeThreadIdKey;
	static pthread_once_t lemonrock_threading_support_relativeThreadIdKeyInitializeOnce = PTHREAD_ONCE_INIT;

	Cold NoInline static void lemonrock_threading_support_destroyRelativeThreadId(unsigned short * relative_thread_id)
	{
		free(relative_thread_id);
	}

	Cold static void lemonrock_threading_support_createRelativeThreadIdKey()
	{
		const int error = pthread_key_create(&lemonrock_threading_support_relativeThreadIdKey, (void *) lemonrock_threading_support_destroyRelativeThreadId);
		if (unlikely(error != 0))
		{
			lemonrock_c_exitWithError(error, "pthread_key_create failed in lemonrock_threading_support_createRelativeThreadIdKey");
		}
	}

	Hot inline extern unsigned short lemonrock_threading_support_getRelativeThreadId()
	{
		return * (unsigned short *) pthread_getspecific(lemonrock_threading_support_relativeThreadIdKey);
	}

	Cold extern unsigned short lemonrock_threading_support_setRelativeThreadId()
	{
	    int error = pthread_once(&lemonrock_threading_support_relativeThreadIdKeyInitializeOnce, lemonrock_threading_support_createRelativeThreadIdKey);
		if (unlikely(error != 0))
		{
			lemonrock_c_exitWithError(error, "pthread_once failed in lemonrock_threading_support_setRelativeThreadId");
		}

		unsigned short * relative_thread_id = (unsigned short *) malloc(sizeof(unsigned short));
		* relative_thread_id = lemonrock_threading_support_nextRelativeThreadId();

		error = pthread_setspecific(lemonrock_threading_support_relativeThreadIdKey, (void *) relative_thread_id);
		if (unlikely(error != 0))
		{
			lemonrock_c_exitWithError(error, "pthread_setspecific failed in lemonrock_threading_support_setRelativeThreadId");
		}

		return * relative_thread_id;
	}

#else

	ThreadLocal unsigned short lemonrock_threading_supportRelativeThreadId;

	Hot inline extern unsigned short lemonrock_threading_support_getRelativeThreadId()
	{
		return lemonrock_threading_supportRelativeThreadId;
	}

	extern unsigned short lemonrock_threading_support_setRelativeThreadId()
	{
		if (unlikely(lemonrock_threading_supportRelativeThreadId == 0))
		{
			lemonrock_c_exitWithError(ENOTSUP, "Relative thread id already set in lemonrock_threading_support_setRelativeThreadId");
		}
		return lemonrock_threading_supportRelativeThreadId = lemonrock_threading_support_nextRelativeThreadId();
	}

#endif

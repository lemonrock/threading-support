// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.

#include <stdio.h>
#include <errno.h>

// https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	#define thread_local __thread
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define thread_local __declspec(thread)
#endif

static unsigned short __relative_thread_id_counter = 0;

extern thread_local unsigned short __relative_thread_id;

extern inline unsigned short current_maximum_threads_with_relative_thread_ids()
{
	return __relative_thread_id_counter + 1;
}

extern inline unsigned short get_relative_thread_id()
{
	return __relative_thread_id;
}

extern unsigned short set_relative_thread_id()
{
	if (__relative_thread_id != 0)
	{
		errno = ENOTSUP;
		perror("Relative thread id already set");
	}
	// value '0' is reserved for the initial thread, hence we add before fetching
	return __relative_thread_id = __sync_add_and_fetch(&__relative_thread_id_counter, 1);
}

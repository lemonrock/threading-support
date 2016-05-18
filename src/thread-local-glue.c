// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

// http://lifecs.likai.org/2010/05/mac-os-x-thread-local-storage.html

// https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32

static unsigned short __relative_thread_id_counter = 0;

__attribute__ ((hot)) extern inline unsigned short current_maximum_threads_with_relative_thread_ids()
{
	return __relative_thread_id_counter + 1;
}

__attribute__ ((hot)) static inline unsigned short next_relative_thread_id()
{
	// value '0' is reserved for the initial thread, hence we add before fetching
	return __sync_add_and_fetch(&__relative_thread_id_counter, 1);
}

__attribute__ ((noreturn, cold)) void exit_with_error(const int error_number, const char * message)
{
	errno = error_number;
	perror(message);
	exit(EXIT_FAILURE);
}

extern inline unsigned short get_relative_thread_id();

extern unsigned short set_relative_thread_id();

// Mac OS X does not support thread local storage
#if defined(__APPLE__) && defined(__MACH__)
	
	#include <pthread.h>

	static pthread_key_t __relative_thread_id_key;
	static pthread_once_t __relative_thread_id_key_initialise_once = PTHREAD_ONCE_INIT;
	
	__attribute__ ((cold)) void __destroy_relative_thread_id(unsigned short * relative_thread_id)
	{
		free(relative_thread_id);
	}
	
	__attribute__ ((cold)) void __create_relative_thread_id_key()
	{
		const int error = pthread_key_create(&__relative_thread_id_key, (void *) __destroy_relative_thread_id);
		if (__builtin_expect(error, 0))
		{
			exit_with_error(error, "pthread_key_create failed in __create_relative_thread_id_key");
		}
	}
	
	 __attribute__ ((hot)) extern inline unsigned short get_relative_thread_id()
	{
		return * (unsigned short *) pthread_getspecific(__relative_thread_id_key);
	}

	__attribute__ ((cold)) extern unsigned short set_relative_thread_id()
	{
	    int error = pthread_once(&__relative_thread_id_key_initialise_once, __create_relative_thread_id_key);
		if (__builtin_expect(error, 0))
		{
			exit_with_error(error, "pthread_once failed in set_relative_thread_id");
		}
		
		unsigned short * relative_thread_id = (unsigned short *) malloc(sizeof(unsigned short));
		* relative_thread_id = next_relative_thread_id();
		
		error = pthread_setspecific(__relative_thread_id_key, (void *) relative_thread_id);
		if (__builtin_expect(error, 0))
		{
			exit_with_error(error, "pthread_setspecific failed in set_relative_thread_id");
		}
		return * relative_thread_id;
	}

#else
	
	#if !defined(_WIN32) && (defined(__unix__) || defined(__unix)
		#define thread_local __thread
	#endif

	#if defined(_WIN32) || defined(_WIN64)
		#define thread_local __declspec(thread)
	#endif

	thread_local unsigned short __relative_thread_id;

	__attribute__ ((hot)) extern inline unsigned short get_relative_thread_id()
	{
		return __relative_thread_id;
	}

	extern unsigned short set_relative_thread_id()
	{
		if (__builtin_expect(__relative_thread_id, 0))
		{
			exit_with_error(ENOTSUP, "Relative thread id already set in set_relative_thread_id");
		}
		return __relative_thread_id = next_relative_thread_id();
	}

#endif



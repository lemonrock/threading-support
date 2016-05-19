// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "compatibility.h"


#ifndef LEMONROCK_C_FUNCTIONS_H
	
	#define LEMONROCK_C_FUNCTIONS_H
	
	#ifdef __cplusplus
		extern "C" {
	#endif
			
			
			
			NoReturn Cold NoInline void lemonrock_c_exitWithError(const int error_number, const char * message)
			{
				errno = error_number;
				perror(message);
				exit(EXIT_FAILURE);
				__builtin_unreachable();
			}
			
			
			
	#ifdef __cplusplus
		}
	#endif
	
#endif


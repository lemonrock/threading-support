// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.


extern crate libc;
use libc::c_ushort;
#[cfg(windows)] extern crate kernel32;
use std::thread;
use std::thread::JoinHandle;
use std::thread::Builder;
use std::time::Duration;

pub const UsefulStackSize: usize = 65536;

pub fn sleepForSeconds(seconds: u64)
{
	let duration = Duration::new(seconds, 0);
	thread::sleep(duration);
}

pub fn sleepForMilliseconds(milliseconds: u64)
{
	let seconds = milliseconds / 1000;
	let nanoseconds = (milliseconds % 1000) * 1_000_000;
	let duration = Duration::new(seconds, nanoseconds as u32);
	thread::sleep(duration);
}

pub fn sleepForNanoseconds(nanoseconds: u32)
{
	let duration = Duration::new(0, nanoseconds);
	thread::sleep(duration);
}

pub fn spawn_thread<F, T>(name: String, stackSize: usize, f: F) -> JoinHandle<T>
	where F: FnOnce(u16) -> T, F: Send + 'static, T: Send + 'static
{
	return Builder::new()
		.name(name)
		.stack_size(stackSize)
		.spawn(move ||
		{
			let relative_thread_id = unsafe { lemonrock_threading_support_setRelativeThreadId() } as u16;
			f(relative_thread_id)
		})
		.unwrap();
}

extern
{
	#[inline(always)]
	pub fn lemonrock_threading_support_currentMaximumThreadsWithRelativeThreadIds() -> c_ushort;
	
	#[inline(always)]
	pub fn lemonrock_threading_support_getRelativeThreadId() -> c_ushort;
	
	pub fn lemonrock_threading_support_setRelativeThreadId() -> c_ushort;
}

// /// pthread_t is unsigned on all platforms apart from Android... go figure.
// /// On musl, pthread_t contains a reference to tid; if 0, refers to current thread (pthread_t->tid)
// #[cfg(all(unix, not(target_os = "android")))]
// #[inline(always)]
// pub fn current_thread_id() -> usize
// {
// 	unsafe { libc::pthread_self() as usize }
// }
//
// // On Android, we really should use the public pthread_gettid_np() function...
// #[cfg(target_os = "android")]
// #[inline(always)]
// pub fn current_thread_id() -> usize
// {
// 	let thread_signed_id = unsafe { libc::pthread_self() };
// 	if thread_signed_id >= 0
// 	{
// 		thread_signed_id as usize + 2_147_483_648usize
// 	}
// 	(thread_signed_id + 2_147_483_648) as usize
// }
//
// #[cfg(windows)]
// #[inline(always)]
// pub fn current_thread_id() -> usize
// {
// 	unsafe { kernel32::GetCurrentThreadId() as usize }
// }

#[test]
fn test()
{
	spawn_thread(String::from("Example"), UsefulStackSize, move |relative_thread_id|
	{
		println!("{}", relative_thread_id);
	}).join().unwrap();
}

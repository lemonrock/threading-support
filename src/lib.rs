// This file is part of threading-support. It is subject to the license terms in the COPYRIGHT file found in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT. No part of threading-support, including this file, may be copied, modified, propagated, or distributed except according to the terms contained in the COPYRIGHT file.
// Copyright © 2016 The developers of threading-support. See the COPYRIGHT file in the top-level directory of this distribution and at https://raw.githubusercontent.com/lemonrock/threading-support/master/COPYRIGHT.

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

pub fn spawnThread<F, T>(name: String, stackSize: usize, f: F) -> JoinHandle<T>
	where F: FnOnce() -> T, F: Send + 'static, T: Send + 'static,
{
	return Builder::new()
		.name(name)
		.stack_size(stackSize)
		.spawn(f)
		.unwrap();
}

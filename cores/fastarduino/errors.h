//   Copyright 2016-2019 Jean-Francois Poilpret
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

/// @cond api

/**
 * @file 
 * Common errors definition.
 */
#ifndef ERRORS_HH
#define ERRORS_HH

/**
 * This namespace defines common errors that can be returned by some FastArduino
 * API methods, e.g. devices::rf::NRF24L01.
 * New errors will be added here as required.
 * These definitions are similar to standard C `errno.h`; however, please note
 * that FastArduino does not define a global `errno`, as in standard C, to
 * store the last occurred error. The error constants defined here are directly 
 * returned, as `int`, by some methods in FastArduino API.
 * Also, only error codes potentially returned by FastArduino API are defined here.
 */
namespace errors
{
	/** Input/output error. */
	constexpr const int EIO = -5;
	/** Invalid argument. */
	constexpr const int EINVAL = -22;
	/** Timer expired. */
	constexpr const int ETIME = -62;
	/** Message too long.  */
	constexpr const int EMSGSIZE = -90;
}

#endif /* ERRORS_HH */
/// @endcond

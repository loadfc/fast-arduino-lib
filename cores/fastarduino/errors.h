//   Copyright 2016-2018 Jean-Francois Poilpret
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

#ifndef ERRORS_HH
#define ERRORS_HH

// List of errors returned by some FastArduino methods
namespace errors
{
	constexpr const int EIO = -5;
	constexpr const int EINVAL = -22;
	constexpr const int ETIME = -62;
	constexpr const int EMSGSIZE = -90;
}

#endif /* ERRORS_HH */

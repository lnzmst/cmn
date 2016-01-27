/**
 *   @authors   Lorenzo Musto
 *
 *   @copyright Copyright 2016 Lorenzo Musto
 *
 *   @section   LICENSE
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
#ifndef LIB_TW_I
#define LIB_TW_I

bool Timer::busy()
{
  return _trigger != 0;
}

#endif // #ifndef LIB_TW_I

/**
 *   Copyright 2016 Lorenzo Musto
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
#include "lib_tw.h"

iTrigger::~iTrigger()
{}

TimingWheel::TimingWheel( int N, int n )
  : _N( N )
  , _n( n )
  , _trigger( 0 )
  , _interval( 0 )
  , _left( 0 )
  , _at( -1 )
{}

void TimingWheel::pulse()
{
  _n = (_n + 1)%_N;

  if (_n == _at) {
    if (_left > _N) {
      _left -= _N;
    } else {
      _trigger->timeout();
      _at = -1;
    }
  }
}

int TimingWheel::create( iTrigger *trigger )
{
  _trigger = trigger;
  return 0;
}

void TimingWheel::arm( int interval )
{
  _interval = interval;
  _left = interval;
  _at = (_n + interval)%_N;
}

void TimingWheel::cancel()
{
  _at = -1;
}

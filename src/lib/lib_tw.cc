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
#include "lib_tw.h"

iTrigger::~iTrigger()
{}


Timer::Timer()
  : _trigger( 0 )
  , _interval( 0 )
  , _left( 0 )
  , _at( -1 )
{}

void Timer::create( iTrigger *trigger )
{
  _trigger = trigger;
}

void Timer::arm( int at, int interval )
{
  _interval = interval;
  _left = interval;
  _at = at;
}

void Timer::cancel()
{
  _at = -1;
}

void Timer::destroy()
{
  _at = -1;
  _trigger = 0;
}

void Timer::pulse( int t, int T )
{
  if (t != _at) return;

  if (_left > T) {
    _left -= T;
  } else {
    _trigger->timeout();
    _at = -1;
  }
}

TimingWheel::TimingWheel( int N, int T, int t_0 )
  : _N( N )
  , _T( T )
  , _t( t_0 )
  , _timer( N )
{}

void TimingWheel::pulse()
{
  _t = (_t + 1)%_T;

  for (int timerid=0; timerid<_N; ++timerid)
    if (_timer[timerid].busy())
      _timer[timerid].pulse( _t, _T );
}

int TimingWheel::create( iTrigger *trigger )
{
  for (int timerid=0; timerid<_N; ++timerid)
    if (not _timer[timerid].busy()) {
      _timer[timerid].create( trigger );
      return timerid;
    }
  return -1;
}

void TimingWheel::arm( int timerid, int interval )
{
  _timer[timerid].arm( (_t + interval)%_T, interval );
}

void TimingWheel::cancel( int timerid )
{
  _timer[timerid].cancel();
}

void TimingWheel::destroy( int timerid )
{
  _timer[timerid].destroy();
}

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
#ifndef LIB_TW_H
#define LIB_TW_H

#include <vector>

class iTrigger {
public:
  virtual ~iTrigger();
  virtual void timeout() = 0;
};

class Timer {
public:
  Timer();

  iTrigger *_trigger;
  int _interval;
  int _left;
  int _at;

  void create( iTrigger *trigger );
  void arm( int at, int interval );
  void cancel();
  void destroy();

  inline void pulse( int n, int N );

  inline bool busy();
};

class TimingWheel {
private:
  int _N;
  int _T;
  int _t;

  std::vector<Timer> _timer;

public:
  /** Constructor to define geometry.
   *
   *  @param[in] N  Maximum number of supported Timer instances.
   *  @param[in] T  Extension, in pulses, of the active time window.
   *  @param[in] t_0 Initial time within the active time window.
   */
  TimingWheel( int N, int T, int t_0 );

  void pulse();

  int create( iTrigger *trigger );
  void arm( int timerid, int interval );
  void cancel( int timerid );
  void destroy( int timerid );
};

#include "lib_tw.i"

#endif // #ifndef LIB_TW_H

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

class iTrigger {
public:
  virtual ~iTrigger();
  virtual void timeout() = 0;
};

class TimingWheel {
private:
  int _N;
  int _n;

  iTrigger *_trigger;
  int _interval;
  int _left;
  int _at;

public:
  TimingWheel( int N, int n = 0 );

  void pulse();

  int create( iTrigger *trigger );
  void arm( int pulses );
  void cancel();
};

#endif // #ifndef LIB_TW_H

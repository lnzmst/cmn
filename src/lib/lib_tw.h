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
#ifndef LIB_TW_H
#define LIB_TW_H

#include <vector>

/**
 * Trigger interface to actuate the timeout operation.
 */
class iTrigger {
public:
  virtual ~iTrigger();
  /**
   * The method triggered at timeout.
   */
  virtual void timeout() = 0;
};

/**
 * A class for Timer state and behaviour.
 */
class Timer {
private:
  iTrigger *_trigger; ///< A reference to Timer's iTrigger. 0 value if Timer is not in valid state.
  int _interval; ///< The scheduling interval before fire the trigger.
  int _left; ///< Number of left pulses since the arm() event before firing the iTrigger.
  int _at; ///< TimingWheel's slot at which the Timer has been installed.

public:
  /**
   * Constructor.
   */
  Timer();

  /**
   * Create the Timer.
   *
   *  @param[in] trigger  The trigger implementing the timeout callback.
   */
  void create( iTrigger *trigger );

  /**
   * Arm the Timer.
   *
   *  @param[in] at       The TimingWheel's pulse the Timer is activated at.
   *  @param[in] interval The number of pulses after which the Timer will be
   *                      fired invoking iTrigger::timeout() method.
   */
  void arm( int at, int interval );

  /**
   * Cancel a scheduled Timer.
   */
  void cancel();

  /**
   * Destroy the Timer.
   */
  void destroy();

  /**
   * Signal the Timer to progress over the next pulse.
   */
  inline void pulse( int n, int N );

  /**
   * Query if the Timer is valid that is created.
   */
  inline bool exists();
};

/**
 * Circular Timing Wheel to schedule Timer instances.
 *
 * The TimingWheel is continuously pulsed to advance current
 * time and in turn it distributes the pulse to any active Timer.
 */
class TimingWheel {
private:
  int _N;
  int _T;
  int _t;

  std::vector<Timer> _timer;

public:
  /**
   * Constructor to define geometry.
   *
   * @param[in] N   Maximum number of supported Timer instances.
   * @param[in] T   Extension, in pulses, of the active time window.
   * @param[in] t_0 Initial time within the active time window.
   */
  TimingWheel( int N, int T, int t_0 );

  /**
   * Signal to progress one more pulse ahead and serve all active Timer instances.
   */
  void pulse();

  /**
   * Create a new Timer and link it with the given Trigger.
   *
   * @param[in] trigger An iTrigger.
   *
   * @return
   * A valid Timer identifier or -1 if no more Timer instances can be allocated.
   */
  int create( iTrigger *trigger );

  /**
   * Program a Timer to schedule its iTrigger.
   *
   * @param[in] timerid A Timer identifier as returned by create().
   * @param[in] interval The number of pulse before calling the iTrigger::timeout() method.
   */
  void arm( int timerid, int interval );

  /**
   * Cancel an armed Timer.
   *
   * @param[in] timerid A Timer identifier as returned by create().
   */
  void cancel( int timerid );

  /**
   * Destroy a Timer.
   *
   * @param[in] timerid A Timer identifier as returned by create().
   */
  void destroy( int timerid );
};

#include "lib_tw.i"

#endif // #ifndef LIB_TW_H

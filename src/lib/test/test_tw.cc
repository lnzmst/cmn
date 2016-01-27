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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "lib_tw.h"

class TimingWheelTest: public ::testing::Test {
};

class TriggerMock: public iTrigger {
public:
  MOCK_METHOD0( timeout, void () );
};

TEST_F( TimingWheelTest, an_armed_timer_should_timeout_at_programmed_time )
{
  static int const timers = 1;
  static int const N = 16;

  for (int n=0; n<N; ++n) {

    TimingWheel tm( timers, N, n );

    for (int m=1; m<=N; ++m) {
      TriggerMock *trigger = new TriggerMock;

      int timerid = tm.create( trigger );
      ASSERT_NE( timerid, -1 );

      tm.arm( timerid, m );

#if 0
      // Class MockFunction<F> has exactly one mock method.
      // It is named Call() and has type F.
      ::testing::MockFunction<void(std::string check_point_name)> check;

      {
        ::testing::InSequence s;

        EXPECT_CALL(check, Call("checkpoint_1"));
        EXPECT_CALL(check, Call("checkpoint_2"));
        EXPECT_CALL(*trigger, timeout())
          .Times(1);
      }

      check.Call("checkpoint_1");
      for (int t=0; t<m; ++t)
        tm.pulse();
      check.Call("checkpoint_2");
      tm.pulse();
#else
      EXPECT_CALL( *trigger, timeout() ).Times(0);

      for (int t=0; t<m-1; ++t)
        tm.pulse();

      ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );

      EXPECT_CALL( *trigger, timeout() ).Times(1);

      tm.pulse();

      ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );

      for (int t=0; t<3*N; ++t)
        tm.pulse();

      EXPECT_CALL( *trigger, timeout() ).Times(0);
      ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );
#endif

      tm.destroy( timerid );

      delete trigger;
    }
  }
}

TEST_F( TimingWheelTest, an_armed_timer_can_be_cancelled )
{
  static int const timers = 1;
  static int const N = 16;

  for (int n=0; n<N; ++n) {
    TimingWheel tm( timers, N, n );

    for (int m=3; m<N; ++m) {
      TriggerMock *trigger = new TriggerMock;

      int timerid = tm.create( trigger );
      ASSERT_NE( timerid, -1 );
      tm.arm( timerid, m );

      EXPECT_CALL( *trigger, timeout() ).Times(0);

      for (int t=0; t<m-1; ++t) {
        if (t == 1) tm.cancel( timerid );
        tm.pulse();
      }

      for (int t=0; t<3*N; ++t)
        tm.pulse();

      tm.destroy( timerid );
      delete trigger;
    }
  }
}

TEST_F( TimingWheelTest, an_armed_long_timer_should_timeout_at_programmed_time )
{
  static int const timers = 1;
  static int const N = 16;

  for (int o=1; o<4; ++o) {

    for (int n=0; n<N; ++n) {

      TimingWheel tm( timers, N, n );

      for (int m=1; m<=N; ++m) {
        TriggerMock *trigger = new TriggerMock;

        int timerid = tm.create( trigger );
        ASSERT_NE( timerid, -1 );
        tm.arm( timerid, o*N+m );

        EXPECT_CALL( *trigger, timeout() ).Times(0);

        for (int t=0; t<o*N+m-1; ++t)
          tm.pulse();

        ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );


        EXPECT_CALL( *trigger, timeout() ).Times(1);

        tm.pulse();

        ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );


        EXPECT_CALL( *trigger, timeout() ).Times(0);

        for (int t=0; t<3*N; ++t)
          tm.pulse();

        ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );

        tm.cancel( timerid );

        tm.destroy( timerid );

        delete trigger;
      }
    }
  }
}

TEST_F( TimingWheelTest, a_timer_can_be_rearmed )
{
  static int const timers = 1;
  static int const N = 16;

  for (int o=1; o<4; ++o) {

    for (int n=0; n<N; ++n) {

      TimingWheel tm( timers, N, n );

      for (int m=1; m<=N; ++m) {
        TriggerMock *trigger = new TriggerMock;

        int timerid = tm.create( trigger );
        ASSERT_NE( timerid, -1 );
        tm.arm( timerid, o*N+m );

        EXPECT_CALL( *trigger, timeout() ).Times(0);

        for (int t=0; t<o*N+m-1; ++t)
          tm.pulse();

        ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );

        tm.arm( timerid, 30 );

        EXPECT_CALL( *trigger, timeout() ).Times(0);

        for (int t=0; t<30-1; ++t)
          tm.pulse();

        ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );


        EXPECT_CALL( *trigger, timeout() ).Times(1);

        tm.pulse();

        ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );


        EXPECT_CALL( *trigger, timeout() ).Times(0);

        for (int t=0; t<3*N; ++t)
          tm.pulse();

        ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger ) );

        tm.cancel( timerid );

        tm.destroy( timerid );

        delete trigger;
      }
    }
  }
}

TEST_F( TimingWheelTest, works_with_multiple_timer_instances )
{
  static int const max_timers = 32;
  static int const T = 64;
  static int const t_0 = 0;
  static int const run_timers = 2;
  static int const timeout0 = 10;
  static int const timeout1 = 30;

  TimingWheel tm( max_timers, T, t_0 );

  TriggerMock *trigger[run_timers];
  int timerid[run_timers];
  for (int k=0; k<run_timers; ++k) {
    trigger[k] = new TriggerMock;
    timerid[k] = tm.create( trigger[k] );
    ASSERT_NE( timerid[k], -1 );
  }
  tm.arm( timerid[0], timeout0 );
  tm.arm( timerid[1], timeout1 );

  EXPECT_CALL( *trigger[0], timeout() ).Times(0);
  EXPECT_CALL( *trigger[1], timeout() ).Times(0);

  for (int t=0; t<timeout0-1; ++t)
    tm.pulse();

  for (int k=0; k<run_timers; ++k)
    ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger[k] ) );


  EXPECT_CALL( *trigger[0], timeout() ).Times(1);
  EXPECT_CALL( *trigger[1], timeout() ).Times(0);

  tm.pulse();

  for (int k=0; k<run_timers; ++k)
    ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( trigger[k] ) );


  for (int k=0; k<run_timers; ++k) {
    tm.destroy( timerid[k] );
    delete trigger[k];
  }
}

#include <tests.h>
#include <lld_odometry.h>
#include <drive_cs.h>

#include <ros_protos.h>

controlValue_t          test_ros_steer_cntr = 0;
float                   test_ros_speed_cntr = 0;

virtual_timer_t         ros_checker_vt;


static void ros_is_dead_cb( void *arg )
{
    arg = arg; 
    test_ros_speed_cntr = 0;
    test_ros_steer_cntr = 0;
    dbgprintf( "ROS is dead\n\r" );
}

void ros_alive( void )
{
    palToggleLine( LINE_LED1 ); // just to check
    chVTSet( &ros_checker_vt, MS2ST( 500 ), ros_is_dead_cb, NULL );
}

void cntrl_handler (float speed, float steer)
{
    systime_t ros_time = chVTGetSystemTimeX();
    test_ros_speed_cntr = speed;
    test_ros_steer_cntr = steer;
    dbgprintf( "Time:%d\n\r", (int)(ros_time * 1000.0 / CH_CFG_ST_FREQUENCY) );
    ros_alive( );
}

/*
 * @brief   Test odometry, speed and steering control via ROS
 * @note    Frequency = 50 Hz
*/
void testRosRoutineControl( void )
{
    ros_driver_cb_ctx_t cb_ctx      = ros_driver_get_new_cb_ctx();
    cb_ctx.cmd_cb                   = cntrl_handler;
    // cb_ctx.set_steer_params_cb      = changeSteerParams;
    cb_ctx.reset_odometry_cb        = lldResetOdometry;
    // cb_ctx.get_control_params       = get_esc_control_params;

    ros_driver_init( NORMALPRIO, &cb_ctx );

//    ros_driver_set_control_cb( cntrl_handler );

    lldOdometryInit( );
    driverCSInit( NORMALPRIO );
    debug_stream_init( );

    chVTObjectInit(&ros_checker_vt);

    odometryValue_t         test_x_pos          = 0;
    odometryValue_t         test_y_pos          = 0;
    odometryValue_t         test_tetta_deg      = 0;

    odometryRawSpeedValue_t test_enc_speed_rps  = 0;
    odometrySpeedValue_t    test_speed_radps    = 0;

    float                   test_speed_lpf_mps  = 0;
    uint32_t                print_cntr          = 0;

    systime_t time = chVTGetSystemTimeX();

    while( 1 )
    {
        print_cntr += 1;

        driveSteerCSSetPosition( test_ros_steer_cntr );
        driveSpeedCSSetSpeed( test_ros_speed_cntr );

        test_enc_speed_rps  = lldGetOdometryRawSpeedRPS( );
        test_speed_radps    = lldGetOdometryObjTettaSpeedRadPS( );

        test_speed_lpf_mps  = lldOdometryGetLPFObjSpeedMPS( );

        test_x_pos          = lldGetOdometryObjX( OBJ_DIST_M );
        test_y_pos          = lldGetOdometryObjY( OBJ_DIST_M );
        test_tetta_deg      = lldGetOdometryObjTettaDeg( );

        ros_driver_send_encoder_speed( test_enc_speed_rps );
        ros_driver_send_pose( test_x_pos, test_y_pos, test_tetta_deg, test_speed_lpf_mps, test_speed_radps );

        time = chThdSleepUntilWindowed( time, time + MS2ST( 20 ) );

    }
}

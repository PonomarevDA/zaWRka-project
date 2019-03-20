#include <tests.h>
#include <lld_light.h>
#include <lld_start_button.h>

#include <max7219.h>

#define             RIGHT_TURN_LINE         PAL_LINE( GPIOG, 2 )
#define             LEFT_TURN_LINE          PAL_LINE( GPIOG, 3 )

bool                turn_right_flag = 0;
bool                turn_left_flag  = 0;
turn_light_state    turn_state = STOP; 

/*
 * @brief   Automatically set the state of turn lights
 *          depends on input conditions 
 */
void lldLightDetectTurnState( float steer_cntrl, float speed_cntrl, system_state s_state )
{
    if( steer_cntrl > 0 ) turn_state = LEFT; 
    else if( steer_cntrl < 0 ) turn_state = RIGHT;
    else if ( steer_cntrl == 0 && speed_cntrl == 0 && s_state == RUN )
    {
        turn_state = STOP; 
    }
    else if( s_state == IDLE )
    {
        turn_state = REMOTE;
    }
}

/*
 * @brief   Get current state of turn lights  
 */
turn_light_state lldGetLightState( void )
{
    return turn_state; 
}

static THD_WORKING_AREA(waTurnRoutine, 128); // 128 - stack size
static THD_FUNCTION(TurnRoutine, arg)
{
    arg = arg; 

    if( turn_state == RIGHT )
    {
        palToggleLine( RIGHT_TURN_LINE );
        palClearLine( LEFT_TURN_LINE );
    }
    else if( turn_state == LEFT )
    {
        palToggleLine( LEFT_TURN_LINE );
        palClearLine( RIGHT_TURN_LINE );
    }
    else if( turn_state == STOP )
    {
        palToggleLine( LEFT_TURN_LINE );
        palToggleLine( RIGHT_TURN_LINE );
    }
    else if( turn_state == REMOTE )
    {
        palClearLine( LEFT_TURN_LINE );
        palClearLine( RIGHT_TURN_LINE );
    }

    chThdSleepMilliseconds( 300 );    

}

#define SIGN_NUMBER     1
#define MAX_DIGITS      8
 

/***    LED Matrix Related   ***/
// static uint8_t signs[SIGN_NUMBER][MAX_DIGITS] = {

// };

static uint8_t brick_sign[8] = {
    0x3C, 0x42, 0x81, 0xBD, 0xBD, 0x81, 0x3C
}; 



#define SPI_MOSI_LINE   PAL_LINE( GPIOC, 3 )
#define SPI_SCLK_LINE   PAL_LINE( GPIOB, 10 )
// #define SPI_MISO_LINE   PAL_LINE( GPIOC, 2 )
#define SPI_CS_LINE     PAL_LINE( GPIOF, 2 )
#define SPI_CS_PORT     GPIOF 
#define SPI_CS_PAD      2


/*
 * Maximum speed SPI configuration (27MHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig led_spicfg = {
    /**
     * @brief Operation complete callback or @p NULL.
     */
    .end_cb = NULL,
    /**
     * @brief The chip select line port.
     */
    .ssport = SPI_CS_PORT,
    /**
     * @brief The chip select line pad number.
     */
    .sspad = SPI_CS_PAD,
    /**
     * @brief SPI CR1 register initialization data.
     */
    .cr1 = SPI_CR1_BR, //SPI_CR1_BR_0,
    /**
     * @brief SPI CR2 register initialization data.
     */
    .cr2 = 0 //SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

#define SPI_BUFFERS_SIZE    128U
static uint8_t txbuf[SPI_BUFFERS_SIZE];
static uint8_t test_tx_spi = 0xFF; 


static THD_WORKING_AREA(waLedMatrix, 256); // 128 - stack size
static THD_FUNCTION(LedMatrix, arg)
{
    arg = arg;  // to avoid warnings
    uint8_t i = 0; 
    while( 1 )
    {
        palClearLine( SPI_CS_LINE );
        spiStartSendI( &SPID2, 2, &test_tx_spi );
        // max7219WriteRegister( &SPID2, MAX7219_AD_DISPLAY_TEST, 0xF0 );

        // for(i = 0; i < MAX_DIGITS; i++)
        // {
        //     palSetLine( LINE_LED1 );

        //     max7219WriteRegister( &SPID2, MAX7219_AD_DIGIT_0 + (i << 8), 
        //         brick_sign[i] );
        // }
        chThdSleepMilliseconds( 50 );    
    }

    
}



static bool             isInitialized = false;

/**
 * @brief   Initialize periphery connected LEDs
 * @note    Stable for repeated calls
 */
void lldLightInit( tprio_t priority )
{
    if( isInitialized )
          return;

    palSetLineMode( RIGHT_TURN_LINE, PAL_MODE_OUTPUT_PUSHPULL );
    palSetLineMode( LEFT_TURN_LINE,  PAL_MODE_OUTPUT_PUSHPULL );

    /***    LED Matrix   ***/
    palSetLineMode( SPI_SCLK_LINE, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST );
    palSetLineMode( SPI_MOSI_LINE, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST );
    // palSetLineMode( SPI_MISO_LINE, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST );

    palSetLineMode( SPI_CS_LINE, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST ); 
    palSetLine( LINE_LED2 );

    chThdCreateStatic( waLedMatrix, sizeof(waLedMatrix), priority, LedMatrix, NULL);
    spiStart( &SPID2, &led_spicfg ); 

    // MAX7219_OM_t spi_mode = MAX7219_OM_Normal;

    // max7219WriteRegister(&SPID2, MAX7219_AD_DISPLAY_TEST, FALSE);

    chThdCreateStatic( waTurnRoutine, sizeof(waTurnRoutine), priority, TurnRoutine, NULL);

    isInitialized = true;
}

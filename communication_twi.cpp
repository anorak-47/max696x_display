#include "communication_twi.h"
#include "command_queue.h"
#include "commands.h"
#include "global.h"
#include "led.h"
#include "max696x/max696x_conf.h"

#define AVR311

#ifdef AVR311
#include "twi_slave_avr311/TWI_slave.h"
#else
#include "twi/twi_slave.h"
#endif

#if defined(SB_ENABLE_DEBUG_BASE) && defined(SB_ENABLE_I2CCOMM_DEBUG)
#define DEBUG
#define DEBUG_UART
#endif

#include "debug.h"

#define TWI_CMD_REQUEST_INFO MATRIX_CMD_READ_CFG // 0xFD 253

//uint8_t const state_size = sizeof(struct _state_t);

// When there has been an error, this function is run and takes care of it
unsigned char TWI_Act_On_Failure_In_Last_Transmission(unsigned char TWIerrorMsg);
void handleReceivedData(uint8_t const *recieveData, uint8_t receiveDataLength);

void initCommunicationTwi(uint8_t deviceAddr)
{
    LV_("i2c slave init, addr: %u 0x%X", deviceAddr, deviceAddr);
    LV_("i2c slave tx size: %u", sizeof(struct _state_t));

    // Initialise TWI module for slave operation. Include address and/or enable General Call.
    // TWI_Slave_Initialise((unsigned char)((deviceAddr << TWI_ADR_BITS) | (TRUE << TWI_GEN_BIT)));
    TWI_Slave_Initialise((unsigned char)((deviceAddr << TWI_ADR_BITS)));

    //TWI_Set_Slave_Receive_Handler(&handleReceivedData);
    TWI_Set_Slave_Transmit_Buffer((unsigned char *)&(global.state), sizeof(struct _state_t));

    // Start the TWI transceiver to enable reseption of the first command from the TWI Master.
    TWI_Start_Transceiver();
}

void handleReceivedData(uint8_t const *recieveData, uint8_t receiveDataLength)
{
    if (receiveDataLength == 0)
        return;

    uint8_t cmd = recieveData[0];

    /*
    if (cmd == TWI_CMD_REQUEST_INFO)
    {
        uint8_t pos = 0;
        if (receiveDataLength > 1)
            pos = recieveData[1];

        if (state_size - pos > state_size)
        {
            //LV_("hrd: INFO pos out of range:%u", pos);
            pos = 0;
        }

        unsigned char *b = (unsigned char *)&(global.state);
        TWI_Prepare_Transceiver_Data(b + pos, state_size - pos);
        //TWI_Start_Transceiver_With_Data(b + pos, state_size - pos);

        //LV_("hrd: INFO pos:%u data:%u", pos, state_size - pos);
    }
    else
    */
    {
        //TWI_Start_Transceiver();

        //LV_("hrd: recv cmd:0x%X", cmd);
        //LV_("hrd: recv len:%u", receiveDataLength);
    	cmd_enqueue_data(cmd, recieveData + 1, receiveDataLength - 1);
        //bool enqueued = cmd_enqueue_data(cmd, recieveData + 1, receiveDataLength - 1);
        //LV_("hrd: enqueued: %u", enqueued);
    }
}

void twiMainLoop()
{
    //static unsigned char messageBuf[TWI_BUFFER_SIZE];

    // Check if the TWI Transceiver has completed an operation.
    if (!TWI_Transceiver_Busy())
    {
        // Check if the last operation was successful
        if (TWI_statusReg.lastTransOK)
        {
            LEDON(LED_ACTIVITY);

#if 0
            // Check if the last operation was a reception
            if (TWI_statusReg.RxDataInBuf)
            {
                LV_("I2C: recv len:%u", TWI_statusReg.RxDataBytes);
                TWI_Get_Data_From_Transceiver(messageBuf, TWI_statusReg.RxDataBytes);

                // Check if the last operation was a reception as General Call
                if (TWI_statusReg.genAddressCall)
                {
                    LS_("I2C: gen call");
                }
                else // Ends up here if the last operation was a reception as Slave Address Match
                {
                    // messageBuf[0] = 0xAA;
                    // TWI_Start_Transceiver_With_Data(messageBuf, 1);

                    handleReceivedData(messageBuf, TWI_statusReg.RxDataBytes);

                    /*
                    // Example of how to interpret a command and respond.
					// TWI_CMD_MASTER_WRITE stores the data to PORTB
					if (messageBuf[0] == TWI_CMD_MASTER_WRITE)
					{
						PORTB = messageBuf[1];
					}
					// TWI_CMD_MASTER_READ prepares the data from PINB in the transceiver buffer for the TWI master to fetch.
					if (messageBuf[0] == TWI_CMD_MASTER_READ)
					{
						messageBuf[0] = PINB;
						TWI_Start_Transceiver_With_Data(messageBuf, 1);
					}
					*/
                }
            }

#endif

            // Check if the TWI Transceiver has already been started.
            // If not then restart it to prepare it for new receptions.
            /*
            if (!TWI_Transceiver_Busy())
            {
                TWI_Start_Transceiver();
            }
            */

            LEDOFF(LED_ACTIVITY);
        }
        else // Ends up here if the last operation completed unsuccessfully
        {
            TWI_Act_On_Failure_In_Last_Transmission(TWI_Get_State_Info());
        }
    }
}

unsigned char TWI_Act_On_Failure_In_Last_Transmission(unsigned char TWIerrorMsg)
{
    // A failure has occurred, use TWIerrorMsg to determine the nature of the failure
    // and take appropriate actions.
    // See header file for a list of possible failures messages.

    if (TWIerrorMsg != TWI_NO_STATE)
        LV_("I2C error %u 0x%X", TWIerrorMsg, TWIerrorMsg);

    TWI_Start_Transceiver();

    return TWIerrorMsg;
}

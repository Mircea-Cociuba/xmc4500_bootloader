/*
 * main.c
 *
 *  Created on: 2019 Sep 17 11:02:00
 *  Author: MI382522
 */




#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)





#define APPL_ADDRESS_RESET_VECTOR 0x08010200
#define PATTERN_ADDRESS 0xC040000

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

void Can_Transmit(void);


const CAN_NODE_t *HandlePtr1 = &CAN_NODE_0;//prima adresa din CAN

int main(void)
{
	DAVE_STATUS_t status;
	uint32_t timeOffSet;
	uint32_t pattern=0;

	int (*pmain)(void);

	status = DAVE_Init();           /* Initialization of DAVE APPs  */

	pmain=(int (*)(void))APPL_ADDRESS_RESET_VECTOR;

	if(status != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while(1U)
		{

		}
	}

	SYSTIMER_Start();

	timeOffSet=SYSTIMER_GetTime(); //microseconds

	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	while(1U)
	{
		if((SYSTIMER_GetTime() - timeOffSet) > 5000) //5ms
		{
			timeOffSet = SYSTIMER_GetTime();

			pattern=*((uint32_t*)PATTERN_ADDRESS);

			if(pattern == 0xDDCCBBAAu )
			{
				PPB->VTOR = 0x0C010000; // Offset of the Vector

				asm("ldr r0, =0x0c010000");
				asm("ldr r1, =0xE000ED08");
				asm("str r0,[r1]");
				asm("ldr sp, [r0], #4");
				asm("ldr r15, [R0]");
			}
		}

	}
}
//todo: delete this thing
void Can_Transmit(void)//functie de transmitere pe can
{

	CAN_NODE_STATUS_t mo_tranmit_status;//statusu mesajelor transmise
	CAN_NODE_STATUS_t status_can;//statusu can
	XMC_CAN_MO_t *MO_Ptr;//pointer la msj

	uint8_t can_data_arr_tx[8]={2,0,0,0,0,0,0,0};

	MO_Ptr = HandlePtr1->lmobj_ptr[3]->mo_ptr;

	// update data for the MO to transmit

	(void)CAN_NODE_MO_UpdateData(HandlePtr1->lmobj_ptr[3],&can_data_arr_tx[0]);

	mo_tranmit_status = CAN_NODE_MO_Transmit(HandlePtr1->lmobj_ptr[3]);//Function to transmit the can MO frame

	if (mo_tranmit_status == CAN_NODE_STATUS_SUCCESS)

	{
		//message object transmission success.

		// read the TXOK status bit
		status_can = CAN_NODE_MO_GetStatus(HandlePtr1->lmobj_ptr[3]);

		if (status_can & XMC_CAN_MO_STATUS_TX_PENDING);
		{
			//Clear the transmit OK flag

			XMC_CAN_MO_ResetStatus(MO_Ptr,XMC_CAN_MO_RESET_STATUS_TX_PENDING);

		}

	}
	else
	{
		// message object failed to transmit.
	}

}

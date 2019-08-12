// xQueue Create/send/receive
// https://www.youtube.com/watch?v=DLqj01asDM0&t=246s

#include "tcp_comms.h"
#include "libs/OutputStream.h"
#include "FreeRTOS.h"
#include "freertos/task.h"


static xTaskHandle xTaskToNotify = NULL;

void set_notification_tcp(xTaskHandle h)
{
	/* Store the handle of the calling task. */
	xTaskToNotify = h;
}



size_t read_tcp(char * buf, size_t length)
{
	// int bytes = Chip_UART_ReadRB(LPC_UARTX, &rxring, buf, length);
	// return bytes;
	return 0;
}

size_t write_tcp(const char *buf, size_t length)
{
	// Note we do a blocking write here until all is written
	size_t sent_cnt = 0;
	while(sent_cnt < length) {
		// int n = Chip_UART_SendRB(LPC_UARTX, &txring, buf+sent_cnt, length-sent_cnt);
		int n = 1;
		if(n > 0) {
			sent_cnt += n;
		}
	}
	return length;
}

#include <vector>
#define MAX_LINE_LENGTH 20
static std::vector<OutputStream*> output_streams;    //TODO: unify output_streams

void tcp_comms(void *)
{
    //listen port 
    set_notification_tcp(xTaskGetCurrentTaskHandle());

//     // create an output stream that writes to the uart
    OutputStream os([](const char *buf, size_t len) { return write_tcp(buf, len); });
    output_streams.push_back(&os);

    const TickType_t waitms = pdMS_TO_TICKS( 300 );

    char rx_buf[256];
    char line[MAX_LINE_LENGTH];
    size_t cnt = 0;
    bool discard = false;
    while(1) {
        // Wait to be notified that there has been a UART irq. (it may have been rx or tx so may not be anything to read)
        uint32_t ulNotificationValue = ulTaskNotifyTake( pdTRUE, waitms );

        if( ulNotificationValue != 1 ) {
            /* The call to ulTaskNotifyTake() timed out. check anyway */
        }

        size_t n = read_tcp(rx_buf, sizeof(rx_buf));
        if(n > 0) {
        //    process_command_buffer(n, rx_buf, &os, line, cnt, &discard);
        }
    }
}



void tcp_setup(){
    printf("DEBUG: TCP Comms thread running\n");
    xTaskCreate(tcp_comms, "tcp_comms", 30000, NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);
}
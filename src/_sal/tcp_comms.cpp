// xQueue Create/send/receive
// https://www.youtube.com/watch?v=DLqj01asDM0&t=246s

#include "tcp_comms.h"
#include "libs/OutputStream.h"
#include "FreeRTOS.h"
#include "freertos/task.h"
#include "WiFi.h"

WiFiServer tcp_server(23456);

void start_tcp_server(){
    while(WiFi.status() != WL_CONNECTED){
        delay(1000);
        printf("[W][tcp_server] waitting for wifi connecting, trying agian.");   // How is ethernet tcp?
    }
    tcp_server.begin();
    printf("ttttttttttttttttttttttttttttttttttttt\n");
}

#define FTP_DEBUG     
WiFiClient client;
int cmdStatus = 0;
uint32_t millisEndConnection;
uint32_t millisDelay;
uint32_t millisTimeOut;
uint32_t transferStatus;
uint16_t iCL;

void abortTransfer()
{
  if( transferStatus > 0 )
  {
    // file.close();
    // data.stop(); 
    client.println( "426 Transfer aborted"  );
    #ifdef FTP_DEBUG
      Serial.println( "Transfer aborted!") ;
    #endif
  }
  transferStatus = 0;
}


void disconnectClient()
{
  #ifdef FTP_DEBUG
	Serial.println(" Disconnecting client");
  #endif
  abortTransfer();
  client.println("221 Goodbye");
  client.stop();
}


void iniVariables()
{
  // Default for data port
  // dataPort = FTP_DATA_PORT_PASV;
  
  // Default Data connection is Active
  // dataPassiveConn = true;
  
  // Set the root directory
  // strcpy( cwdName, "/" );

  // rnfrCmd = false;
  transferStatus = 0;
  
}
int8_t readChar()
{
  int8_t rc = -1;

  if( client.available())
  {
    char c = client.read();
	 char* cc ;
	 client.readBytes((uint8_t*) cc, 1);
    #ifdef FTP_DEBUG
      Serial.print( c);
    #endif
  }
  return rc;
}


void clientConnected()
{
  #ifdef FTP_DEBUG
	Serial.println("Client connected!");
  #endif
  client.println( "220--- Welcome to FTP for ESP8266 ---");
  client.println( "220---   By David Paiva   ---");
  // client.println( "220 --   Version "+ String(FTP_SERVER_VERSION) +"   --");
  iCL = 0;
}

void loop_333(){
 if((int32_t) ( millisDelay - millis() ) > 0 )
    return;

  if (tcp_server.hasClient()) {
    printf("hasClient()\n");
//  if (tcp_server.available()) {
	  client.stop();
	  client = tcp_server.available();
  }
  
  if( cmdStatus == 0 )
  {
    if( client.connected())
      disconnectClient();
      
    cmdStatus = 1;
  }
  else if( cmdStatus == 1 )         // Ftp server waiting for connection
  {
    abortTransfer();
    iniVariables();
    #ifdef FTP_DEBUG
	    Serial.println("Ftp server waiting for connection on port "+ String(23456));
    #endif
    cmdStatus = 2;
  }
  else if( cmdStatus == 2 )         // Ftp server idle
  {
   		
    if( client.connected() )                // A client connected
    {
      clientConnected();      
      millisEndConnection = millis() + 10 * 1000 ; // wait client id during 10 s.
      cmdStatus = 3;
    }
  }
  else if( readChar() > 0 )         // got response
  {
    // if( cmdStatus == 3 )            // Ftp server waiting for user identity
    //   if( userIdentity() )
    //     cmdStatus = 4;
    //   else
    //     cmdStatus = 0;
    // else if( cmdStatus == 4 )       // Ftp server waiting for user registration
    //   if( userPassword() )
    //   {
    //     cmdStatus = 5;
    //     millisEndConnection = millis() + millisTimeOut;
    //   }
    //   else
    //     cmdStatus = 0;
    // else if( cmdStatus == 5 )       // Ftp server waiting for user command
    //   if( ! processCommand())
    //     cmdStatus = 0;
    //   else
    //     millisEndConnection = millis() + millisTimeOut;
  }
  else if (!client.connected() || !client)
  {
	  cmdStatus = 1;
      #ifdef FTP_DEBUG
	    Serial.println("client disconnected");
	  #endif
  }

  if( transferStatus == 1 )         // Retrieve data
  {
    // if( ! doRetrieve())
    //   transferStatus = 0;
  }
  else if( transferStatus == 2 )    // Store data
  {
    // if( ! doStore())
    //   transferStatus = 0;
  }
  else if( cmdStatus > 2 && ! ((int32_t) ( millisEndConnection - millis() ) > 0 ))
  {
	  client.println("530 Timeout");
    millisDelay = millis() + 200;    // delay of 200 ms
    cmdStatus = 0;
  }
  
}

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
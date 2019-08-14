//                     Hex file size of     RAM
//  Normal C pattern :       1267198       47436      
// Singleton pattern :       1266950       47436
//        difference :           248           0


#define TCP_DEBUG_ON

#ifndef TCP_SERVERESP_H
#define TCP_SERVERESP_H

//#include "Streaming.h"

#include "stdint.h"
#include <WiFiClient.h>
#include <_SAL/StateMachine/StateMachine.h>

#define TCP_SERVER_VERSION "TCP Aug2019"

#define TCP_CTRL_PORT    23456          // Command port on wich server is listening  
// #define TCP_DATA_PORT_PASV 50009     // Data port in passive mode

#define TCP_TIME_OUT  5           // Disconnect client after 5 minutes of inactivity
#define TCP_CMD_SIZE 255 + 8 // max size of a command
#define TCP_CWD_SIZE 255 + 8 // max size of a directory name
#define TCP_FIL_SIZE 255     // max size of a file name
#define TCP_BUF_SIZE 1024 //512   // size of file buffer for read/write

typedef enum e_STATES {
    LISTENING
    ,CONEECTED
    ,ON_SUB_PORT
    ,TIME_OUT
} STATES_t ;


class TcpServer: public StateMachine<STATES_t> {
    public:
        TcpServer();

        static TcpServer* get_instance();
        
        void    begin(String uname, String pword);
        void    handleTCP();

    private:
        static TcpServer* __instance;
        bool is_allowed_to_enter_(STATES_t new_state) override;
        
        void    iniVariables();
        void    clientConnected();
        void    disconnectClient();
        boolean userIdentity();
        boolean userPassword();
        boolean processCommand();
        boolean dataConnect();
        boolean doRetrieve();
        boolean doStore();
        void    closeTransfer();
        void    abortTransfer();
        boolean makePath( char * fullname );
        boolean makePath( char * fullName, char * param );
        uint8_t getDateTime( uint16_t * pyear, uint8_t * pmonth, uint8_t * pday,
                            uint8_t * phour, uint8_t * pminute, uint8_t * second );
        char *  makeDateTimeStr( char * tstr, uint16_t date, uint16_t time );
        int8_t  readChar();

        IPAddress      dataIp;              // IP address of client for data
        WiFiClient client;
        WiFiClient data;
        
        // File file;
        
        // boolean  dataPassiveConn;
        // uint16_t dataPort;
        char     buf[ TCP_BUF_SIZE ];       // data buffer for transfers
        char     cmdLine[ TCP_CMD_SIZE ];   // where to store incoming char from client
        char     cwdName[ TCP_CWD_SIZE ];   // name of current directory
        char     command[ 5 ];              // command sent by client
        boolean  rnfrCmd;                   // previous command was RNFR
        char *   parameters;                // point to begin of parameters sent by client
        uint16_t iCL;                       // pointer to cmdLine next incoming char
        int8_t   cmdStatus,                 // status of ftp command connexion
                transferStatus;            // status of ftp data transfer
        uint32_t millisTimeOut,             // disconnect after 5 min of inactivity
                millisDelay,
                millisEndConnection,       // 
                millisBeginTrans          // store time of beginning of a transaction
                //  bytesTransfered;           //
        // String   _TCP_USER;
        // String   _TCP_PASS;
        ;

};

#endif // TCP_SERVERESP_H


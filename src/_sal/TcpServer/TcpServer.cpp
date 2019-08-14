
#include "stdio.h"
#include "TcpServer.h"

#include <WiFi.h>
#include <WiFiClient.h>
//#include <ESP32WebServer.h>


WiFiServer tcp_server( TCP_CTRL_PORT );

TcpServer* TcpServer::__instance = nullptr; 

TcpServer::TcpServer() : StateMachine<e_STATES>(LISTENING){
  printf("[D]TcpServer() at entrance\n");
}

void TcpServer::begin(String uname, String pword)
{

  printf("[V][TcpServer.begin()] at entrance\n");

	tcp_server.begin();

}
// template<T>
bool TcpServer::is_allowed_to_enter_(STATES_t new_state){
  return true;
}
void TcpServer::iniVariables()
{
  // Default for data port
  // dataPort = TCP_DATA_PORT_PASV;
  
  // Default Data connection is Active
  // dataPassiveConn = true;
  
  // Set the root directory
  strcpy( cwdName, "/" );

  rnfrCmd = false;
  transferStatus = 0;
  
}

void TcpServer::handleTCP()
{
  // return;
  // if((int32_t) ( millisDelay - millis() ) > 0 )
  //   return;
  // printf("tttttttccccccccccpppppppppppp\n");
  if (tcp_server.hasClient()) {
    printf("[V][TcpServer][haneleTCP()] hasClient() \n");
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
    #ifdef TCP_DEBUG_ON
	Serial.println("Ftp server waiting for connection on port "+ String(TCP_CTRL_PORT));
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
    if( cmdStatus == 3 )            // Ftp server waiting for user identity
      if( userIdentity() )
        cmdStatus = 4;
      else
        cmdStatus = 0;
    else if( cmdStatus == 4 )       // Ftp server waiting for user registration
      if( userPassword() )
      {
        cmdStatus = 5;
        millisEndConnection = millis() + millisTimeOut;
      }
      else
        cmdStatus = 0;
    else if( cmdStatus == 5 )       // Ftp server waiting for user command
      if( ! processCommand())
        cmdStatus = 0;
      else
        millisEndConnection = millis() + millisTimeOut;
  }
  else if (!client.connected() || !client)
  {
	  cmdStatus = 1;
      #ifdef TCP_DEBUG_ON
	    Serial.println("client disconnected");
	  #endif
  }

  // if( transferStatus == 1 )         // Retrieve data
  // {
  //   if( ! doRetrieve())
  //     transferStatus = 0;
  // }
  // else if( transferStatus == 2 )    // Store data
  // {
  //   if( ! doStore())
  //     transferStatus = 0;
  // }
  // else if( cmdStatus > 2 && ! ((int32_t) ( millisEndConnection - millis() ) > 0 ))
  // {
	//   client.println("530 Timeout");
  //   millisDelay = millis() + 200;    // delay of 200 ms
  //   cmdStatus = 0;
  // }
}

void TcpServer::clientConnected()
{
  #ifdef TCP_DEBUG_ON
	Serial.println("Client connected!");
  #endif
  client.println( "220--- Welcome to TCP for ESP8266 ---");
  client.println( "220---   By David Paiva   ---");
  client.println( "220 --   Version "+ String(TCP_SERVER_VERSION) +"   --");
  iCL = 0;
}

void TcpServer::disconnectClient()
{
  #ifdef TCP_DEBUG_ON
	Serial.println(" Disconnecting client");
  #endif
  abortTransfer();
  client.println("221 Goodbye");
  client.stop();
}

boolean TcpServer::userIdentity()
{	
  // if( strcmp( command, "USER" ))
  //   client.println( "500 Syntax error");
  // if( strcmp( parameters, _TCP_USER.c_str() ))
  //   client.println( "530 user not found");
  // else
  // {
  //   client.println( "331 OK. Password required");
  //   strcpy( cwdName, "/" );
  //   return true;
  // }
  millisDelay = millis() + 100;  // delay of 100 ms
  return false;
}

boolean TcpServer::userPassword()
{
  // if( strcmp( command, "PASS" ))
  //   client.println( "500 Syntax error");
  // else if( strcmp( parameters, _TCP_PASS.c_str() ))
  //   client.println( "530 ");
  // else
  // {
    // #ifdef TCP_DEBUG_ON
      Serial.println( "OK. Waiting for commands.");
    // #endif
    client.println( "230 OK.");
    return true;
  // }
  // millisDelay = millis() + 100;  // delay of 100 ms
  // return false;
}

boolean TcpServer::processCommand()
{
  ///////////////////////////////////////
  //                                   //
  //      ACCESS CONTROL COMMANDS      //
  //                                   //
  ///////////////////////////////////////

  //
  //  CDUP - Change to Parent Directory 
  //
  // if( ! strcmp( command, "CDUP" ))
  // {
	//   client.println("250 Ok. Current directory is " + String(cwdName));
  // }
  //
  //  CWD - Change Working Directory
  //
  // else if( ! strcmp( command, "CWD" ))
  // {
  //   char path[ TCP_CWD_SIZE ];
  //   if( strcmp( parameters, "." ) == 0 )  // 'CWD .' is the same as PWD command
  //     client.println( "257 \"" + String(cwdName) + "\" is your current directory");
  //   else 
  //     {       
  //       client.println( "250 Ok. Current directory is " + String(cwdName) );
  //     }
    
  // }
  //
  //  PWD - Print Directory
  //
  // else if( ! strcmp( command, "PWD" ))
  //   client.println( "257 \"" + String(cwdName) + "\" is your current directory");
  // //
  // //  QUIT
  // //
  // else if( ! strcmp( command, "QUIT" ))
  // {
  //   disconnectClient();
  //   return false;
  // }

  ///////////////////////////////////////
  //                                   //
  //    TRANSFER PARAMETER COMMANDS    //
  //                                   //
  ///////////////////////////////////////

  //
  //  MODE - Transfer Mode 
  //
  // else if( ! strcmp( command, "MODE" ))
  // {
  //   if( ! strcmp( parameters, "S" ))
  //     client.println( "200 S Ok");
  //   // else if( ! strcmp( parameters, "B" ))
  //   //  client.println( "200 B Ok\r\n";
  //   else
  //     client.println( "504 Only S(tream) is suported");
  // }
  // //
  // //  PASV - Passive Connection management
  // //
  // else if( ! strcmp( command, "PASV" ))
  // {
  //   if (data.connected()) data.stop();
  //   //dataServer.begin();
  //    //dataIp = Ethernet.localIP();    
	// dataIp = WiFi.localIP();	
	// dataPort = TCP_DATA_PORT_PASV;
  //   //data.connect( dataIp, dataPort );
  //   //data = dataServer.available();
  //   #ifdef TCP_DEBUG_ON
	// Serial.println("Connection management set to passive");
  //     Serial.println( "Data port set to " + String(dataPort));
  //   #endif
  //  client.println( "227 Entering Passive Mode ("+ String(dataIp[0]) + "," + String(dataIp[1])+","+ String(dataIp[2])+","+ String(dataIp[3])+","+String( dataPort >> 8 ) +","+String ( dataPort & 255 )+").");
  //  dataPassiveConn = true;
  // }
  //
  //  PORT - Data Port
  //
  // else if( ! strcmp( command, "PORT" ))
  // {
	// if (data) data.stop();
  //   // get IP of data client
  //   dataIp[ 0 ] = atoi( parameters );
  //   char * p = strchr( parameters, ',' );
  //   for( uint8_t i = 1; i < 4; i ++ )
  //   {
  //     dataIp[ i ] = atoi( ++ p );
  //     p = strchr( p, ',' );
  //   }
  //   // get port of data client
  //   dataPort = 256 * atoi( ++ p );
  //   p = strchr( p, ',' );
  //   dataPort += atoi( ++ p );
  //   if( p == NULL )
  //     client.println( "501 Can't interpret parameters");
  //   else
  //   {
      
	// 	client.println("200 PORT command successful");
  //     dataPassiveConn = false;
  //   }
  // }
  //
  //  STRU - File Structure
  //
  // else if( ! strcmp( command, "STRU" ))
  // {
  //   if( ! strcmp( parameters, "F" ))
  //     client.println( "200 F Ok");
  //   // else if( ! strcmp( parameters, "R" ))
  //   //  client.println( "200 B Ok\r\n";
  //   else
  //     client.println( "504 Only F(ile) is suported");
  // }
  //
  //  TYPE - Data Type
  //
  // else if( ! strcmp( command, "TYPE" ))
  // {
  //   if( ! strcmp( parameters, "A" ))
  //     client.println( "200 TYPE is now ASII");
  //   else if( ! strcmp( parameters, "I" ))
  //     client.println( "200 TYPE is now 8-bit binary");
  //   else
  //     client.println( "504 Unknow TYPE");
  // }

  ///////////////////////////////////////
  //                                   //
  //        TCP SERVICE COMMANDS       //
  //                                   //
  ///////////////////////////////////////

  //
  //  ABOR - Abort
  //
  // else if( ! strcmp( command, "ABOR" ))
  // {
  //   abortTransfer();
  //   client.println( "226 Data connection closed");
  // }
  //
  //  DELE - Delete a File 
  //
  // else if( ! strcmp( command, "DELE" ))
  // {
  //   char path[ TCP_CWD_SIZE ];
  //   if( strlen( parameters ) == 0 )
  //     client.println( "501 No file name");
  //   else if( makePath( path ))
  //   {
  //     if( ! SD.exists( path ))
  //       client.println( "550 File " + String(parameters) + " not found");
  //     else
  //     {
  //       if( SD.remove( path ))
  //         client.println( "250 Deleted " + String(parameters) );
  //       else
  //         client.println( "450 Can't delete " + String(parameters));
  //     }
  //   }
  // }
  //
  //  LIST - List 
  //
//   else if( ! strcmp( command, "LIST" ))
//   {
//     if( ! dataConnect())
//       client.println( "425 No data connection");
//     else
//     {
//       client.println( "150 Accepted data connection");
//       uint16_t nm = 0;
// //      Dir dir=SD.openDir(cwdName);
//       File dir=SD.open(cwdName);
// //      if( !SD.exists(cwdName))
//      if((!dir)||(!dir.isDirectory()))
//         client.println( "550 Can't open directory " + String(cwdName) );
//       else
//       {
//         File file = dir.openNextFile();
//         while( file)
//         {
//     			String fn, fs;
//           fn = file.name();
//     			fn.remove(0, 1);
//       		#ifdef TCP_DEBUG_ON
//   			  Serial.println("File Name = "+ fn);
//       		#endif
//           fs = String(file.size());
//           if(file.isDirectory()){
//             data.println( "01-01-2000  00:00AM <DIR> " + fn);
//           } else {
//             data.println( "01-01-2000  00:00AM " + fs + " " + fn);
// //          data.println( " " + fn );
//           }
//           nm ++;
//           file = dir.openNextFile();
//         }
//         client.println( "226 " + String(nm) + " matches total");
//       }
//       data.stop();
//     }
//   }
  //
  //  MLSD - Listing for Machine Processing (see RFC 3659)
//   //
//   else if( ! strcmp( command, "MLSD" ))
//   {
//     if( ! dataConnect())
//       client.println( "425 No data connection MLSD");
//     else
//     {
// 	  client.println( "150 Accepted data connection");
//       uint16_t nm = 0;
// //      Dir dir= SD.openDir(cwdName);
//       File dir= SD.open(cwdName);
//       char dtStr[ 15 ];
//     //  if(!SD.exists(cwdName))
//      if((!dir)||(!dir.isDirectory()))
//         client.println( "550 Can't open directory " +String(cwdName) );
// //        client.println( "550 Can't open directory " +String(parameters) );
//       else
//       {
// //        while( dir.next())
//         File file = dir.openNextFile();
// //        while( dir.openNextFile())
//         while( file)
//     		{
//     			String fn,fs;
//           fn = file.name();
//     			fn.remove(0, 1);
//           fs = String(file.size());
//           if(file.isDirectory()){
//             data.println( "Type=dir;Size=" + fs + ";"+"modify=20000101000000;" +" " + fn);
// //            data.println( "Type=dir;modify=20000101000000; " + fn);
//           } else {
//             //data.println( "Type=file;Size=" + fs + ";"+"modify=20000101160656;" +" " + fn);
//             data.println( "Type=file;Size=" + fs + ";"+"modify=20000101000000;" +" " + fn);
//           }
//           nm ++;
//           file = dir.openNextFile();
//         }
//         client.println( "226-options: -a -l");
//         client.println( "226 " + String(nm) + " matches total");
//       }
//       data.stop();
//     }
//   }
  //
  //  NLST - Name List 
  //
//   else if( ! strcmp( command, "NLST" ))
//   {
//     if( ! dataConnect())
//       client.println( "425 No data connection");
//     else
//     {
//       client.println( "150 Accepted data connection");
//       uint16_t nm = 0;
// //      Dir dir=SD.openDir(cwdName);
//       File dir= SD.open(cwdName);
//       if( !SD.exists( cwdName ))
//         client.println( "550 Can't open directory " + String(parameters));
//       else
//       {
//           File file = dir.openNextFile();
// //        while( dir.next())
//         while( file)
//         {
// //          data.println( dir.fileName());
//           data.println( file.name());
//           nm ++;
//           file = dir.openNextFile();
//         }
//         client.println( "226 " + String(nm) + " matches total");
//       }
//       data.stop();
//     }
//   }
  //
  //  NOOP
  //
  // else if( ! strcmp( command, "NOOP" ))
  // {
  //   // dataPort = 0;
  //   client.println( "200 Zzz...");
  // }
  //
  //  RETR - Retrieve
  //
  // else if( ! strcmp( command, "RETR" ))
  // {
  //   char path[ TCP_CWD_SIZE ];
  //   if( strlen( parameters ) == 0 )
  //     client.println( "501 No file name");
  //   else if( makePath( path ))
	// {
	// 	file = SD.open(path, "r");
  //     if( !file)
  //       client.println( "550 File " +String(parameters)+ " not found");
  //     else if( !file )
  //       client.println( "450 Can't open " +String(parameters));
  //     else if( ! dataConnect())
  //       client.println( "425 No data connection");
  //     else
  //     {
  //       #ifdef TCP_DEBUG_ON
	// 	  Serial.println("Sending " + String(parameters));
  //       #endif
  //       client.println( "150-Connected to port "+ String(dataPort));
  //       client.println( "150 " + String(file.size()) + " bytes to download");
  //       millisBeginTrans = millis();
  //       bytesTransfered = 0;
  //       transferStatus = 1;
  //     }
  //   }
  // }
  //
  //  STOR - Store
  //
  // else if( ! strcmp( command, "STOR" ))
  // {
  //   char path[ TCP_CWD_SIZE ];
  //   if( strlen( parameters ) == 0 )
  //     client.println( "501 No file name");
  //   else if( makePath( path ))
  //   {
	// 	file = SD.open(path, "w");
  //     if( !file)
  //       client.println( "451 Can't open/create " +String(parameters) );
  //     else if( ! dataConnect())
  //     {
  //       client.println( "425 No data connection");
  //       file.close();
  //     }
  //     else
  //     {
  //       #ifdef TCP_DEBUG_ON
  //         Serial.println( "Receiving " +String(parameters));
  //       #endif
  //       client.println( "150 Connected to port " + String(dataPort));
  //       millisBeginTrans = millis();
  //       bytesTransfered = 0;
  //       transferStatus = 2;
  //     }
  //   }
  // }
  //
  //  MKD - Make Directory
  //
  // else if( ! strcmp( command, "MKD" ))
  // {
	//   client.println( "550 Can't create \"" + String(parameters));  //not support on espyet
  // }
  //
  //  RMD - Remove a Directory 
  //
  // else if( ! strcmp( command, "RMD" ))
  // {
	//   client.println( "501 Can't delete \"" +String(parameters));
	
  // }
  //
  //  RNFR - Rename From 
  //
  // else if( ! strcmp( command, "RNFR" ))
  // {
  //   buf[ 0 ] = 0;
  //   if( strlen( parameters ) == 0 )
  //     client.println( "501 No file name");
  //   else if( makePath( buf ))
  //   {
  //     if( ! SD.exists( buf ))
  //       client.println( "550 File " +String(parameters)+ " not found");
  //     else
  //     {
  //       #ifdef TCP_DEBUG_ON
	// 	  Serial.println("Renaming " + String(buf));
  //       #endif
  //       client.println( "350 RNFR accepted - file exists, ready for destination");     
  //       rnfrCmd = true;
  //     }
  //   }
  // }
  //
  //  RNTO - Rename To 
  //
  // else if( ! strcmp( command, "RNTO" ))
  // {  
  //   char path[ TCP_CWD_SIZE ];
  //   char dir[ TCP_FIL_SIZE ];
  //   if( strlen( buf ) == 0 || ! rnfrCmd )
  //     client.println( "503 Need RNFR before RNTO");
  //   else if( strlen( parameters ) == 0 )
  //     client.println( "501 No file name");
  //   else if( makePath( path ))
  //   {
  //     if( SD.exists( path ))
  //       client.println( "553 " +String(parameters)+ " already exists");
  //     else
  //     {          
  //           #ifdef TCP_DEBUG_ON
	// 	  Serial.println("Renaming " + String(buf) + " to " + String(path));
  //           #endif
  //           if( SD.rename( buf, path ))
  //             client.println( "250 File successfully renamed or moved");
  //           else
	// 			client.println( "451 Rename/move failure");
                                 
  //     }
  //   }
  //   rnfrCmd = false;
  // }

  ///////////////////////////////////////
  //                                   //
  //   EXTENSIONS COMMANDS (RFC 3659)  //
  //                                   //
  ///////////////////////////////////////

  //
  //  FEAT - New Features
  //
  // else if( ! strcmp( command, "FEAT" ))
  // {
  //   client.println( "211-Extensions suported:");
  //   client.println( " MLSD");
  //   client.println( "211 End.");
  // }
  //
  //  MDTM - File Modification Time (see RFC 3659)
  //
  // else if (!strcmp(command, "MDTM"))
  // {
	//   client.println("550 Unable to retrieve time");
  // }

  // //
  // //  SIZE - Size of the file
  // //
  // else if( ! strcmp( command, "SIZE" ))
  // {
  //   char path[ TCP_CWD_SIZE ];
  //   if( strlen( parameters ) == 0 )
  //     client.println( "501 No file name");
  //   else if( makePath( path ))
	// {
	// 	file = SD.open(path, "r");
  //     if(!file)
  //        client.println( "450 Can't open " +String(parameters) );
  //     else
  //     {
  //       client.println( "213 " + String(file.size()));
  //       file.close();
  //     }
  //   }
  // }
  //
  //  SITE - System command
  //
  // else if( ! strcmp( command, "SITE" ))
  // {
  //     client.println( "500 Unknow SITE command " +String(parameters) );
  // }
  // //
  // //  Unrecognized commands ...
  // //
  // else
  //   client.println( "500 Unknow command");
  
  // return true;
}

boolean TcpServer::dataConnect()
{
//   unsigned long startTime = millis();
//   //wait 5 seconds for a data connection
//   if (!data.connected())
//   {
//     while (!dataServer.hasClient() && millis() - startTime < 10000)
// //    while (!dataServer.available() && millis() - startTime < 10000)
// 	  {
// 		  //delay(100);
// 		  yield();
// 	  }
//     if (dataServer.hasClient()) {
// //    if (dataServer.available()) {
// 		  data.stop();
// 		  data = dataServer.available();
// 			#ifdef TCP_DEBUG_ON
// 		      Serial.println("ftpdataserver client....");
// 			#endif
		
// 	  }
//   }

//   return data.connected();

}

boolean TcpServer::doRetrieve()
{
	//int16_t nb = file.readBytes((uint8_t*) buf, TCP_BUF_SIZE );
	// int16_t nb = file.readBytes(buf, TCP_BUF_SIZE);
  // if( nb > 0 )
  // { 
  //   data.write((uint8_t*) buf, nb );
  //   bytesTransfered += nb;
  //   return true;
  // }
  // closeTransfer();
  // return false;
}

boolean TcpServer::doStore()
{
  // if( data.connected() )
  // {
  //   int16_t nb = data.readBytes((uint8_t*) buf, TCP_BUF_SIZE );
  //   if( nb > 0 )
  //   {
  //     // Serial.println( millis() << " " << nb << endl;
  //     file.write((uint8_t*) buf, nb );
  //     bytesTransfered += nb;
  //   }
  //   return true;
  // }
  // closeTransfer();
  return false;
}

void TcpServer::closeTransfer()
{
  // uint32_t deltaT = (int32_t) ( millis() - millisBeginTrans );
  // if( deltaT > 0 && bytesTransfered > 0 )
  // {
  //   client.println( "226-File successfully transferred");
  //   client.println( "226 " + String(deltaT) + " ms, "+ String(bytesTransfered / deltaT) + " kbytes/s");
  // }
  // else
  //   client.println( "226 File successfully transferred");
  
  // file.close();
  // data.stop();
}

void TcpServer::abortTransfer()
{
  // if( transferStatus > 0 )
  // {
  //   file.close();
  //   data.stop(); 
  //   client.println( "426 Transfer aborted"  );
  //   #ifdef TCP_DEBUG_ON
  //     Serial.println( "Transfer aborted!") ;
  //   #endif
  // }
  // transferStatus = 0;
}

// Read a char from client connected to ftp server
//
//  update cmdLine and command buffers, iCL and parameters pointers
//
//  return:
//    -2 if buffer cmdLine is full
//    -1 if line not completed
//     0 if empty line received
//    length of cmdLine (positive) if no empty line received 

int8_t TcpServer::readChar()
{
  int8_t rc = -1;

  if( client.available())
  {
    char c = client.read();
	 // char c;
	 // client.readBytes((uint8_t*) c, 1);
    #ifdef TCP_DEBUG_ON
      Serial.print( c);
    #endif
    if( c == '\\' )
      c = '/';
    if( c != '\r' )
      if( c != '\n' )
      {
        if( iCL < TCP_CMD_SIZE )
          cmdLine[ iCL ++ ] = c;
        else
          rc = -2; //  Line too long
      }
      else
      {
        cmdLine[ iCL ] = 0;
        command[ 0 ] = 0;
        parameters = NULL;
        // empty line?
        if( iCL == 0 )
          rc = 0;
        else
        {
          rc = iCL;
          // search for space between command and parameters
          parameters = strchr( cmdLine, ' ' );
          if( parameters != NULL )
          {
            if( parameters - cmdLine > 4 )
              rc = -2; // Syntax error
            else
            {
              strncpy( command, cmdLine, parameters - cmdLine );
              command[ parameters - cmdLine ] = 0;
              
              while( * ( ++ parameters ) == ' ' )
                ;
            }
          }
          else if( strlen( cmdLine ) > 4 )
            rc = -2; // Syntax error.
          else
            strcpy( command, cmdLine );
          iCL = 0;
        }
      }
    if( rc > 0 )
      for( uint8_t i = 0 ; i < strlen( command ); i ++ )
        command[ i ] = toupper( command[ i ] );
    if( rc == -2 )
    {
      iCL = 0;
      client.println( "500 Syntax error");
    }
  }
  return rc;
}

// Make complete path/name from cwdName and parameters
//
// 3 possible cases: parameters can be absolute path, relative path or only the name
//
// parameters:
//   fullName : where to store the path/name
//
// return:
//    true, if done

boolean TcpServer::makePath( char * fullName )
{
  return makePath( fullName, parameters );
}

boolean TcpServer::makePath( char * fullName, char * param )
{
  if( param == NULL )
    param = parameters;
    
  // Root or empty?
  if( strcmp( param, "/" ) == 0 || strlen( param ) == 0 )
  {
    strcpy( fullName, "/" );
    return true;
  }
  // If relative path, concatenate with current dir
  if( param[0] != '/' ) 
  {
    strcpy( fullName, cwdName );
    if( fullName[ strlen( fullName ) - 1 ] != '/' )
      strncat( fullName, "/", TCP_CWD_SIZE );
    strncat( fullName, param, TCP_CWD_SIZE );
  }
  else
    strcpy( fullName, param );
  // If ends with '/', remove it
  uint16_t strl = strlen( fullName ) - 1;
  if( fullName[ strl ] == '/' && strl > 1 )
    fullName[ strl ] = 0;
  if( strlen( fullName ) < TCP_CWD_SIZE )
    return true;

  client.println( "500 Command line too long");
  return false;
}

// Calculate year, month, day, hour, minute and second
//   from first parameter sent by MDTM command (YYYYMMDDHHMMSS)
//
// parameters:
//   pyear, pmonth, pday, phour, pminute and psecond: pointer of
//     variables where to store data
//
// return:
//    0 if parameter is not YYYYMMDDHHMMSS
//    length of parameter + space

uint8_t TcpServer::getDateTime( uint16_t * pyear, uint8_t * pmonth, uint8_t * pday,
                                uint8_t * phour, uint8_t * pminute, uint8_t * psecond )
{
  char dt[ 15 ];

  // Date/time are expressed as a 14 digits long string
  //   terminated by a space and followed by name of file
  if( strlen( parameters ) < 15 || parameters[ 14 ] != ' ' )
    return 0;
  for( uint8_t i = 0; i < 14; i++ )
    if( ! isdigit( parameters[ i ]))
      return 0;

  strncpy( dt, parameters, 14 );
  dt[ 14 ] = 0;
  * psecond = atoi( dt + 12 ); 
  dt[ 12 ] = 0;
  * pminute = atoi( dt + 10 );
  dt[ 10 ] = 0;
  * phour = atoi( dt + 8 );
  dt[ 8 ] = 0;
  * pday = atoi( dt + 6 );
  dt[ 6 ] = 0 ;
  * pmonth = atoi( dt + 4 );
  dt[ 4 ] = 0 ;
  * pyear = atoi( dt );
  return 15;
}

// Create string YYYYMMDDHHMMSS from date and time
//
// parameters:
//    date, time 
//    tstr: where to store the string. Must be at least 15 characters long
//
// return:
//    pointer to tstr

char * TcpServer::makeDateTimeStr( char * tstr, uint16_t date, uint16_t time )
{
  sprintf( tstr, "%04u%02u%02u%02u%02u%02u",
           (( date & 0xFE00 ) >> 9 ) + 1980, ( date & 0x01E0 ) >> 5, date & 0x001F,
           ( time & 0xF800 ) >> 11, ( time & 0x07E0 ) >> 5, ( time & 0x001F ) << 1 );            
  return tstr;
}

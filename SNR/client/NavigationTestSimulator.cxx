/*=========================================================================

  Program:   BRP Prostate Robot: Testing Simulator (Client)
  Language:  C++

  Copyright (c) Brigham and Women's Hospital. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

  Please see
    http://wiki.na-mic.org/Wiki/index.php/ProstateBRP_OpenIGTLink_Communication_June_2013
  for the detail of the testing protocol.

=========================================================================*/

#include <iostream>
#include <math.h>
#include <cstdlib>

#include "igtlClientSocket.h"
#include "NavigationNormalOperationTest.h"
#include "NavigationStartUpErrorTest.h"
#include "NavigationCalibrationErrorTest.h"
#include "NavigationTargetingWithoutCalibrationTest.h"
#include "NavigationOutOfRangeTest.h"
#include "NavigationStopDuringOperationTest.h"
#include "NavigationEmergencyStopDuringOperationTest.h"
#include "NavigationMoveToWithoutTargetTest.h"
#include "NavigationAccidentalCommandDuringManualModeTest.h"
#include "NavigationHardwareErrorDuringOperationTest.h"
#include "NavigationDynamicCommunicationTest.h"

#include "script.hxx"
#include "pthread.h"
//void *startTests(void *socketPtr);

int main(int argc, char* argv[])
{
  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 6) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <hostname> <port> <string>"    << std::endl;
    std::cerr << "    <wpiHostname> : IP or hostname for WPI robot server connection" << std::endl;
    std::cerr << "    <wpiPort>     : Port # for WPI robot server connection"   << std::endl;
    std::cerr << "    <snrHostname> : IP or hostname for Slicer server connection" << std::endl;
    std::cerr << "    <snrPort>     : Port # for Slicer server connection" << std::endl;
    std::cerr << "    <test>     : Test # (1-10)"   << std::endl;
    exit(0);
  }

  char*  wpiHostname = argv[1];
  int    wpiPort     = atoi(argv[2]);
  char*  snrHostname = argv[3];
  int    snrPort     = atoi(argv[4]);
  int    test     = atoi(argv[5]);

  if (test < 0 || test > 10)
  {
    std::cerr << "Invalid test" << std::endl;
    exit(0);
  }

  //------------------------------------------------------------
  // Establish Connection

  igtl::ClientSocket::Pointer socket;
  socket = igtl::ClientSocket::New();
  int r = socket->ConnectToServer(wpiHostname, wpiPort);

  if (r != 0)
  {
    std::cerr << "Cannot connect to the server." << std::endl;
    exit(0);
  }

  // Call function in script.cxx to establish connection and start the thread that receives messages from Slicer
  setSocketVars(snrHostname, snrPort);

  // Call startThread function in script.cxx in a thread s.t. the rest of main() continues to run simultaneously
  pthread_t thread;
  pthread_create(&thread, NULL, startThread, NULL);
  //pthread_exit(NULL);


  //------------------------------------------------------------
  // Call Test
  NavigationTestBase* navTest = NULL;

  switch (test)
  {
    case 0:
    {
      std::cout << "------------------- Starting NavigationDynamicCommunicationTest -------------------" << std::endl;
      navTest = (NavigationDynamicCommunicationTest*) new NavigationDynamicCommunicationTest();
      break;
    }
    case 1:
    {
      std::cout << "------------------- Starting NavigationNormalOperationTest -------------------" << std::endl;
      navTest = (NavigationNormalOperationTest*) new NavigationNormalOperationTest();
      break;
    }
    // case 2:
    //   {
    //     navTest = (NavigationStartUpErrorTest*) new NavigationStartUpErrorTest();
    //     break;
    //   }
    // case 3:
    //   {
    //     navTest = (NavigationCalibrationErrorTest*) new NavigationCalibrationErrorTest();
    //     break;
    //   }
    // case 4:
    //   {
    //     navTest = (NavigationTargetingWithoutCalibrationTest*) new NavigationTargetingWithoutCalibrationTest();
    //     break;
    //   }
    // case 5:
    //   {
    //     navTest = (NavigationOutOfRangeTest*) new NavigationOutOfRangeTest();
    //     break;
    //   }
    // case 6:
    //   {
    //     navTest = (NavigationStopDuringOperationTest*) new NavigationStopDuringOperationTest();
    //     break;
    //   }
    // case 7:
    //   {
    //     navTest = (NavigationEmergencyStopDuringOperationTest*) new NavigationEmergencyStopDuringOperationTest();
    //     break;
    //   }
    // case 8:
    //   {
    //     navTest = (NavigationMoveToWithoutTargetTest*) new NavigationMoveToWithoutTargetTest();
    //     break;
    //   }
    // case 9:
    //   {
    //     navTest = (NavigationAccidentalCommandDuringManualModeTest*) new NavigationAccidentalCommandDuringManualModeTest();
    //     break;
    //   }
    // case 10:
    //   {
    //     navTest = (NavigationHardwareErrorDuringOperationTest*) new NavigationHardwareErrorDuringOperationTest();
    //     break;
    //   }
    // default:
    //   break;
  }

  if (navTest)
  {
    // Set timeout values (ms)
    navTest->SetTimeoutShort(1000);
    navTest->SetTimeoutMedium(5000);
    navTest->SetTimeoutMedium(10000);

    navTest->SetSocket(socket);
    navTest->Exec();
  }

  //------------------------------------------------------------
  // Close connection

  socket->CloseSocket();
}


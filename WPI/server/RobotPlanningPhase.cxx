/*=========================================================================
  Language:  C++
  Please see
    http://wiki.na-mic.org/Wiki/index.php/ProstateBRP_OpenIGTLink_Communication_June_2013
  for the detail of the protocol.

=========================================================================*/

#include "RobotPlanningPhase.h"
#include <string.h>
#include <stdlib.h>

#include "igtlOSUtil.h"
#include "igtlStringMessage.h"
#include "igtlClientSocket.h"
#include "igtlStatusMessage.h"
#include "igtlTransformMessage.h"
#include <cmath>

RobotPlanningPhase::RobotPlanningPhase() :
  RobotPhaseBase()
{
}


RobotPlanningPhase::~RobotPlanningPhase()
{
}

int RobotPlanningPhase::Initialize()
{
  return 1;
}


int RobotPlanningPhase::MessageHandler(igtl::MessageHeader* headerMsg)
{

  if (RobotPhaseBase::MessageHandler(headerMsg))
    {
    return 1;
    }

  return 0;
}


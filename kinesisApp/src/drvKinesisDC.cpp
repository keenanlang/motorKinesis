#include "drvKinesis.h"
#include "Thorlabs.MotionControl.KCube.DCServo.h"

/*
 * DC Motor Implementation
 */
int KinesisDCMotorAxis::connect()        
{
    TLI_BuildDeviceList();
    return CC_Open(this->serial); 
}
void KinesisDCMotorAxis::disconnect()    { CC_Close(this->serial); }

void KinesisDCMotorAxis::startPoll(int interval)    { CC_StartPolling(this->serial, interval); }
void KinesisDCMotorAxis::stopPoll()                 { CC_StopPolling(this->serial); }

void KinesisDCMotorAxis::enableChannel()    { CC_EnableChannel(this->serial); }
void KinesisDCMotorAxis::disableChannel()   { CC_DisableChannel(this->serial); }

int KinesisDCMotorAxis::getPosition()    { return CC_GetPosition(this->serial); }
int KinesisDCMotorAxis::getStatus()      { return CC_GetStatusBits(this->serial); }

void KinesisDCMotorAxis::moveRelative(double position)      { CC_MoveRelative(this->serial, position); }
void KinesisDCMotorAxis::moveToPosition(double position)    { CC_MoveToPosition(this->serial, position); }

bool KinesisDCMotorAxis::canHome()    { CC_CanHome(this->serial); }
void KinesisDCMotorAxis::home()       { CC_Home(this->serial); }

int KinesisDCMotorAxis::stopImmediate()    { CC_StopImmediate(this->serial); }
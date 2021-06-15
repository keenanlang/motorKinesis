#include "drvKinesis.h"
#include "Thorlabs.MotionControl.KCube.StepperMotor.h"


/*
 * Stepper Motor Implementation
 */
int KinesisStepMotorAxis::connect()        
{
    TLI_BuildDeviceList();
    return SCC_Open(this->serial); 
}
void KinesisStepMotorAxis::disconnect()    { SCC_Close(this->serial); }

void KinesisStepMotorAxis::startPoll(int interval)    { SCC_StartPolling(this->serial, interval); }
void KinesisStepMotorAxis::stopPoll()                 { SCC_StopPolling(this->serial); }

void KinesisStepMotorAxis::enableChannel()    { SCC_EnableChannel(this->serial); }
void KinesisStepMotorAxis::disableChannel()   { SCC_DisableChannel(this->serial); }

int KinesisStepMotorAxis::getPosition()    { return SCC_GetPosition(this->serial); }
int KinesisStepMotorAxis::getStatus()      { return SCC_GetStatusBits(this->serial); }

void KinesisStepMotorAxis::moveRelative(double position)      { SCC_MoveRelative(this->serial, position); }
void KinesisStepMotorAxis::moveToPosition(double position)    { SCC_MoveToPosition(this->serial, position); }

bool KinesisStepMotorAxis::canHome()    { SCC_CanHome(this->serial); }
void KinesisStepMotorAxis::home()       { SCC_Home(this->serial); }

int KinesisStepMotorAxis::stopImmediate()    { SCC_StopImmediate(this->serial); }

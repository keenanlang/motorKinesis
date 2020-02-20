#include <stdlib.h>
#include <conio.h>
#include <iocsh.h>
#include <epicsExport.h>

#include "drvKinesis.h"

#include "Thorlabs.MotionControl.KCube.DCServo.h"
#include "Thorlabs.MotionControl.KCube.StepperMotor.h"

KinesisController::KinesisController(const char* asyn_port, int serial, int type, double movingPollPeriod, double idlePollPeriod)
	: asynMotorController(asyn_port,
	                      1,
	                      0,
	                      0,
	                      0,
	                      ASYN_CANBLOCK | ASYN_MULTIDEVICE,
	                      1,
	                      0,
	                      0)
{
	if      (type == THOR_LABS_DC_MOTOR)    { new ThorLabsDCMotorAxis(this, 0, serial); }
	else if (type == THOR_LABS_STEP_MOTOR)  { new ThorLabsStepMotorAxis(this, 0, serial); }
	
	startPoller(movingPollPeriod, idlePollPeriod, 2);
}

KinesisMotorAxis* KinesisController::getAxis(asynUser* pasynUser)
{ 
	return static_cast<KinesisMotorAxis*>(asynMotorController::getAxis(pasynUser));
}

KinesisMotorAxis* KinesisController::getAxis(int axis)
{ 
	return static_cast<KinesisMotorAxis*>(asynMotorController::getAxis(axis)); 
}


KinesisMotorAxis::KinesisMotorAxis(KinesisController* pc, int axisNo, int serial_no)
	: asynMotorAxis(pc, axisNo),
	  pC_(pc)
{
	sprintf_s(this->serial, "%d", serial_no);
	
	TLI_BuildDeviceList();
	
	int success = this->connect();
	
	if(success == 0)
    {
		// start the device polling at 200ms intervals
        this->startPoll(200);
		this->enableChannel();
	}
	else
	{
		printf("Error Connecting: %d\n", success);
	}
	
	setDoubleParam(pC_->motorEncoderPosition_, 0.0);
	callParamCallbacks();
}

KinesisMotorAxis::~KinesisMotorAxis()
{
	this->disableChannel();
	this->stopPoll(); 
	this->disconnect();
}

asynStatus KinesisMotorAxis::move(double position, int relative, double minVelocity, double maxVelocity, double acceleration)
{	
	if (relative)    { this->moveRelative(position); }
	else             { this->moveToPosition(position); }
	
	return asynSuccess;
}

asynStatus KinesisMotorAxis::home(double minVelocity, double maxVelocity, double acceleration, int forwards)
{
	if (this->canHome())    { this->home();	}
	
	return asynSuccess;
}

asynStatus KinesisMotorAxis::poll(bool* moving)
{
	int pos = this->getPosition();
	int status = this->getStatus();
		
	setDoubleParam(this->pC_->motorPosition_, (double)pos);
	
	if ((status & 0x00000010) | (status & 0x00000020))
	{
		setIntegerParam(this->pC_->motorStatusMoving_, 1);
		setIntegerParam(this->pC_->motorStatusDirection_, (status & 0x00000010) ? 0 : 1);
		setIntegerParam(this->pC_->motorStatusDone_, 0);
		*moving = true;
	}
	else
	{
		setIntegerParam(this->pC_->motorStatusMoving_, 0);
		setIntegerParam(this->pC_->motorStatusDone_, 1);
		*moving = false;
	}
	
	setIntegerParam(this->pC_->motorStatusLowLimit_, (status & 0x00000001) ? 1 : 0);
	setIntegerParam(this->pC_->motorStatusHighLimit_,  (status & 0x00000002) ? 1 : 0);
	setIntegerParam(this->pC_->motorStatusHome_,  (status & 0x00000200) ? 1 : 0);
	setIntegerParam(this->pC_->motorStatusHomed_, (status & 0x00000400) ? 1 : 0);
	
	callParamCallbacks();
	
	return asynSuccess;
}

asynStatus KinesisMotorAxis::stop(double acceleration)
{
	this->stopImmediate();
	
	return asynSuccess;
}

/*
 * DC Motor Implementation
 */
int KinesisDCMotorAxis::connect()        { return CC_Open(this->serial); }
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


/*
 * Stepper Motor Implementation
 */
int KinesisStepMotorAxis::connect()        { return SCC_Open(this->serial); }
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



extern "C" void KinesisControllerConfig(const char* asyn_port, int serial, const char* type, double movingPollPeriod, double idlePollPeriod)
{
	std::string motor_type(type);
	
	if (motor_type == "dc" || motor_type == "DC")
	{ 
		new KinesisController(asyn_port, serial, KINESIS_DC_MOTOR, movingPollPeriod, idlePollPeriod);
	}
	else if (motor_type == "stepper" || motor_type == "Stepper" ||
	         motor_type == "step" || motor_type == "Step")
	{
		new KinesisController(asyn_port, serial, KINESIS_STEP_MOTOR, movingPollPeriod, idlePollPeriod);
	}
	else
	{
		printf("Unknown motor type: %s\n", type);
	}
	
	return asynSuccess;
}

static const iocshArg KinesisControllerArg0 = {"Motor Port Name", iocshArgString};
static const iocshArg KinesisControllerArg1 = {"Serial Number", iocshArgInt};
static const iocshArg KinesisControllerArg2 = {"Motor Type", iocshArgString};
static const iocshArg KinesisControllerArg3 = {"movingPollPeriod", iocshArgDouble};
static const iocshArg KinesisControllerArg4 = {"idlePollPeriod", iocshArgDouble};

static const iocshArg* const KinesisControllerArgs[] = {&KinesisControllerArg0, &KinesisControllerArg1, &KinesisControllerArg2, &KinesisControllerArg3, &KinesisControllerArg4};

static const iocshFuncDef KinesisControllerDef = {"KinesisControllerConfig", 5, KinesisControllerArgs};

static void KinesisControllerCallFunc(const iocshArgBuf* args)
{
	KinesisControllerConfig(args[0].sval, args[1].ival, args[2].sval, args[3].dval, args[4].dval);
}

static void KinesisRegister(void)
{
	iocshRegister(&KinesisControllerDef, KinesisControllerCallFunc);
}

extern "C"
{
	epicsExportRegistrar(KinesisRegister);
}
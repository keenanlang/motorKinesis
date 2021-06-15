#ifndef INC_DRVKINESIS_H
#define INC_DRVKINESIS_H

#include "asynMotorController.h"
#include "asynMotorAxis.h"

enum
{
	KINESIS_DC_MOTOR,
	KINESIS_STEP_MOTOR,
};

class epicsShareClass KinesisController;

class epicsShareClass KinesisAxis : public asynMotorAxis
{
	public:
		KinesisAxis(KinesisController* controller, int axisNo, int serialNo);
		~KinesisAxis();
		
		asynStatus move(double position, int relative, double min_velocity, double max_velocity, double acceleration);
		asynStatus home(double min_velocity, double max_velocity, double acceleration, int forwards);
		asynStatus stop(double acceleration);
		asynStatus poll(bool* moving);
		
		virtual int connect();
		virtual void disconnect();
		
		virtual void startPoll(int interval);
		virtual void stopPoll();
		
		virtual void enableChannel();
		virtual void disableChannel();
		
		virtual int getPosition();
		virtual int getStatus();
		
		virtual void moveRelative(double position);
		virtual void modeToPosition(double position);
		
		virtual bool canHome();
		virtual void home();
		
		virtual int stopImmediate();
		
	private:
		KinesisController* pC_;
		
		int axisIndex_;
		
		char serial[16];
};

class epicsShareClass KinesisDCMotorAxis : public KinesisAxis
{
	public:
		KinesisDCMotorAxis(KinesisController* controller, int axisNo, int serialNo);
		~KinesisDCMotorAxis();
		
		int connect();
		void disconnect();
		
		void startPoll(int interval);
		void stopPoll();
		
		void enableChannel();
		void disableChannel();
		
		int getPosition();
		int getStatus();
		
		void moveRelative(double position);
		void modeToPosition(double position);
		
		bool canHome();
		void home();
		
		int stopImmediate();
};

class epicsShareClass KinesisStepMotorAxis : public KinesisAxis
{
	public:
		KinesisStepMotorAxis(KinesisController* controller, int axisNo, int serialNo);
		~KinesisStepMotorAxis();
		
		int connect();
		void disconnect();
		
		void startPoll(int interval);
		void stopPoll();
		
		void enableChannel();
		void disableChannel();
		
		int getPosition();
		int getStatus();
		
		void moveRelative(double position);
		void modeToPosition(double position);
		
		bool canHome();
		void home();
		
		int stopImmediate();
};

class epicsShareClass KinesisController : public asynMotorController
{
	public:
		KinesisController(const char* portName, int serialNo, int type, double movingPollPeriod, double idlePollPeriod);
		KinesisAxis* getAxis(asynUser* pasynuser);
		KinesisAxis* getAxis(int axis);
		
	friend class KinesisAxis;
};


#endif

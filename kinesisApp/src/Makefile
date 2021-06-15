# Makefile
TOP = ../..
include $(TOP)/configure/CONFIG

DBD += devKinesis.dbd

LIBRARY_IOC = ThorLabsUSB

USR_INCLUDES = -I"C:/Program Files/Thorlabs/Kinesis"

SRCS += drvKinesis.cpp
SRCS += drvKinesisStepper.cpp
SRCS += drvKinesisDC.cpp

ThorLabsUSB_LIBS += Thorlabs.MotionControl.KCube.DCServo
ThorLabsUSB_LIBS += Thorlabs.MotionControl.KCube.StepperMotor
ThorLabsUSB_LIBS += motor asyn
ThorLabsUSB_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES


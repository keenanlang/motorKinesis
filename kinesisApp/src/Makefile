# Makefile
TOP = ../..
include $(TOP)/configure/CONFIG

DBD += devKinesis.dbd

LIBRARY_IOC = ThorLabsUSB

USR_INCLUDES = -I$(THORLABS_DIR)

SRCS += drvKinesis.cpp
SRCS += drvKinesisStepper.cpp
SRCS += drvKinesisDC.cpp

Thorlabs.MotionControl.KCube.DCServo_DIR += $(THORLABS_DIR)
ThorLabsUSB_DLL_LIBS += Thorlabs.MotionControl.KCube.DCServo

Thorlabs.MotionControl.KCube.StepperMotor_DIR += $(THORLABS_DIR)
ThorLabsUSB_DLL_LIBS += Thorlabs.MotionControl.KCube.StepperMotor

ThorLabsUSB_LIBS += motor asyn
ThorLabsUSB_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES


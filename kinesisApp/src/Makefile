# Makefile
TOP = ../..
include $(TOP)/configure/CONFIG

DBD += devKinesis.dbd

LIBRARY_IOC = ThorLabsUSB

SRCS += drvKinesis.cpp

ThorLabsUSB_LIBS += KDC101
ThorLabsUSB_LIBS += KST101
ThorLabsUSB_LIBS += motor asyn
ThorLabsUSB_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES


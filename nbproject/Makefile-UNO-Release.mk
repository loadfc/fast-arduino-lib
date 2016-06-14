#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=avr-gcc
CCC=avr-g++
CXX=avr-g++
FC=gfortran
AS=avr-as

# Macros
CND_PLATFORM=Arduino-1.6.9-Windows
CND_DLIB_EXT=dll
CND_CONF=UNO-Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cores/fastarduino/Events.o \
	${OBJECTDIR}/cores/fastarduino/LinkedList.o \
	${OBJECTDIR}/cores/fastarduino/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-mmcu=${MCU} -DF_CPU=${F_CPU} -D${VARIANT} -DARDUINO=${ARDUINO} -fno-exceptions -Wextra -flto -std=gnu++11 -felide-constructors -Os -ffunction-sections -fdata-sections -mcall-prologues
CXXFLAGS=-mmcu=${MCU} -DF_CPU=${F_CPU} -D${VARIANT} -DARDUINO=${ARDUINO} -fno-exceptions -Wextra -flto -std=gnu++11 -felide-constructors -Os -ffunction-sections -fdata-sections -mcall-prologues

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfastarduino.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfastarduino.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfastarduino.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfastarduino.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfastarduino.a

${OBJECTDIR}/cores/fastarduino/Events.o: nbproject/Makefile-${CND_CONF}.mk cores/fastarduino/Events.cpp 
	${MKDIR} -p ${OBJECTDIR}/cores/fastarduino
	${RM} "$@.d"
	$(COMPILE.cc) -Wall -Icores -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cores/fastarduino/Events.o cores/fastarduino/Events.cpp

${OBJECTDIR}/cores/fastarduino/LinkedList.o: nbproject/Makefile-${CND_CONF}.mk cores/fastarduino/LinkedList.cpp 
	${MKDIR} -p ${OBJECTDIR}/cores/fastarduino
	${RM} "$@.d"
	$(COMPILE.cc) -Wall -Icores -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cores/fastarduino/LinkedList.o cores/fastarduino/LinkedList.cpp

${OBJECTDIR}/cores/fastarduino/main.o: nbproject/Makefile-${CND_CONF}.mk cores/fastarduino/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/cores/fastarduino
	${RM} "$@.d"
	$(COMPILE.cc) -Wall -Icores -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cores/fastarduino/main.o cores/fastarduino/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libfastarduino.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

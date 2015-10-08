SET (project_SRC_DIR ${PROJECT_SOURCE_DIR}/src) 
#Main executable source files
FILE (GLOB_RECURSE project_SRCS
	${project_SRC_DIR}/*.cpp
	${project_SRC_DIR}/*.cxx
	${project_SRC_DIR}/*.cc
	${project_SRC_DIR}/*.C
	${project_SRC_DIR}/*.c
	${project_SRC_DIR}/*.h
	${project_SRC_DIR}/*.hpp)
#Exclude main of the application
list(REMOVE_ITEM project_SRCS ${project_SRC_DIR}/WolvesAI.cpp) 

FILE (GLOB_RECURSE test_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
IF(${USE_OPENCV})
	SET (test_LIBS ${CPPUNIT_LIBRARIES} ${JPEG_LIBRARIES} ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}${OpenCV_LIBS})
ELSE()
	SET (test_LIBS ${CPPUNIT_LIBRARIES} ${JPEG_LIBRARIES} ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
ENDIF(${USE_OPENCV})
SET (test_BIN ${PROJECT_NAME}-unittests)

PROTOBUF_GEN_CPP(PROTO_SRCS PROTO_HDRS ${MAINFOLDER}/src/Messages ${MAINFOLDER}/src/Messages)

ADD_EXECUTABLE(${test_BIN} ${test_SRCS} ${project_SRCS} ${PROTO_SRCS} ${PROTO_HDRS})
TARGET_LINK_LIBRARIES(${test_BIN} ${test_LIBS})

#Automatically execute unit tests after the build is complete
#ADD_CUSTOM_TARGET(check ALL "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
#Add custom target "make test" to execute unit tests
ADD_CUSTOM_TARGET(test "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})



#
# PC-Lint
#LINT binary
SET (lint_BIN LIN)
#LINT files to check --> LNT-file
SET (lint_FILES_LNT lint-files.lnt)

#Filter out protobuf sources

SET (lint_SRCS )
FOREACH(item ${project_SRCS})
  IF(NOT item MATCHES "\\.pb\\.(cpp|cc)")
    LIST(APPEND lint_SRCS ${item})
  ENDIF(NOT item MATCHES "\\.pb\\.(cpp|cc)")
ENDFOREACH(item ${project_SRCS})

#Delete old lint file
FILE(REMOVE ${lint_FILES_LNT})
#Write lint source files to LNT-file
FOREACH(source_file ${lint_SRCS})
  FILE(APPEND ${lint_FILES_LNT} \"${source_file}\" " ")
ENDFOREACH(source_file ${project_SRCS})

ADD_CUSTOM_TARGET(lint ${lint_BIN} lint.lnt ${LINT_CDEFS} ${lint_FILES_LNT})

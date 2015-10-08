#
# Protobuf proto files
#
function(PROTOBUF_GEN_CPP SRCS HDRS PROTO_PATH DIST_PATH)
	FILE (GLOB_RECURSE PROTO_PROTOS ${PROTO_PATH}/*.proto)
	if(NOT PROTO_PROTOS)
	    message(SEND_ERROR "Error: PROTOBUF_GEN_CPP() called without any proto files (PROTO_PROTOS is empty)")
	    return()
  	endif()

	set(${SRCS})
	set(${HDRS})
	foreach(PROTO_FILE ${PROTO_PROTOS})
		get_filename_component(ABS_FIL ${PROTO_FILE} ABSOLUTE)
		get_filename_component(FIL_WE ${PROTO_FILE} NAME_WE)

		list(APPEND ${SRCS} "${DIST_PATH}/${FIL_WE}.pb.cc")
		list(APPEND ${HDRS} "${DIST_PATH}/${FIL_WE}.pb.h")

		add_custom_command(
		  OUTPUT "${DIST_PATH}/${FIL_WE}.pb.cc"
		         "${DIST_PATH}/${FIL_WE}.pb.h"
		  PRE_BUILD
		  COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
		  ARGS --cpp_out ${DIST_PATH} ${PROTO_FILE} -I ${PROTO_PATH}
		  DEPENDS ${PROTO_FILE}
		  COMMENT "Running C++ protocol buffer compiler on ${PROTO_FILE}"
		  VERBATIM )
	endforeach()
	set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  	set(${HDRS} ${${HDRS}} PARENT_SCOPE)
  	set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
endfunction()
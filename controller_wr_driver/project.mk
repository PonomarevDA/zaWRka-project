ROSLIB = ./ros_lib
include $(ROSLIB)/ros.mk

PROJECT_MODULES = src/lld_control.c        			\
				  src/remote_control.c				\
				  src/lld_encoder.c					\
				  src/lld_odometry.c				\
				  src/lld_steer_angle_fb.c			\
				  src/drive_cs.c					\
				  src/lld_start_button.c			\
				  src/main_control_unit.c			\
				  src/lld_light.c 					\
				  src/max7219.c 					\
				  
PROJECT_TESTS   = tests/test_lld_control.c     		\
				  tests/test_remote_control.c		\
				  tests/test_drive_cs.c				\
				  tests/test_lld_encoder.c			\
				  tests/test_lld_odometry.c			\
				  tests/test_lld_steer_angle_fb.c	\
				  tests/test_ros.c					\
				  tests/test_gui_server.c			\
				  tests/test_ros_odometry.c			\
				  tests/test_ros_control.c			\
				  tests/test_lld_start_button.c		\
				  tests/test_lld_light.c 			\
				  		
PROJECT_CSRC    = src/main.c src/common.c src/debug.c src/usbcfg.c \
    				$(PROJECT_MODULES) $(PROJECT_TESTS)

PROJECT_CPPSRC 	= $(ROSSRC) src/ros.cpp

PROJECT_INCDIR	= include tests $(ROSINC)

PROJECT_LIBS	= -lm

PROJECT_OPTS	= -specs=nosys.specs


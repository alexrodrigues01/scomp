################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main\ .c 

OBJS += \
./main\ .o 

C_DEPS += \
./main\ .d 


# Each subdirectory must supply rules for building sources it contributes
main\ .o: ../main\ .c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"main .d" -MT"main\ .d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



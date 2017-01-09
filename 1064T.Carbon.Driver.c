#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, in2,    gyro2,          sensorGyro)
#pragma config(Sensor, in3,    clawSense,      sensorPotentiometer)
#pragma config(Sensor, in4,    liftSense,      sensorPotentiometer)
#pragma config(Sensor, in5,    subPower,       sensorAnalog)
#pragma config(Sensor, dgtl1,  ledArcade,      sensorLEDtoVCC)
#pragma config(Sensor, dgtl2,  ledTank,        sensorLEDtoVCC)
#pragma config(Sensor, dgtl3,  lcdOn,          sensorTouch)
#pragma config(Sensor, dgtl4,  lcdOff,         sensorTouch)
#pragma config(Sensor, dgtl5,  hangGuide,      sensorTouch)
#pragma config(Sensor, I2C_1,  leftDrive,      sensorNone)
#pragma config(Sensor, I2C_2,  rightDrive,     sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           topRight,      tmotorVex393HighSpeed_HBridge, openLoop, driveRight)
#pragma config(Motor,  port2,           midRight,      tmotorVex393HighSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port3,           bottomRight,   tmotorVex393HighSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port4,           clawLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           clawRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           yLiftRight,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           yLiftLeft,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           bottomLeft,    tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port9,           midLeft,       tmotorVex393HighSpeed_MC29, openLoop, driveLeft, encoderPort, I2C_2)
#pragma config(Motor,  port10,          topLeft,       tmotorVex393HighSpeed_HBridge, openLoop, driveLeft)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "1064T.Carbon.Methods.c"

task userDisplay(){
	resetLcd();
	while(true){
		if (nLCDButtons == 1){
			driveCount--;
			resetLcd();
			while(nLCDButtons == 1)(wait1Msec(10));
		}
		if(nLCDButtons == 4){
			driveCount++;
			resetLcd();
			while(nLCDButtons == 4)(wait1Msec(10));
		}
		if(driveCount < 1){
			driveCount = 10;
		}
		else if(driveCount > 10){
			driveCount = 1;
		}
		switch (driveCount){
		case 1:
			displayBatteryValues();
			break;
		case 2:
			gyroValue();
			break;
		case 3:
			nothing();
			break;
		case 4:
			nothing();
			break;
		case 5:
			nothing();
			break;
		case 6:
			nothing();
			break;
		case 7:
			nothing();
			break;
		case 8:
			nothing();
			break;
		case 9:
			nothing();
			break;
		case 10:
			nothing();
			break;
		}
		time(10);
	}
}

void pre_auton(){
	resetGyro();
	resetLcd();
	selectAuton();
}

task autonomous(){
	lcdAuton();
}

task usercontrol(){

	bool useTank = false;

	resetLcd();

	startTask(userDisplay);

	if(useTank){ //LED driver type indicater
		turnLEDOn(ledTank);
		turnLEDOff(ledArcade);
		}else{
		turnLEDOn(ledArcade);
		turnLEDOff(ledTank);
	}

	while(true){

		if(vexRT[Btn7U]){
			if(useTank){ //Drive type indicater
				useTank = false;
				turnLEDOn(ledArcade);
				turnLEDOff(ledTank);
				}else{
				useTank = true;
				turnLEDOn(ledTank);
				turnLEDOff(ledArcade);
			}
			wait1Msec(200);
		}
		if(vexRT[Btn7D]){
			stopDrive(10);
		}
		if(vexRT[Btn7L]){
			autonClaw(2000);
		}
		if(vexRT[Btn7R]){
			autonClaw(4000);
		}
		if(vexRT[Btn8U]){
			autonLift(3000);
		}
		if(vexRT[Btn8D]){
			autonLift(1000);
		}
		if(vexRT[Btn8L]){
			//empty
		}
		if(vexRT[Btn8R]){
			//empty
		}

		if(useTank){ //Tank drive
			int joyRight = vexRT[Ch2];
			int joyLeft  = vexRT[Ch3];
			drive(joyLeft, -joyRight);
			}else{ //Arcade drive
			int joy_x = vexRT[Ch1];
			int joy_y = vexRT[Ch2];

			motor[topRight]       =  joy_x - joy_y;
			motor[midRight]       = -joy_x + joy_y;
			motor[bottomRight]    =  joy_x - joy_y;
			motor[topLeft]        =  joy_x + joy_y;
			motor[midLeft]        = -joy_x - joy_y;
			motor[bottomLeft]     =  joy_x + joy_y;

		}

		if(vexRT[Btn5U]){ //Claw control, self locks if not use
			claw(127);
		}
		else if(vexRT[Btn5D]){
			claw(-127);
		}
		else if(vexRT[Btn5U] != true && vexRT[Btn5D] != true){
			autonClaw(clawGrip);
		}

		if(vexRT[Btn6U]){ //Lift control, self locks if not in use
			lift(127);
		}
		else if(vexRT[Btn6D]){
			lift(-127);
		}
		else if(vexRT[Btn8U] != true && vexRT[Btn8D] != true){
			autonLift(liftHeight);
		}
	}
}

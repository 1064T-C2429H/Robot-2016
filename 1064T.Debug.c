#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    Gyro,           sensorGyro)
#pragma config(Sensor, in2,    Gyro2,          sensorGyro)
#pragma config(Sensor, in3,    xAccel,         sensorAccelerometer)
#pragma config(Sensor, in4,    yAccel,         sensorAccelerometer)
#pragma config(Sensor, in5,    zAccel,         sensorAccelerometer)
#pragma config(Sensor, dgtl1,  ledTank,        sensorLEDtoVCC)
#pragma config(Sensor, dgtl2,  ledArcade,      sensorLEDtoVCC)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           leftFront,     tmotorVex393_HBridge, openLoop, driveLeft)
#pragma config(Motor,  port2,           rightFront,    tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port3,           leftBack,      tmotorVex393_MC29, openLoop, driveLeft, encoderPort, I2C_4)
#pragma config(Motor,  port4,           rightBack,     tmotorVex393_MC29, openLoop, reversed, driveRight, encoderPort, I2C_3)
#pragma config(Motor,  port5,           YRightLift,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           RightLift,     tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port7,           YLeftLift,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           LeftLift,      tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port9,           ClawLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          ClawRight,     tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"

//init global varibals
int GLOBAL_display = 0;

int errorPrior = 0;
int PID_Integral = 0;
float PID_P = 0;
float PID_I = 0;
float PID_D = 0;

long delta = 0;

const int GLOBAL_DOUBLE_CLICK_SPEED = 250;
const short lcdRightButton = 1;
const short lcdCenterButton = 2;
const short lcdLeftButton = 4;

// driver methods
// drive Tank code method
void driveTank(int joyLeft, int joyRight){
	motor[leftFront] = vexRT[Ch3];
	motor[rightFront] = vexRT[Ch2];
	motor[leftBack] = vexRT[Ch3];
	motor[rightBack] = vexRT[Ch2];
}

//drive arcade mode method
void driveArcade(int joy){
	motor[leftFront] = joy;
	motor[rightFront] = joy;
	motor[leftBack] = joy;
	motor[rightBack] = joy;
}

void driveForward(float time){ //TODO: PID LOOP
	motor[leftFront] = 127;
	motor[rightFront] = 127;
	motor[leftBack] = 127;
	motor[rightBack] = 127;
	wait1Msec(time);
	motor[leftFront] = 0;
	motor[rightFront] = 0;
	motor[leftBack] = 0;
	motor[rightBack] = 0;
}

void driveBackward(float time){ //TODO: PID LOOP
	motor[leftFront] = -127;
	motor[rightFront] = -127;
	motor[leftBack] = -127;
	motor[rightBack] = -127;
	wait1Msec(time);
	motor[leftFront] = 0;
	motor[rightFront] = 0;
	motor[leftBack] = 0;
	motor[rightBack] = 0;
}
//lift method raise
void lift(int power){
	motor[YRightLift] = -power;
	motor[RightLift] = -power;
	motor[YLeftLift] = power;
	motor[LeftLift] =  power;
}
//claw method
void claw(int power){
	motor[ClawLeft] = power;
	motor[ClawRight] = power;
}

//Wait for Press
void waitForPress(){
	while(nLCDButtons == 0){}
	wait1Msec(5);
}
//Wait for Release
void waitForRelease(){
	while(nLCDButtons != 0){}
	wait1Msec(5);
}

void resetGyro(){
	//gyro config
	//Completely clear out any previous sensor readings by setting the port to "sensorNone"
	SensorType[Gyro] = sensorNone;
	SensorType[Gyro2] = sensorNone;
	wait1Msec(500);
	//Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
	SensorType[Gyro] = sensorGyro;
	SensorType[Gyro2] = sensorGyro;
	wait1Msec(1500);
}
void turnByGyro(int degrees2){
	//Adjust SensorScale to correct the scaling for your gyro
	//SensorScale[in8] = 260;
	//Adjust SensorFullCount to set the "rollover" point. A value of 3600 sets the rollover point to +/-3600
	//SensorFullCount[in8] = 3600;

	//Specify the number of degrees for the robot to turn (1 degree = 10, or 900 = 90 degrees)
	int degrees10 = degrees2;
	//Specify the amount of acceptable error in the turn
	int error = 5;

	//While the absolute value of the gyro is less than the desired rotation - 100...
	while(SensorValue[Gyro] < degrees10 - 100){
		motor[rightFront] = 127;
		motor[leftFront] = -127;
		motor[rightBack] = 127;
		motor[leftBack] = -127;
	}
	//Brief brake to eliminate some drift
	motor[rightFront] = -5;
	motor[leftFront] = 5;
	motor[rightBack] = -5;
	motor[leftBack] = 5;
	wait1Msec(100);

	//Second while loop to move the robot more slowly to its goal, also setting up a range
	//for the amount of acceptable error in the system
	while(SensorValue[Gyro] > degrees10 + error || SensorValue[Gyro] < degrees10 - error){
		if(SensorValue[Gyro] > degrees10){
			motor[rightFront] = -100;
			motor[leftFront] = 100;
			motor[rightBack] = -100;
			motor[leftBack] = 100;
		}else{
			motor[rightFront] = 100;
			motor[leftFront] = -100;
			motor[rightBack] = 100;
			motor[leftBack] = -100;
		}
	}
	//Stop
	motor[rightFront] = 0;
	motor[leftFront] = 0;
	motor[rightBack] = 0;
	motor[leftBack] = 0;
	wait1Msec(250);
}


void pre_auton(){
	resetGyro();
}

int pid(int desired, int actual, int bias){
	//PID
	int error = desired - actual;
	PID_Integral = PID_Integral + (error*1);
	int drivative = (error - errorPrior)/1;
	int output = PID_P*error + PID_I*PID_Integral + PID_D*drivative + bias;
	errorPrior = error;
	return output;
}

task autonomous(){
	bool running = false;
	startTask(lcdDisplay);
	while(running){

		//stop the auton
		running = false;
	}
}

task usercontrol(){
	bool useTank = true;

	long lastTime = nSysTime;

	if(!useTank){
		turnLEDOn(dgtl2);
		turnLEDOff(dgtl1);
	}else{
		turnLEDOn(dgtl1);
		turnLEDOff(dgtl2);
	}


	while(true){
		long time = nSysTime;
		delta = time - lastTime;
		lastTime = time;

		if(vexRT[Btn7U]){
			if(useTank){
				useTank = false;
				turnLEDOn(dgtl2);
				turnLEDOff(dgtl1);
			}else{
				useTank = true;
				turnLEDOn(dgtl1);
				turnLEDOff(dgtl2);
			}
			wait1Msec(200);
		}

		if(vexRT[Btn6U]){ // Lift up
			lift(127);
		}else if(vexRT[Btn5U]){// Lift down
			lift(-127);
		}else{
			lift(0);
		}

		if(vexRT[Btn6D]){ // Open claw
			claw(127);
		}else if(vexRT[Btn5D]){ // Close claw
			claw(-127);
		}else{
			claw(0);
		}

		if(useTank){
			int joyRight = vexRT[Ch2];
			int joyLeft = vexRT[Ch3];
			driveTank(pid(joyRight, nMotorEncoder[rightBack], 1), pid(joyLeft, nMotorEncoder[leftBack], 1));
			//driveTank(pid(joyRight, nMotorEncoder[rightBack], 1), pid(joyLeft, nMotorEncoder[leftBack], 1));
		}else{
			int joyX = vexRT[Ch1];
			int joyY = vexRT[Ch2];

			int leftDesired = joyY + joyX;
			int rightDesired = joyY - joyX;

			motor[leftFront] = leftDesired;
			motor[leftBack] = leftDesired;
			motor[rightFront] = rightDesired;
			motor[rightBack] = rightDesired;

			//motor[leftFront] = pid(leftDesired, nMotorEncoder[leftBack], 1);
			//motor[leftBack] = pid(leftDesired, nMotorEncoder[leftBack], 1);
			//motor[rightFront] = pid(rightDesired, nMotorEncoder[rightBack], 1);
			//motor[rightBack] = pid(rightDesired, nMotorEncoder[rightBack], 1);
		}
		wait1Msec(1);
	}
}

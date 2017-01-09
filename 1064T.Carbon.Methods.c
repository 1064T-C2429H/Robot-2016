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

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"

//----///////////////////////////////GLOBALS/////////////////////////////////////////////////----

int liftHeight = SensorValue[liftSense];
int clawGrip   = SensorValue[clawSense];

//--------------------LCD variables-------------------------------
//Declare count variable to keep track of our choice
int driveCount = 1; //drivemodecount
int autonSelec = 0; //auton mode
//lcd buttons
const short leftButton   = 1;
const short centerButton = 2;
const short rightButton  = 4;

//----///////////////////////////////DRIVER METHODS///////////////////////////////////////////----

void time(int Msecs){//This method pauses code for a specified time
	wait1Msec(Msecs);
}

void lift(int power){//User csontrol method controls the robots lift (positive power raises the lift
	motor[yLiftRight] =  power;
	motor[yLiftLeft]  = -power;

}

void claw(int power) { //claw controls
	motor[clawLeft]  =  power;
	motor[clawRight] = -power;
}

void autonClaw(int angle){
	int error = 10;
	if(SensorValue[clawSense] < angle - error){
		claw(-127);
	}
	else if(SensorValue[clawSense] > angle + error){
		claw(127);
	}
	else{
		claw(-10);
	}
}

void autonLift(int height){
	int error = 10;
	if(SensorValue[liftSense] < height - error){
		lift(100);
	}
	else if(SensorValue[liftSense] > height + error){
		lift(-40);
	}
	else{
		lift(10);
	}
}
void drive(int left, int right){//Tank drive auton control method
	motor[topLeft]     =  left;
	motor[midLeft]     = -left;
	motor[bottomLeft]  =  left;
	motor[topRight]    =  right;
	motor[midRight]    = -right;
	motor[bottomRight] =  right;
}

void stopDrive(int timeLength){ //stop the drive
	motor[topLeft]     =  0;
	motor[midLeft]     =  0;
	motor[bottomLeft]  =  0;
	motor[topRight]    =  0;
	motor[midRight]    =  0;
	motor[bottomRight] =  0;
	time(timeLength);
}

void autoDrive(int speed, int dist){

}
//----///////////////////////////////SENSOR METHODS//////////////////////////////////////////----

void resetGyro(){ //resets the Gryos during the robots start up
	//Completely clear out any previous sensor readings by setting the port to "sensorNone"
	SensorType[in1] = sensorNone;
	SensorType[in2] = sensorNone;
	wait1Msec(500);

	//Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
	SensorType[in1] = sensorGyro;
	SensorType[in2] = sensorGyro;
	wait1Msec(1500);

}

void gyroValue(){
	displayLCDCenteredString(0,"Gyro Value");
	displayLCDNumber(1,5,(SensorValue[in1]+SensorValue[in2])/2,5);
}

void gyroTurn(int degrees2){

	//Specify the number of degrees for the robot to turn (1 degree = 10, or 900 = 90 degrees)
	int degrees10 = degrees2;
	//Specify the amount of acceptable error in the turn
	int error = 50;

	//While the absolute value of the gyro is less than the desired rotation - 100...
	while((SensorValue[in1] + SensorValue[in2])/2 < degrees10*.5)
	{
		motor[topLeft]     = -80;
		motor[midLeft]     =  80;
		motor[bottomLeft]  = -80;
		motor[topRight]    = -80;
		motor[midRight]    =  80;
		motor[bottomRight] = -80;
	}
	//Brief brake to eliminate some drift
	motor[topLeft]     =  10;
	motor[midLeft]     = -10;
	motor[bottomLeft]  =  10;
	motor[topRight]    =  10;
	motor[midRight]    = -10;
	motor[bottomRight] =  10;
	wait1Msec(100);

	//Second while loop to move the robot more slowly to its goal, also setting up a range
	//for the amount of acceptable error in the system
	while((SensorValue[in1] + SensorValue[in2])/2 > degrees10 + error || (SensorValue[in1] + SensorValue[in2])/2 < degrees10 - error)
	{
		if((SensorValue[in1] + SensorValue[in2])/2 > degrees10)
		{
			motor[topLeft]     =  50;
			motor[midLeft]     = -50;
			motor[bottomLeft]  =  50;
			motor[topRight]    =  50;
			motor[midRight]    = -50;
			motor[bottomRight] =  60;
		}
		else
		{
			motor[topLeft]     = -50;
			motor[midLeft]     =  50;
			motor[bottomLeft]  = -50;
			motor[topRight]    = -50;
			motor[midRight]    =  50;
			motor[bottomRight] = -50;
		}
	}
	//Stop
	motor[topLeft]     =  0;
	motor[midLeft]     =  0;
	motor[bottomLeft]  =  0;
	motor[topRight]    =  0;
	motor[midRight]    =  0;
	motor[bottomRight] =  0;
	wait1Msec(100);
}

//----///////////////////////////////Auton Selections////////////////////////////////////////----

//----//////////////////////////Blue Auton Selections////////////////////////////////////////----
void auton5(){
	wait(1);
}

void auton6(){
	wait(1);
}

void auton7(){
	wait(1);
}

void auton8(){
	wait(1);
}

//----///////////////////////////Red Auton Selections////////////////////////////////////////----

void auton10(){
	wait(1);
}

void auton11(){
	wait(1);
}

void auton12(){
	wait(1);
}

void auton13(){
	wait(1);
}

void displayBatteryValues(){ //displays power levels
	string mainBatteryVoltage, expanderBattery;
	displayLCDString(0,0,"Cor Power: ");
	displayLCDString(1,0,"Sub Power: ");
	sprintf(mainBatteryVoltage,"%1.2f%c",nImmediateBatteryLevel/1000.0,'V');
	sprintf(expanderBattery,"%1.2f%c",SensorValue[subPower]/283.2, 'V');
	displayLCDString(0,10,mainBatteryVoltage);
	displayLCDString(1,10,expanderBattery);
}

void resetLcd(){
	bLCDBacklight = true;
	//Clear LCD
	clearLCDLine(0);
	clearLCDLine(1);
}

void waitForPress(){//Pauses the lcd display until a button is pressed
	while(nLCDButtons == 0){}
	wait1Msec(5);
}

void waitForRelease(){//Pauses the lcd display until a button is released
	while(nLCDButtons != 0){}
	wait1Msec(5);
}

void selectAuton(){
	//Loop while center button is not pressed
	while(nLCDButtons != centerButton){
		switch(autonSelec){
		case 0:
			//Display first choice
			displayLCDCenteredString(0, "Playing as Blue?");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec = 1;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			else{
				waitForRelease();
				autonSelec=5;
			}
			break;
		case 1:
			//Display second choice
			displayLCDCenteredString(0, "Playing as  Red?");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec=0;
			}
			else{
				waitForRelease();
				autonSelec = 10;
			}
			break;
		case 5:
			displayLCDCenteredString(0, "1B) R fence/hang");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec=9;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			break;
		case 6:
			displayLCDCenteredString(0, "1B) L fence/hang");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			break;
		case 7:
			displayLCDCenteredString(0, "2B) R  Clear All");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			break;
		case 8:
			displayLCDCenteredString(0, "2B) L  Clear All");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			break;
		case 9:
			displayLCDCenteredString(0, "     Go Back    ");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec=5;
			}
			else{
				waitForRelease();
				autonSelec=0;
			}
			break;
		case 10:
			displayLCDCenteredString(0, "1R) R fence/hang");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec=14;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			break;
		case 11:
			displayLCDCenteredString(0, "1R) L fence/hang");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			break;
		case 12:
			displayLCDCenteredString(0, "2R) R  Clear All");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			break;
		case 13:
			displayLCDCenteredString(0, "2R) L  Clear All");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec++;
			}
			break;
		case 14:
			displayLCDCenteredString(0, "     Go Back    ");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				autonSelec--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				autonSelec=10;
			}
			else{
				waitForRelease();
				autonSelec=0;
			}
			break;
		default:
			autonSelec = 0;
			break;
		}
		time(10);
	}
}

void nothing(){ //Display emtpy slot on the lcd
	displayLCDCenteredString(0, "slot");
	displayLCDCenteredString(1, "here");
	time(10);
}
void lcdAuton(){ //autonomous selection on lcd
	resetLcd();
	//Switch Case that actually runs the user choice
	switch(autonSelec){
	case 5:
		//If count = 0, run the code correspoinding with choice 1
		displayLCDCenteredString(0, "1B) R fence/hang");
		displayLCDCenteredString(1, "   is running!  ");
		// First auton option
		auton5();
		break;
	case 6:
		//If count = 1, run the code correspoinding with choice 2
		displayLCDCenteredString(0, "1B) L fence/hang");
		displayLCDCenteredString(1, "   is running!  ");
		//second auton option
		auton6();
		break;
	case 7:
		//If count = 2, run the code correspoinding with choice 3
		displayLCDCenteredString(0, "2B) R Clear  All");
		displayLCDCenteredString(1, "   is running!  ");
		//third auton option
		auton7();
		break;
	case 8:
		//If count = 3, run the code correspoinding with choice 4
		displayLCDCenteredString(0, "2B) L Clear  All");
		displayLCDCenteredString(1, "   is running!  ");
		//fourth auton option
		auton8();
		break;
	case 10:
		//If count = 0, run the code correspoinding with choice 1
		displayLCDCenteredString(0, "1R) R fence/hang");
		displayLCDCenteredString(1, "   is running!  ");
		// First auton option
		auton10();
		break;
	case 11:
		//If count = 1, run the code correspoinding with choice 2
		displayLCDCenteredString(0, "1R) L fence/hang");
		displayLCDCenteredString(1, "   is running!  ");
		//second auton option
		auton11();
		break;
	case 12:
		//If count = 2, run the code correspoinding with choice 3
		displayLCDCenteredString(0, "2R) R Clear  All");
		displayLCDCenteredString(1, "   is running!  ");
		//third auton option
		auton12();
		break;
	case 13:
		//If count = 3, run the code correspoinding with choice 4
		displayLCDCenteredString(0, "2R) L Clear  All");
		displayLCDCenteredString(1, "   is running!  ");
		//fourth auton option
		auton13();
		break;
	}
}

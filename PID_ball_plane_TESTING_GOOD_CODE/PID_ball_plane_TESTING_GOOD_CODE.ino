#testing changes
#include <SPI.h>  
#include <Pixy.h>
#include <Servo.h>
#include<PID_v1.h>
#dogs
int xSPin = 5;
int ySPin = 6;  

int xmaxi = 260;
int xmini = 56;
int ymaxi = 173;
int ymini = 25;
int j;
int xkalibrator = 90; // Servo kuning 80
int ykalibrator = 90; // Servo Merah

int xSP[] = {90, 90, 210, 210};
int ySP[] = {64, 145, 145, 64};


double xSetpoint, xInput, xOutput, xServoSignal; 
double ySetpoint, yInput, yOutput, yServoSignal; 

float xKp = 1.0;   // 1.1    
float xKi = 0.05;  //.05  
float xKd = 0.3;   //.2
float yKp = .9;   //.9
float yKi = 0.05;  // .05
float yKd = 0.3;   //.2

PID xmyPID(&xInput, &xOutput, &xSetpoint, xKp, xKi, xKd, DIRECT);                                                               
PID ymyPID(&yInput, &yOutput, &ySetpoint, yKp, yKi, yKd, DIRECT);                                                                                                                                                                                                        

Servo xmyServo;
Servo ymyServo;
Pixy pixy;

void setup() {
  xSetpoint = 100;//  Sumbu X
  ySetpoint = 90; // sumbu Y
  
  Serial.begin(115200);
  xmyServo.attach(xSPin);
  ymyServo.attach(ySPin); 
  pixy.init();
  Serial.print("Siap...\n");
                                    
  xmyPID.SetMode(AUTOMATIC);                                   
  xmyPID.SetOutputLimits(-100,100);
  ymyPID.SetMode(AUTOMATIC);                                   
  ymyPID.SetOutputLimits(-100,100);
}

void loop() {
  static int i = 0;
  uint16_t blocks;
  char buf[32];  
  blocks = pixy.getBlocks();
    if (blocks) {
       i++;
      if (i%1==0) {
           for (j=0; j<blocks; j++) {           
              loopa();
           }        
       }
    } 
}

void loopa(){
  if (pixy.blocks[j].x <= xmaxi) {
      xInput = pixy.blocks[j].x;
      yInput = pixy.blocks[j].y;
      if (xInput > xmaxi) {
        xInput = xmaxi;
      }
      if (xInput < xmini){
        xInput = xmini;
      }
                                 
      xmyPID.Compute();                    
      xServoSignal=xkalibrator+xOutput; // +/- tergatung posisi tungkai pd servo
      xmyServo.write(xServoSignal); 
      ymyPID.Compute();                    
      yServoSignal=ykalibrator+yOutput; // +/- tergatung posisi tungkai pd servo
      ymyServo.write(yServoSignal);     
      
      Serial.println("Posisi x dan y : ");
      Serial.print(xInput);
      Serial.print(",");
      Serial.print(yInput);
      Serial.print(",");
      Serial.print(xServoSignal);
      Serial.print(",");
      Serial.println(yServoSignal);

   } 
}

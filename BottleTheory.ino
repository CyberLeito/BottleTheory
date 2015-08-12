

//                         attiny85
//                   reset -+---+- vcc
//            (Serial) pb3 -+*  +- pb2 (connected to led) C+, D-, E-, F+
//       (touch input) pb4 -+   +- pb1 (connected to led) A+, B-, C-, D+
//                  ground -+---+- pb0 (connected to LED) A-, B+, E+, F-  

int num = 0;
int pin1 = 0;  
int pin2 = 1; 
int pin3 = 2; 
int mode = 0;
int timer = 0;


int NUMOFMODES =4; // Number of modes

bool lasttouched = false;
bool fade = false;
bool justfaded = false;

int calibration = 0;
//int previous;

int randomval = 0;
int fadeval = 0, fadestep = 1;
int togglestate = LOW;

boolean touched = false;

///////////////////////////////////////////////////////////////////////////////////////

int allOn(){ 
 //for (;;) {
   lightR();
   lightB(0);
   lightC(0);
   delayMicroseconds(20); 
   lightR();
   lightE(0);
   delayMicroseconds(20);   
   lightR();
   lightA(0);
   lightD(0);
   delayMicroseconds(20); 
   lightR();
   lightF(0);
   delayMicroseconds(20); 
// }
}
///////////////////////////////////////////////////////////////////////////////////////

int lightB(int n){
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,LOW);
  delay(n);  
}

int lightD(int n){
  pinMode(pin3, OUTPUT);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin2,HIGH);
  digitalWrite(pin3,LOW);
  delay(n); 
}

int lightA(int n){
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin2,HIGH);
  digitalWrite(pin1,LOW);
  delay(n); 
}

int lightC(int n){
  pinMode(pin3, OUTPUT);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin3,HIGH);
  digitalWrite(pin2,LOW);
  delay(n);
}

int lightF(int n){
  pinMode(pin1, OUTPUT);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3,HIGH);
  digitalWrite(pin1,LOW);
  delay(n);  
}

int lightE(int n){
  pinMode(pin1, OUTPUT);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin1,HIGH);
  digitalWrite(pin3,LOW);
  delay(n);
}

int lightR(){
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT); 
}
///////////----------------------------------------------------------

#define NUM_LEDS 6 // Maximum with 3 output pins

#define CHARLIE(pin_vcc, pin_gnd) \
	{ 1 << (pin_vcc), 1 << (pin_gnd) }

static const uint8_t mux[NUM_LEDS][2] = {
	CHARLIE(1,2),
	CHARLIE(2,1),
	CHARLIE(0,1),
	CHARLIE(1,0),
	CHARLIE(2,0),
	CHARLIE(0,2),
};

void off()
{
	DDRB = 0;
	PORTB = 0;
}

void on(int n)
{
	uint8_t vcc = mux[n][0];
	uint8_t gnd = mux[n][1];
  
	// turn both to output
	PORTB = 0;
	DDRB = vcc | gnd;
	PORTB = vcc;
}

static uint8_t fb[NUM_LEDS];

//-------------------------------------------------------------------

void setup() {
// Serial.begin(9600); 
// Serial.println("Initiating Serial");
  
pinMode(pin1, INPUT);
pinMode(pin2, INPUT);
pinMode(pin3, INPUT);


delay(100);
  for (int i = 0; i < 8; i++) {
    calibration += chargeTime(PB4);
    delay(20);
  }
  calibration = (calibration + 4) / 8;
}

void loop()
{
  //int OffValue = 1000;
  
  int n = chargeTime(PB4);
  
  if (n > calibration){ 
    touched = true;    
  }
  else {
    touched = false;
  }
  
  if (touched == true){
    //Serial.println("Touched");
     //MaskSwitch(random(6), 500);
     //poweroff=0;
    lasttouched = true;
    timer++;
    if(timer > 600){ 
      
      //Serial.println("Timer Exceeded");
      //delay(500);
       TouchToggleRun();
       if(justfaded==false){
        if (fade == false){
          fade= true;
          justfaded=true;
          //Serial.println("Fade is True");
        }
        else {
          fade= false;
          justfaded=true;
          
        }
      }
    }
     
     
  }
  else {
  justfaded=false;
  if(lasttouched == true){
    justfaded=false;
    timer = 0;
    if(fade == true){
      TouchToggleRun();
      delay(70);
      
    }
    else 
    {   
      lightR();
       if (mode == NUMOFMODES){ 
         mode = 0;
       }
        else
          mode ++;
        }
       
  lasttouched = false;
  if(fade == false){
    switch(mode){
    
      case 0:
        chase2();
        break;
      case 1:
        allOn();
        break;
      case 2:
        MaskSwitch(random(6), 200);
        break;     
    case 3:      
       RandomChase();
       break;      
   case 4:      
       lightR();
       break;    
    }
  }

}
  delayMicroseconds(500);
}
}

byte chargeTime(byte pin)
{
  byte mask = (1 << pin);
  byte i;

  DDRB &= ~mask; // input
  PORTB |= mask; // pull-up on

  for (i = 0; i < 16; i++) {
    if (PINB & mask) break;
  }

  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge

  return i;
} 

int TouchToggleRun(){
    if (num == 5)
      num = 0;
    else
      num ++;
   // delay(80); 
    MaskSwitch(num,80);
 }
    
int MaskSwitch(int n, int del) {    
  switch(n){
    
    case 0 :
    lightR();
    lightA(del);
    break;
    
    case 1 :
      lightR();
      lightB(del);
      break;
    
    case 2 :
      lightR();
      lightC(del);
      break;
    
    case 3 :
      lightR();
      lightD(del);
      break;
    
    case 4 :
      lightR();
      lightE(del);
      break;
    
    case 5 :
      lightR();
      lightF(del);
      break;
    
    }    
  }
  

void draw()
{
	for(uint8_t i = 0 ; i < 255 ; i++)
	{
		for (uint8_t j = 0 ; j < NUM_LEDS ; j++)
		{
			if (fb[j] > i)
				on(j);
			else
				off();
		}
	}
}


static void
decay(
	const uint8_t speed
)
{
	for(int i = 0 ; i < NUM_LEDS ; i++)
	{
		if (fb[i] > speed)
			fb[i] -= speed;
		else
			fb[i] = 0;
	}
}

void chase2()
{
	for(uint8_t i = 0 ; i < NUM_LEDS; i++)
	{
		for(uint8_t delay = 0 ; delay < 30 ; delay++)
		{
			if (fb[i] < 200)
				fb[i] += 16;
			draw();
			if ((delay & 3) == 0)
				decay(8);
		}
	}
}

void RandomChase()
{
	uint8_t i = random(7);
	{
		for(uint8_t delay = 0 ; delay < 30 ; delay++)
		{
			if (fb[i] < 200)
				fb[i] += 16;
			draw();
			if ((delay & 3) == 0)
				decay(8);
		}
	}
}

 

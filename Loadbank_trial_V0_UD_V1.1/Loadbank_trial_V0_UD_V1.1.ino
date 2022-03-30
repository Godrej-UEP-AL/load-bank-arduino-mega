#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>

const byte _start = 2;
const byte _stop = 3;
const byte _resume = 20;
const byte _pause = 21;

unsigned char DebounceCounter_start = 0;
unsigned char DebounceCounter_stop = 0;
unsigned char DebounceCounter_resume = 0;
unsigned char DebounceCounter_pause = 0;

#define _KEY_DEBOUNE_TIME_MS  50

unsigned long _previousmillis = 0;
File dataFile;
char _ldAry[1000] = {0};
int _executeLoop = 0;
int _executeSequence = 0;
void _extract(int);
unsigned int _GetNumber(char*);
int _repeat = 0;
unsigned int debounce_delay = 20;


int _NoOfLoops = 0; 
int _NoOfSequence = 0;
unsigned char filesize = 0;
unsigned char line = 0;
char *_DataPtr = 0;
char *_LoadPtr = 0;
char *_TimePtr = 0;
unsigned int _add = 0;
unsigned int _TimePtradd = 0;
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte _state = 0;

unsigned long TimeNow_mS_start = 0;
unsigned long TimeNow_mS_pause = 0;
unsigned long TimeNow_mS_resume = 0;
unsigned long TimeNow_mS_stop = 0;

bool ExecuteKey_start = false;
bool ExecuteKey_resume = false;
bool ExecuteKey_pause = false;
bool ExecuteKey_stop = false;

void ISR_start(){
//  if(ExecuteKey_start == false){
    if(!digitalRead(_start))
    {
      DebounceCounter_start = true;
      TimeNow_mS_start = millis();
    }
//  }
  Serial.println("Key 2 ISR");
}

void ISR_pause(){
//  if(ExecuteKey_pause == false){
    if(!digitalRead(_pause))
    {
    DebounceCounter_pause = true;
    TimeNow_mS_pause = millis();
    }
  Serial.println("Key 21 ISR");
//}
}

void ISR_resume(){
//  if(ExecuteKey_resume == false){
    if(!digitalRead(_resume))
    {
    DebounceCounter_resume = true;
    TimeNow_mS_resume = millis();
    }
  Serial.println("Key 20 ISR");
//  }
}

void ISR_stop(){
//  if(ExecuteKey_stop == false){
    if(!digitalRead(_stop))
    {
    DebounceCounter_stop = true;
    TimeNow_mS_stop = millis();
    }
  Serial.println("Key 3 ISR");
//  }
}

void KeyDetection(){
  if(DebounceCounter_start && !digitalRead(_start))
  {
    if(millis() - TimeNow_mS_start >= _KEY_DEBOUNE_TIME_MS)
    {
      DebounceCounter_start = 0;
      ExecuteKey_start = true;
    }    
  }
  else if(digitalRead(_start)){
    DebounceCounter_start = 0;
  }
  
  if(DebounceCounter_pause && !digitalRead(_pause))
  {
    if(millis() - TimeNow_mS_pause >= _KEY_DEBOUNE_TIME_MS)
    {
      DebounceCounter_pause = 0;
      ExecuteKey_pause = true;
      Serial.println("Pause function will be called");
    }    
  }
  else if(digitalRead(_pause)){
    DebounceCounter_pause = 0;
  }
  
  if(DebounceCounter_resume && !digitalRead(_resume))
  {
    if(millis() - TimeNow_mS_resume >= _KEY_DEBOUNE_TIME_MS)
    {
      DebounceCounter_resume = 0;
      ExecuteKey_resume = true;
      Serial.println("resume function will be called");
    }    
  }
  else if(digitalRead(_resume)){
    DebounceCounter_resume = 0;
  }

  if(DebounceCounter_stop && !digitalRead(_stop))
  {
    if(millis() - TimeNow_mS_stop >= _KEY_DEBOUNE_TIME_MS)
    {
      DebounceCounter_stop = 0;
      ExecuteKey_stop = true;
      Serial.println("Stop function will be called");
    }    
  }
  else if(digitalRead(_stop)){
    DebounceCounter_stop = 0;
  }
//
/*
    if(ExecuteKey_start == true)
    {
      ExecuteKey_start = false;
      Serial.println("start key detected");
    }
    if(ExecuteKey_pause == true)
    {
      ExecuteKey_pause = false;
      Serial.println("pause key detected");
    }
    if(ExecuteKey_resume == true)
    {
      ExecuteKey_resume = false;
      Serial.println("resume key detected");
    }
    if(ExecuteKey_stop == true)
    {
      ExecuteKey_stop = false;
      Serial.println("stop key detected");
    }
    */
//  }
}

void setup() 
{
  Serial.begin(9600);
  lcd.clear();
  lcd.begin(16, 2);
//  while (!Serial) 
  {
    ; 
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(53)) 
  {
    Serial.println("initialization failed!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SD card Error!");
  }
  
  
    pinMode(_start,INPUT_PULLUP);
    pinMode(_pause,INPUT_PULLUP);
    pinMode(_resume,INPUT_PULLUP);
    pinMode(_stop,INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(_start), ISR_start, FALLING);
    attachInterrupt(digitalPinToInterrupt(_pause), ISR_pause, FALLING);
    attachInterrupt(digitalPinToInterrupt(_resume), ISR_resume, FALLING);
    attachInterrupt(digitalPinToInterrupt(_stop), ISR_stop, FALLING);

    
    _state = 0;

  for(unsigned int pin = 22; pin < 44; pin ++){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }    
}

void _startISR()
{   
//   lcd.clear();
//   lcd.setCursor(1, 0);
//   lcd.print("Start!");
//   delay(2000);
     Serial.println("Start");
     _state = 2;
     
}
//
//void _pauseISR()
//{
//    lcd.clear();
//    lcd.setCursor(1, 0);
//    lcd.print("Pause Pressed!");
//    Serial.println("Pause");
//    attachInterrupt(digitalPinToInterrupt(_resume),_resumeISR,LOW);
//    while(_resumebutton)
//    {
//      ;
//    }
//    detachInterrupt(digitalPinToInterrupt(_pause));
//    detachInterrupt(digitalPinToInterrupt(_resume));
//    
//}
//
//void _resumeISR()
//{
//   lcd.clear();
//   lcd.setCursor(1, 0);
//   lcd.print("Resume Pressed!");
//   Serial.println("Resume");
//   _resumebutton = LOW;
//}
//
//void _stopISR()
//{
//   lcd.clear();
//   lcd.setCursor(1, 0);
//   lcd.print("Stop Pressed!");
//   Serial.println("Stop");
//   _state = 0;
//}

unsigned int _GetNumber(char *_FindNo)
{
  char *_DataPtr = 0;
  char *_CommaIndex = 0;
  char _SequenceNo[1000] = {0};
  unsigned int _add = 0;
  int _Sequence = 0;

  _DataPtr = strstr(_ldAry, _FindNo);
  _DataPtr += strlen(_FindNo);


  _CommaIndex = strchr(_DataPtr, ',');

  for(unsigned char _inc = 0; _DataPtr < _CommaIndex; _inc++,_DataPtr++)
    {
      _SequenceNo[_inc] = *_DataPtr;
    }
    _Sequence = atoi(_SequenceNo);
    return(_Sequence);
}

void _extract(int _Seq) 
{

char _GenStr[_Seq][5]={0};
  for(char _IncAry = 0;_IncAry < _Seq; _IncAry++)
    {
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("LD:");
     
     lcd.setCursor(0, 1);
     lcd.print("CY:");
     
     lcd.setCursor(3, 1);
     lcd.print(_executeLoop);
     
     Serial.println(_Seq);
     int _PassOn = 0;
     char _LoadVal[10] = {0};
     char _TimeVal[20] = {0};
     sprintf(&(_GenStr[_IncAry][0]), "#%03d,",_IncAry + 1);
     Serial.println(&(_GenStr[_IncAry][0]));
     
    _DataPtr = strstr(_ldAry, (&(_GenStr[_IncAry][0])));
    _DataPtr += strlen(&(_GenStr[_IncAry][0]));
    
    _add = _DataPtr;
    //Serial.println(_add);


    _LoadPtr = strchr(_DataPtr, ',');
    _add = _LoadPtr;
    //Serial.println(_add);
  

    for(unsigned char _incLoad = 0 ; _DataPtr < _LoadPtr ; _incLoad++,_DataPtr++)
      {
        _LoadVal[_incLoad] = *_DataPtr;
      }
    
      _DataPtr += 1;
      _TimePtr = strchr(_DataPtr, '\n');
      _TimePtradd = _TimePtr;
      //Serial.println(_TimePtradd);
  
      for(unsigned char _incTime = 0;_DataPtr < _TimePtr ; _incTime++,_DataPtr++)
      {
        _TimeVal[_incTime] = *_DataPtr;
      }
      
    int _NumberOfRelays = atoi(_LoadVal);
    Serial.println(_NumberOfRelays);
    Serial.println(_LoadVal);
    unsigned long _delay = atol(_TimeVal);
    unsigned int _timeInSec = _delay/1000;
    Serial.println(_delay);

    
        
            //attachInterrupt(digitalPinToInterrupt(_pause),_pauseISR,LOW);
            //attachInterrupt(digitalPinToInterrupt(_stop),_stopISR,LOW);
    
    int _load1000, _load200, _load100, _load60, _load50, _load40, _load20, load10;
    lcd.setCursor(3, 0);
    lcd.print(_NumberOfRelays);
    lcd.print("W");

    lcd.setCursor(10, 0);
    lcd.print("T:");
    lcd.print(_timeInSec);
    
    digitalWrite(42,HIGH);
    
//    PORTA = 0xFF;
//    PORTC = 0xFF;
//    PORTD = 0xFF;
//    PORTG = 0xFF;
    
    _load1000 = _load200 = _load100 = _load60 = _load50 = _load40 = _load20 = load10 = 0;
 
    if(_NumberOfRelays <= 0)
    {
      delay(500);
      digitalWrite(42,LOW);
      lcd.setCursor(0, 0);
      lcd.print("Charging ON");
      Serial.println("Charging ON");
      
    }
    /* Input _NumberOfRelays from user */
//    if(_NumberOfRelays >= 1000)
    {
        _load1000 = _NumberOfRelays/1000;
        _NumberOfRelays -= _load1000 * 1000;

        switch(_load1000)
        {
        case 1: 
        PORTA = (PORTA | 0x0F)& B11111110;
//        PORTA &= B11111110;
        Serial.println("1 OF 1000W ON");
        break;

        case 2:
        PORTA = (PORTA | 0x0F)& B11111100;
//        PORTA &= B11111100; //22,23 ON
        Serial.println("2 OF 1000W ON");
        break;

        case 3:
        PORTA = (PORTA | 0x0F)& B1111000;
//        PORTA &= B1111000; //22,23,24 ON
        Serial.println("3 OF 1000W");
        break;

        case 4:
        PORTA = (PORTA | 0x0F)& B11110000;
//        PORTA &= B11110000;
        Serial.println("4 OF 1000W");
        break;

        default:
        PORTA |= B00001111;
        break;
        }
    }

//    if(_NumberOfRelays >= 200)
    {   
        _load200 = _NumberOfRelays/200;
        _NumberOfRelays -= _load200 * 200;
        switch(_load200)
        {
        case 1: 
        PORTA = (PORTA | 0xF0)& B11101111;
//        PORTA &= B11101111; //26 ON
        Serial.println("1 OF 200W ON");
        break;

        case 2:
        PORTA = (PORTA | 0xF0)& B11001111;
//        PORTA &= B11001111; //26,27 ON
        Serial.println("2 OF 200W ON");
        break;

        case 3:
        PORTA = (PORTA | 0xF0)& B10001111;
//        PORTA &= B10001111; //26,27,28 ON
        Serial.println("3 OF 200W ON");
        break;

        case 4:
        PORTA = (PORTA | 0xF0)& B00001111;
//        PORTA &= B00001111;
        Serial.println("4 OF 200W ON");
        break;

        default:
        PORTA |= B11110000;
        break;
        }
    }
    
//    if(_NumberOfRelays >= 40)
    {     
        _load40 = _NumberOfRelays /40;
        _NumberOfRelays -= _load40 * 40;
        switch(_load40)
        {
        case 1: 
        {
          PORTC = (PORTC | 0x0F)& B11111110;
//        PORTC &= B11111110; //30 ON
        Serial.println("1 OF 40W ON");
        }
        break;
        
        case 2:
        PORTC = (PORTC | 0x0F)& B11111100;        
//        PORTC &= B11111100; //30,31  ON
        Serial.println("2 OF 40W ON");
        break;

        case 3:
        PORTC = (PORTC | 0x0F)& B11111000; 
//        PORTC &= B11111000; //30,31,32 ON
        Serial.println("3 OF 40W ON");
        break;

        case 4:
        PORTC = (PORTC | 0x0F)& B11110000; 
//        PORTC &= B11110000; //30,31,32,33 ON
        Serial.println("4 OF 40W ON");
        break;

        default:
        PORTC |= B00001111;
        break;
      }
    }
    
//    if(_NumberOfRelays >= 20)
    {   
        _load20 = _NumberOfRelays/20;
        _NumberOfRelays -= _load20 * 20;
        switch(_load20)
        {
        case 1: 
        PORTC = (PORTC | 0xF0)& B11101111;
//        PORTC &= B11101111; //34 ON
        Serial.println("1 OF 20W ON");
        break;

        case 2:
        PORTC = (PORTC | 0xF0)& B11001111;
//        PORTC &= B11001111; //34,35 ON
        Serial.println("2 OF 20W ON");
        break;

        case 3:
        PORTC = (PORTC | 0xF0)& B10001111;
//        PORTC &= B10001111; //34,35,36 ON
        Serial.println("3 OF 20W ON");
        break;

        case 4:
        PORTC = (PORTC | 0xF0)& B00001111;
//        PORTC &= B00001111; //34,35,36,37 ON
        Serial.println("4 OF 20W ON");
        break;

        default:
        PORTC |= B11110000;
        break;
        }
        
    }
   
//    if(_NumberOfRelays >= 10)
    {   
        load10 = _NumberOfRelays/10;
        _NumberOfRelays -= load10 * 10;
        switch(load10)
        {
        case 1:
        PORTD = (PORTD | 0xF0)& B01111111; 
//        PORTD &= B01111111; //38 ON
        Serial.println("1 OF 10W ON");
        break;

        case 2:
        PORTD = (PORTD | 0xF0)& B01111111; 
        PORTG = (PORTG | 0x0F)& B11111011; 
//        PORTD &= B01111111; //38 ON
//        PORTG &= B11111011; //39 ON
        Serial.println("2 OF 10W ON");
        break;

        case 3:
        PORTD = (PORTD | 0xF0)& B01111111; 
        PORTG = (PORTG | 0x0F)& B11111001;
//        PORTD &= B01111111; //38 ON
//        PORTG &= B11111001; //39,40 ON
        Serial.println("3 OF 10W ON");
        break;

        case 4:
        PORTD = (PORTD | 0xF0)& B01111111; 
        PORTG = (PORTG | 0x0F)& B11111000;
//        PORTD &= B01111111; //38 ON
//        PORTG &= B11111000; //39,40,41 ON
        Serial.println("4 OF 10W ON");
        break;

        default:
        PORTD |= B11110000;
        PORTG |= B00001111;
        break;
        }
    }
     //delay(_LoadTime);
    
//    attachInterrupt(digitalPinToInterrupt(_stop),_stopISR,LOW);
    while(!_PassOn)
    {
    unsigned long _currentmillis = millis();
 
      if(_currentmillis - _previousmillis >= _delay)
      {
        _PassOn = 1;
        _previousmillis = _currentmillis;
      }
      KeyDetection();
      if(ExecuteKey_pause == true)
      {
        ExecuteKey_pause = false;
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("Paused!");

        Serial.println("pause key detected, press resume or STOP key");
        while(!ExecuteKey_resume)
        {
          KeyDetection();
          if(ExecuteKey_stop)
          {
            ExecuteKey_stop = false;
            lcd.clear();
            lcd.setCursor(2,0);
            lcd.print("Stop Executed");
            Serial.println("Stop Executed");
            
            _executeLoop = 0;
            _executeSequence = 0;
            ExecuteKey_start = false;
            _state = 0;
            for(unsigned int pin = 22; pin < 42; pin ++){
              digitalWrite(pin, HIGH);
            }
            asm volatile ( "jmp 0");
            return;
          }
        }
        if(ExecuteKey_resume)
        {
            ExecuteKey_resume = false;
            lcd.clear();
            lcd.setCursor(4,0);
            lcd.print("Resumed!");
            Serial.println("Resumed");
        }
      }
      else if(ExecuteKey_stop)
      {
        ExecuteKey_stop = false;
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Stop Executed");
        Serial.println("Stop Executed");
        
        _state = 0;
        _executeLoop = 0;
        _executeSequence = 0;
        ExecuteKey_start = false;
        for(unsigned int pin = 22; pin < 42; pin ++){
          digitalWrite(pin, HIGH);
        }    
        asm volatile ( "jmp 0");
        return;
      }
      else if(ExecuteKey_resume)
      {
          ExecuteKey_resume = false;
          Serial.println("Nothing to resume");
      }
      else if(ExecuteKey_start)
      {
          ExecuteKey_start = false;
          Serial.println("Nothing to start");
      }
    }      
}
//return(_delay);
}

void loop(){
  KeyDetection();
switch (_state)
 {
  case 0://init
  {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Load Simulator");
  lcd.setCursor(5, 1);
  lcd.print("GODREJ"); 
  Serial.println("initialization done.");
  dataFile = SD.open("DATA.csv", FILE_READ);
  if (dataFile) 
    {
      filesize = dataFile.size();
      Serial.print("Size=");
      Serial.println(filesize);
      dataFile.read(_ldAry, filesize);
      Serial.print(_ldAry);
    }
  else 
    { 
      Serial.println("error opening LoadProfile.csv");
      lcd.setCursor(0, 0);
      lcd.print("Error opening");
      lcd.setCursor(0, 1);
      lcd.print("File!");
    }
    _NoOfLoops = _GetNumber("NoOfLoops,");
    _NoOfSequence = _GetNumber("NoOfSequence,");


    if(_NoOfLoops && _NoOfSequence)
      {
        _state = 1;  
      }
    else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Invalid Data!"); 
        Serial.println("Invalid Data!");
        delay(500); 
      }

    break;
  }

  case 1://wait for start key
  {
//    attachInterrupt(digitalPinToInterrupt(_start),_startISR,LOW);

    if(ExecuteKey_start == true)
    {
      ExecuteKey_start = false;
      _state = 2;

      Serial.println("start key detected");
    }
    break;
  }

  case 2://Execute loop
  {
//    static bool _paused = false;
//    detachInterrupt(digitalPinToInterrupt(_start));
//    if(_paused == false)
//    {
//      if(ExecuteKey_pause == true)
//      {
//        ExecuteKey_pause = false;
//        _paused = true;
//        Serial.println("pause key detected");
//      }

      for(_executeLoop = 1; _executeLoop <= _NoOfLoops; _executeLoop++)
      {
       _extract(_NoOfSequence);
      }
      Serial.println("_state: 2");
      Serial.println(_state);
      _state = 0;
//    }
//    else{//paused
//      if(ExecuteKey_resume == true)
//      {
//        ExecuteKey_resume = false;
//        _paused = false;
//        Serial.println("resume key detected");
//      }
//      else if(ExecuteKey_stop == true)
//      {
//        ExecuteKey_stop = false;
//        _paused = false;
//        Serial.println("stop key detected");
//      }
//    }
   }
   
  }
}
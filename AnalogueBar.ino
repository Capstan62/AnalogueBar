/*     
  DEMO SKETCH FOR THE BUILT IN LCD DISPLAY AND POTENTIOMETER
      

DISPLAYS ARDUINO'S BUILT 'MILLISEC' TIMER IN FORM OF HH:MM:SS
    PLUS AN ANALOGUE BAR REPRESENTING POTENTIOMETER VALUE

Copyright (c) <2015> Alessando Capitani

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/


#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int temp1 = 1;
int curs;

byte DisplayA[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
byte DisplayB[8] = {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000};
byte DisplayC[8] = {B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000};
byte DisplayD[8] = {B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100};
byte DisplayE[8] = {B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110};



void setup()
{

	lcd.begin (16, 2);

	lcd.createChar (1, DisplayA);
	lcd.createChar (2, DisplayB);
	lcd.createChar (3, DisplayC);
	lcd.createChar (4, DisplayD);
	lcd.createChar (5, DisplayE);

	pinMode(10, OUTPUT);
	analogWrite(10, 127); //dim backlight @50%

	// Serial.begin(9600); only for debug

	lcd.home();

	lcd.print("A. Capitani 2015"); //print welcome screen
	lcd.setCursor (0, 1);
	lcd.print("Progressbar Demo");
	delay(2000);
	lcd.clear(); // init screen
	lcd.setCursor (0, 0);
	lcd.print("Time=");
	lcd.setCursor (0, 1);
	lcd.print("Pot =");
}

void loop()
{

	lcd.home();
	lcd.setCursor (5, 0);

	lcd.print(clockStr(millis())); //prints internal Arduino timer, clock style

	int val = analogRead(1);
	analogueBar (11, 5, 1, 1024, val); // draws the analogue bar

	
		/*
		PLEASE NOTE analogueBar FUNCTION PARAMETERS:
		analogueBar (int fullScale, int xPos, int yPos, int maxValue, int val )
		    fullScale // max bar lenght
			xPos // horizontal start (n-1)
			int yPos = 1 // row select (n-1)
			int maxValue // max input value to get fullscale representatin
		*/

	
} 

String formatInt(int x, int len)
{
	String r = String(x);
	while(r.length() < len)r = "0" + r;
	return r;
}

String clockStr(long mi)
{
	mi = mi / 1000;
	String s = String(mi % 60);
	if(s.length() < 2)s = "0" + s;
	String m = String((mi / 60) % 60);
	if(m.length() < 2)m = "0" + m;
	String h = String((mi / 3600) % 60);
	if(h.length() < 2)h = "0" + h;
	return h + ":" + m + ":" + s;
}

void clock(long mi, int & h, int & m, int & s)
{

	mi = mi / 1000;
	s = mi % 60;
	m = (mi / 60) % 60;
	h = (mi / 3600) % 60;
	return;

}

void analogueBar (int fullScale, int xPos, int yPos, int maxValue, int val )
{
	/*	int fullScale = 16; // max bar lenght
		int xPos = 0; // horizontal start
		int yPos = 1; // row select
		int maxValue = 1024; // max input value to get fullscale representatin
	*/

	val = map(val, 0, maxValue, 0, fullScale * 11); 
	int val2 = val % 10; // this allows to compute the sub segment(s) to be lit, in 0-9 range
	val2 = map(val2, 0, 9, 0, 4); // then in range 0-4
	val = val / 10; // this allows to compute the actual number of full segments to be lit

	if(val != temp1) //if a variation from previous pot value reading is detected, then bargraph routine is recalled
	{
		if (val < temp1)
		{
			lcd.setCursor (curs - 1, yPos); //taking care of bar deletion when decreasing, anti flicker
			lcd.print("        ");	    // a single empty char is not enough, this avoids problems in case of sudden variation...
		}

		temp1 = val;
		lcd.setCursor (xPos, yPos);
		for (int i = 0; i <= val; i++) //fill bargraph, integer part
		{
			curs = i + xPos;
			lcd.setCursor (curs, yPos);
			lcd.write(255);


		}

	}

	lcd.setCursor (curs, yPos);

	switch (val2)  //fill bargraph, sub-segment part
	{

		case 0:
			lcd.write (1);
			break;

		case 1:
			lcd.write (2);
			break;

		case 2:
			lcd.write (3);
			break;

		case 3:
			lcd.write (4);
			break;

		case 4:
			lcd.write (5);
			break;

	}
}

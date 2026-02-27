#include "Arduino.h"
#include "LiquidCrystal.h"
#include "WhosOnFirst.h"

const byte WhosOnFirst::wordLists[35][10] = {
	{42, 58, 57, 25, 27, 68, 45, 15, 35, 0},
	{29, 60, 40, 20, 57, 50, 7, 12, 1, 36},
	{11, 60, 66, 1, 28, 62, 64, 52, 2, 37},
	{4, 2, 48, 13, 0, 5, 65, 47, 38, 3},
	{16, 26, 20, 32, 54, 23, 3, 6, 39, 4},
	{11, 1, 35, 47, 38, 14, 4, 23, 5, 40},
	{24, 22, 36, 33, 47, 34, 28, 10, 6, 41},
	{68, 53, 59, 67, 12, 45, 33, 16, 4, 42},
	{68, 42, 37, 61, 41, 67, 54, 26, 43, 8},
	{39, 36, 63, 26, 32, 35, 38, 67, 9, 44},
	{54, 6, 39, 61, 56, 12, 21, 13, 45, 10},
	{50, 17, 62, 33, 42, 0, 37, 6, 11, 46},
	{45, 5, 43, 10, 21, 51, 1, 2, 12, 47},
	{30, 12, 40, 58, 19, 11, 29, 32, 13, 48},
	{46, 40, 3, 52, 2, 55, 25, 31, 49, 14},
	{16, 46, 56, 6, 39, 21, 57, 60, 50, 15},
	{54, 55, 4, 39, 22, 20, 30, 9, 51, 16},
	{7, 28, 0, 41, 3, 65, 68, 24, 17, 52},
	{60, 11, 48, 55, 13, 8, 26, 22, 18, 53},
	{28, 20, 50, 23, 46, 64, 49, 17, 54, 19},
	{5, 10, 45, 15, 19, 18, 53, 51, 55, 20},
	{53, 29, 30, 44, 37, 9, 7, 39, 56, 21},
	{69, 44, 41, 14, 6, 17, 18, 58, 22, 57},
	{28, 25, 18, 27, 29, 26, 5, 19, 58, 23},
	{64, 55, 23, 15, 63, 52, 36, 8, 59, 24},
	{21, 62, 33, 65, 0, 7, 61, 27, 25, 60},
	{2, 34, 1, 38, 50, 15, 63, 11, 61, 26},
	{49, 29, 45, 31, 47, 16, 34, 66, 62, 27},
	{19, 24, 4, 7, 53, 30, 35, 46, 63, 28},
	{57, 20, 52, 46, 59, 25, 48, 41, 64, 29},
	{26, 66, 9, 35, 24, 49, 14, 69, 65, 30},
	{20, 61, 51, 33, 69, 61, 56, 20, 66, 31},
	{27, 5, 38, 30, 44, 66, 42, 59, 67, 32},
	{26, 69, 31, 10, 57, 4, 44, 53, 68, 33},
	{9, 37, 17, 8, 43, 36, 62, 40, 34, 69},
};

const String WhosOnFirst::words[70] = {"\'", "///", "/SLASH/", "3 SLASHES", "9", "", "AND", "APOSTROPHE", "BLANK", "BOTTOM", "BUTTON", "CHARACTER", "CLEAR", "DISPLAY", "DONE", "HOLD ON", "IT SAYS", "LEFT", "LETTER", "LIKE", "LITERALLY", "MIME", "MINE", "NEXT", "NIME", "NINE", "NO", "NOTHING", "NUMBER", "OK", "OKAY", "PRESS", "QUOTE", "READ", "READY", "RED", "REED", "RIGHT", "SLASH//", "SPACE", "START", "STOP", "SURE", "SYMBOL", "THE", "THEIR", "THEN", "THERE", "THEY ARE", "THEY\'RE", "TOP", "U ARE", "U M", "U R", "UH HUH", "UH UH", "UHHH", "UM", "UMMM", "UR", "WAIT", "WHAT?", "WHERE?", "WITH A", "WITH AN", "WORD", "YES", "YOU ARE", "YOU\'RE", "YOUR"};
byte WhosOnFirst::leftIdx[35] = {0, 1, 2, 6, 9, 10, 12, 13, 18, 19, 20, 22, 23, 24, 25, 27, 26, 29, 31, 32, 33, 37, 41, 44, 47, 49, 56, 54, 57, 53, 60, 62, 64, 65, 68};
byte WhosOnFirst::topIdx[35] = {0, 1, 5, 11, 12, 14, 13, 15, 18, 19, 21, 25, 26, 30, 31, 33, 34, 35, 37, 38, 39, 41, 46, 47, 48, 50, 55, 56, 57, 53, 60, 61, 63, 69, 67};

WhosOnFirst::WhosOnFirst(byte rs, byte en, byte d4, byte d5, byte d6, byte d7, byte b1, byte b2, byte b3, byte b4) 
	: lcd(rs, en, d4, d5, d6, d7) {
	buttonPort[0] = b1;
	buttonPort[1] = b2;
	buttonPort[2] = b3;
	buttonPort[3] = b4;
}

bool WhosOnFirst::disarmed() {
    return isDisarmed;
}

void WhosOnFirst::disarm() {
    isDisarmed = true;
}



byte WhosOnFirst::find_correct_button() {
	byte wordListIdx = dispWords[(in(leftIdx, blinkPos[0]) ? 0 : 2) + (in(topIdx, blinkPos[1]) ? 0 : 1)] % 35;
	Serial.println((in(leftIdx, blinkPos[0]) ? 0 : 2) + (in(topIdx, blinkPos[1]) ? 0 : 1));
	for (byte i=0; i<10; i++) {
		for (byte j=0; j<4; j++) {
			if (wordLists[wordListIdx][i] == dispWords[j]) {
				Serial.println(words[dispWords[j]]);
				return j;
			}
		}
	}
}

bool WhosOnFirst::identical_disp_words() {
	for (byte i=0; i<4; i++) {
		for (byte j=i+1; j<4; j++) {
			if (dispWords[i] == dispWords[j]) {
				return true;
			}
		}
	}
	return false;
}

void WhosOnFirst::randomize() {
	// randomize blinking positions
	do {
		blinkPos[0] = random(0, 4);
		blinkPos[1] = random(0, 4);
	} while (blinkPos[0] == blinkPos[1]);

	// randomize words
	do {
		// initial randomization
		for (byte i=0; i<4; i++) {
			if ((i == blinkPos[0]) || (i == blinkPos[1])) {
				do {
					dispWords[i] = random(0, 70);
				} while (dispWords[i] == 5);
			}
			else {
				dispWords[i] = random(0, 70);
			}
		}
		// slightly alter initial randomization it so that words in the given list are more likely to appear
		byte step1pos = (in(leftIdx, blinkPos[0]) ? 0 : 2) + (in(topIdx, blinkPos[1]) ? 0 : 1);
		for (byte i=0; i<4; i++) {
			if ((i != step1pos) && (i != blinkPos[0]) && (i != blinkPos[1]) && (random(0, 2) == 0)) {
				dispWords[i] = wordLists[dispWords[step1pos] % 35][random(0, 6)];
			}
		}
	} // ensure words aren't too long, and no two words are identical
	while ((words[dispWords[0]].length() + words[dispWords[1]].length() >= 16) || (words[dispWords[2]].length() + words[dispWords[3]].length() >= 16) || identical_disp_words());

	Serial.println(dispWords[0]);
	Serial.println(dispWords[1]);
	Serial.println(dispWords[2]);
	Serial.println(dispWords[3]);
	Serial.println(words[dispWords[0]]);
	Serial.println(words[dispWords[1]]);
	Serial.println(words[dispWords[2]]);
	Serial.println(words[dispWords[3]]);
}

bool WhosOnFirst::in(byte listIdx[35], byte val) {
	for (byte i=0; i<35; i++) {
		if (val == listIdx[i]) {
			return true;
		}
		else if (val > listIdx[i]) {
			return false;
		}
	}
}

void WhosOnFirst::display_words() {
	lcd.clear();
	for (byte i=0; i<4; i++) {
		if (((i != blinkPos[0]) && (i != blinkPos[1])) || (i == blinkPos[0] && (millis() % 2000 < 1000)) || ((i == blinkPos[1]) && (millis() % 1000 < 500))) {
			String word = words[dispWords[i]];
			lcd.setCursor((i % 2 == 0) ? 0 : 16-word.length(), i/2);
			lcd.print(word);
		}
	}
}

void WhosOnFirst::msetup() {
	lcd.begin(16, 2);
	randomize();
	display_words();
}

bool WhosOnFirst::mloop() {
	if (!isDisarmed) {
		if (intermissionClock > 0) {
			intermissionClock--;
			if (intermissionClock == 0) {
				randomize();
			}			
		}
		else {
			display_words();
			for (byte i=0; i<4; i++) {
				if (digitalRead(buttonPort[i]) == HIGH) {
					held[i] = true;

					Serial.print("Button pressed ");
					Serial.println(i);
				}
				else {
					if (held[i]) {
						held[i] = false;
						if (i == find_correct_button()) {
							stage++;
							lcd.clear();
							lcd.setCursor(4, 0);
							lcd.print("STAGE ");
							lcd.print(stage);
							lcd.setCursor(4, 1);
							lcd.print("COMPLETE");
							intermissionClock = 24;
							if (stage == 3) {
								disarm();
							}
						} 
						else {
							return true;
						}
					}
				}
			}
		}
	}
    return false;
}



void WhosOnFirst::print_info() {

}
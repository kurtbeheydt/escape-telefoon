#include <Arduino.h>
#include <Keypad.h>

#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

const byte pinHoorn = 12;

const int passwordLength = 4;
char inputPassword[passwordLength];
char correctPassword[passwordLength] = {'1', '2', '4', '5'};

int passwordIndex = 0;

const byte rows = 4;
const byte columns = 3;

const char hexaKeys[rows][columns] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

const byte rowPins[rows] = {5, 6, 7, 8};
const byte columnPins[columns] = {2, 3, 4};

Keypad keypadKey = Keypad(makeKeymap(hexaKeys), rowPins, columnPins, rows, columns);

SoftwareSerial mySoftwareSerial(10, 9);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void resetPassword() {
    for (int i = 0; i < passwordLength; i++) {
        inputPassword[i] = 0;
    }
}

void setup() {
    pinMode(pinHoorn, INPUT_PULLUP);
    mySoftwareSerial.begin(9600);

    Serial.begin(115200);
    Serial.println("###");

    Serial.print("Correct Password: ");
    Serial.println(String(correctPassword));

    Serial.println();
    Serial.println(F("DFRobot DFPlayer Mini"));
    Serial.println(F("Initializing DFPlayer module ... Wait!"));

    if (!myDFPlayer.begin(mySoftwareSerial)) {
        Serial.println(F("Not initialized:"));
        Serial.println(F("1. Check the DFPlayer Mini connections"));
        Serial.println(F("2. Insert an SD card"));
        while (true)
            ;
    }

    Serial.println();
    Serial.println(F("DFPlayer Mini module initialized!"));

    myDFPlayer.setTimeOut(500);
    myDFPlayer.volume(25);
    myDFPlayer.EQ(0);
}

void loop() {
    bool phoneActive = !digitalRead(pinHoorn);

    if (phoneActive) {
        char key_pressed = keypadKey.getKey();

        if (key_pressed) {
            Serial.println(key_pressed);

            inputPassword[passwordIndex] = key_pressed;
            passwordIndex++;

            if (passwordIndex >= passwordLength) {
                Serial.print("input password: *");
                Serial.print(String(inputPassword));
                Serial.print("* - correct password: *");
                Serial.print(String(correctPassword));
                Serial.println("*");

                delay(500);

                if (!(strncmp(inputPassword, correctPassword, passwordLength))) {
                    Serial.println("correct");
                    myDFPlayer.play(2);
                } else {
                    Serial.println("niet correct");
                    myDFPlayer.play(1);
                }

                resetPassword();
                passwordIndex = 0;
            }
        }
    } else {
        myDFPlayer.stop();
        resetPassword();
    }
}
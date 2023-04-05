#pragma once

#include "BoxController.h"

class TableHeightController {
    public:
        enum states {
            idle,
            raising,
            lowering
        };

    private:
        pin tableRaisePinId;
        pin tableLowerPinId;

        unsigned long lastActionMillis;
        unsigned long lastActionDuration;

        states state;

    public:
        TableHeightController(pin tableRaisePinId, pin tableLowerPinId) {
            pinMode(tableRaisePinId, OUTPUT);
            pinMode(tableLowerPinId, OUTPUT);
            this->tableRaisePinId = tableRaisePinId;
            this->tableLowerPinId = tableLowerPinId;
            this->state = states::idle;
        }

        void beginRaise() {
            digitalWrite(tableRaisePinId, HIGH);
            digitalWrite(tableLowerPinId, LOW);
            this->state = states::raising;
            this->lastActionMillis = millis();
            this->lastActionDuration = -1;
            #ifdef DEBUG_PRINT_TABLEHEIGHT
                Serial.println("BeginRaiseTable");
            #endif
        }

        void raise(int ms) {
            this->beginRaise();
            this->lastActionDuration = ms;
            #ifdef DEBUG_PRINT_TABLEHEIGHT
                Serial.print("RaiseTable: ");
                Serial.print(ms);
                Serial.println("ms");
            #endif
        }

        void beginLower() {
            digitalWrite(tableRaisePinId, LOW);
            digitalWrite(tableLowerPinId, HIGH);
            this->state = states::lowering;
            this->lastActionMillis = millis();
            this->lastActionDuration = -1;
            #ifdef DEBUG_PRINT_TABLEHEIGHT
                Serial.println("BeginLowerTable");
            #endif
        }
        
        void lower(int ms) {
            this->beginLower();
            this->lastActionDuration = ms;
            #ifdef DEBUG_PRINT_TABLEHEIGHT
                Serial.print("LowerTable: ");
                Serial.print(ms);
                Serial.println("ms");
            #endif
        }

        void stopMovement() {
            digitalWrite(tableRaisePinId, LOW);
            digitalWrite(tableLowerPinId, LOW);
            this->state = states::idle;
            #ifdef DEBUG_PRINT_TABLEHEIGHT
                Serial.println("StopTable");
            #endif
        }

        void tick() {
            switch (this->state) {
                case states::raising:
                case states::lowering:
                    if(millis() - this->lastActionMillis >= this->lastActionDuration)
                        stopMovement();
                    break;
                
                case states::idle:
                    break;
            }
        }
};

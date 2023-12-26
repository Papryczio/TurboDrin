#include "main.h"

void setup()
{
    Serial.setRxBufferSize(512);
    Serial.begin(115200);

    SerialBT.begin("Nalewak");

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(CALIBRATION_FACTOR);
    scale.tare();

    attachLoadCellInterrupt();
}

void loop()
{
    String message = readBTline();
    parseRequest(message);
    Serial.println("Glass weight: " + (String)glassWeight + " detected: " + (String)glassDetected);
}

/**
 * The function reads a line of text from a Bluetooth connection and returns it as a string.
 *
 * @return a string message.
 */
String readBTline()
{
    String message = "";

    if (SerialBT.available())
    {
        message = SerialBT.readStringUntil('\n');
    }
    return message;
}

/**
 * The function parses a JSON string, prints it to the serial monitor, and then resolves the request.
 *
 * @param json The "json" parameter is a string that represents a JSON object. It is the input to the
 * function and contains the JSON data that needs to be parsed.
 *
 * @return The function does not have a return type specified, so it does not explicitly return
 * anything. However, there are two return statements within the function. If the input parameter
 * `json` is an empty string, the function will return immediately without performing any further
 * actions. If there is a parsing error while deserializing the JSON string, the function will also
 * return without executing the `resolveRequest` function.
 */
void parseRequest(String json)
{
    if (json == "")
        return;
    Serial.println("Received a message via BT:\n" + json);
    // Parse the JSON string
    StaticJsonDocument<256> jsonDocument;
    DeserializationError error = deserializeJson(jsonDocument, json);

    // Check for parsing errors
    if (error)
    {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        return;
    }

    resolveRequest(jsonDocument);
}

/**
 * The function `resolveRequest` processes a JSON request and performs actions based on the specified
 * action type.
 *
 * @param jsonDocument The `jsonDocument` parameter is a `StaticJsonDocument` object that contains the
 * JSON data received from an external source. It is used to extract information about the requested
 * action and the values for the liquid quantities.
 */
void resolveRequest(StaticJsonDocument<256> jsonDocument)
{
    if (jsonDocument["Action"] == "START_AUTO_PROGRAM")
    {
        Serial.println("ACTION:START_AUTO_PROGRAM");

        // Motor#1
        int liquid_1 = jsonDocument["Liquid_1"];
        double time_1 = calculateTime(liquid_1);
        Serial.println("Motor#1 -- liquid:" + (String)liquid_1 + ", time:" + (String)time_1);
        SwitchOnMotor_1(time_1);

        // Motor#2
        int liquid_2 = jsonDocument["Liquid_2"];
        double time_2 = calculateTime(liquid_2);
        Serial.println("Motor#2 -- liquid:" + (String)liquid_2 + ", time:" + (String)time_2);
        SwitchOnMotor_2(time_2);

        requestResponse("START_AUTO_PROGRAM_SUCCESS", time_1, time_2);
    }
    else if (jsonDocument["Action"] == "ABORT_AUTO_PROGRAM")
    {
        Serial.println("ACTION:ABORT_AUTO_PROGRAM");

        motor_1.switchOff();
        motor_2.switchOff();

        if (motor_1.isAuto())
        {
            clearTimer(My_timer_1);
        }
        if (motor_2.isAuto())
        {
            clearTimer(My_timer_2);
        }

        requestResponse("ABORT_AUTO_PROGRAM_SUCCESS");
    }
}

/**
 * The above code defines two overloaded functions that create a JSON document with an "Action" field
 * and optional "Time_1" and "Time_2" fields, and then serialize the document to the Serial and
 * SerialBT streams.
 *
 * @param action The "action" parameter is a string that represents the action being requested or
 * performed. It could be any action that the code is designed to handle or respond to.
 * @param time_1 The parameter "time_1" is a double data type, which means it can store decimal
 * numbers. It is used to represent a specific time value in the requestResponse function.
 * @param time_2 The parameter "time_2" is a double data type, which means it can store decimal
 * numbers. It is used in the "requestResponse" function to represent a second time value.
 */
void requestResponse(String action, double time_1, double time_2)
{
    DynamicJsonDocument doc(256);
    doc["Action"] = action;
    doc["Time_1"] = time_1;
    doc["Time_2"] = time_2;

    serializeJson(doc, Serial);
    serializeJson(doc, SerialBT);
}

void requestResponse(String action)
{
    DynamicJsonDocument doc(256);
    doc["Action"] = action;

    serializeJson(doc, Serial);
    serializeJson(doc, SerialBT);
}

// =============
// Motor control
// =============

void IRAM_ATTR motor_1_switch()
{
    motor_1.switchOff();
    clearTimer(My_timer_1);
    motor_1.setAuto(0);
}

void SwitchOnMotor_1(double time)
{
    My_timer_1 = timerBegin(0, 80, true);
    timerAttachInterrupt(My_timer_1, &motor_1_switch, true);
    int execution_time = secondsToMicroSeconds(time);
    Serial.println(execution_time);
    timerAlarmWrite(My_timer_1, execution_time, false);
    timerAlarmEnable(My_timer_1);
    motor_1.switchOn();
    motor_1.setAuto(1);
    Serial.println("Motor#1 switched on");
}

// Motor#2 control
void IRAM_ATTR motor_2_switch()
{
    motor_2.switchOff();
    clearTimer(My_timer_2);
    motor_2.setAuto(0);
}

void SwitchOnMotor_2(double time)
{
    My_timer_2 = timerBegin(1, 80, true);
    timerAttachInterrupt(My_timer_2, &motor_2_switch, true);
    int execution_time = secondsToMicroSeconds(time);
    Serial.println(execution_time);
    timerAlarmWrite(My_timer_2, execution_time, true);
    timerAlarmEnable(My_timer_2);
    motor_2.switchOn();
    motor_2.setAuto(1);
    Serial.println("Motor#2 switched on");
}

// LoadCell interrupt
void IRAM_ATTR readLoadCellValue()
{
    glassWeight = scale.get_units();
    if (glassWeight > 0.1)
    {
        glassDetected = true;
    }
    else
    {
        glassDetected = false;
    }
}

void attachLoadCellInterrupt()
{
    My_timer_3 = timerBegin(3, 80, true);
    timerAttachInterrupt(My_timer_3, &readLoadCellValue, true);
    timerAlarmWrite(My_timer_3, 100000, true);
    timerAlarmEnable(My_timer_3);
}
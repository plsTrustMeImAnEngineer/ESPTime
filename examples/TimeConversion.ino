#include <ESPTime.h>

ESPTime espTime(TZ_Europe_Brussels); // create ESPTime Object and set Timezone

void setup()
{
    Serial.begin(9600);

    // Conversion from Unixtime to Human Readable time
    espTime.setTime(1661409849); // set Unixtime
    Serial.println(espTime.getFormattedDateTime()); // convert and print formatted time
    Time timeStruct = espTime.getTime(); // convert to struct
    Serial.print("Year: ");
    Serial.println(timeStruct.year);

    // Conversion also works vice versa
    Serial.println("Setting Year to 2010");
    timeStruct.year = 2010;
    espTime.setTime(timeStruct);
    Serial.println(espTime.getFormattedDateTime());
}

void loop()
{
    // nothing to do here
}
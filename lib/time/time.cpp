#include <WString.h>
#include "time.hpp"

// TODO - do something with this
//fnc for reading time from json response -----
String readTime(String realTime){

    // Convert to JSON
    String timeEst;
    int index;

    for (int i = 0; i < realTime.length(); i++) {
        if (realTime[i] == '{') {
            index = i;
            break;
        }
    }

    /*
    // Get JSON data
    timeEst = realTime.substring(index);
    Serial.println();
    Serial.println("JSON answer: ");
    Serial.println(timeEst);
    jsonAnswer.trim();
    */

    // Get rate as float
    String year = realTime.substring(index+18,index+22);
    String month = realTime.substring(index+10,index+13);
    String day = realTime.substring(index+14,index+16);
    String hour = realTime.substring(index+24,index+25);
    String minute = realTime.substring(index+27,index+28);

    //priceString.trim();
    String jsonTime = (month + " " + day + ", " + year + " " + hour + ":" + minute);

    return jsonTime;
}


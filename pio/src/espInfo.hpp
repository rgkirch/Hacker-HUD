#ifndef ESPINFO_HPP
#define ESPINFO_HPP

void printEspInfo()
{
    Serial.println(ESP.getVcc());
    Serial.println(ESP.getFreeHeap());
    Serial.println(ESP.getChipId());
    Serial.println(ESP.getSdkVersion());
    Serial.println(ESP.getCoreVersion());
    Serial.println(ESP.getBootVersion());
    Serial.println(ESP.getBootMode());
    Serial.println(ESP.getCpuFreqMHz());
    Serial.println(ESP.getFlashChipId());
    Serial.println(ESP.getFlashChipRealSize()); //gets the actual chip size based on the flash id
    Serial.println(ESP.getFlashChipSize()); //gets the size of the flash as set by the compiler
    Serial.println(ESP.getFlashChipSpeed());
    Serial.println(ESP.getFlashChipMode());
    Serial.println(ESP.getFlashChipSizeByChipId());
}

#endif

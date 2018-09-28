#pragma once

void BCD2String(const unsigned char * const BCDCode, const unsigned int BCDLength, char * str);
void String2BCD(const char * const str, unsigned char * BCDCode);
void PrintHex(const char * ptr, unsigned long length);

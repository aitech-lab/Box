/* 
Copyright (C) 2010- Alexander Chudnovets <effractor@gmail.com>

Based on SMS Server Tools 3
Copyright (C) 2006- Keijo Kasvi
http://smstools3.kekekasvi.com/

This program is free software unless you got it under another license directly
from the author. You can redistribute it and/or modify it under the terms of
the GNU General Public License as published by the Free Software Foundation.
Either version 2 of the License, or (at your option) any later version.
*/

#include <stdio.h>
#include <string.h>
#include "pdu.h"

using namespace std;


int main()
{    
    PDU pdu2;
    
    pdu2.setMessage("Привет, мир!");
    pdu2.setNumber("79150079500");
    pdu2.setSMSC("+79262909090");
    pdu2.setAlphabet(PDU::UCS2);
    
    pdu2.generate();
    printf("PDU: %s\n", pdu2.getPDU());
    printf("PDU Length: %d\n", pdu2.getMessageLen());

    return 0;
}


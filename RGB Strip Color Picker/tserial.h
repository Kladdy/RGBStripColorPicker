/*
Copyright (c) 2015 Sigge Stjärnholm

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

/* ------------------------------------------------------------------------ --
--                                                                          --
--                        PC serial port connection object                  --
--                           for non-event-driven programs                  --
--                                                                          --
--                                                                          --
--                                                                          --
--  Copyright @ 2001          Thierry Schneider                             --
--                            thierry@tetraedre.com                         --
--                                                                          --
--                                                                          --
--                                                                          --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  Filename : tserial.cpp                                                  --
--  Author   : Thierry Schneider                                            --
--  Created  : April 4th 2000                                               --
--  Modified : April 8th 2001                                               --
--  Plateform: Windows 95, 98, NT, 2000 (Win32)                             --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  This software is given without any warranty. It can be distributed      --
--  free of charge as long as this header remains, unchanged.               --
--                                                                          --
-- ------------------------------------------------------------------------ */

#ifndef TSERIAL_H
#define TSERIAL_H

#include <stdio.h>
#include <windows.h>


enum serial_parity  { spNONE,    spODD, spEVEN };


/* -------------------------------------------------------------------- */
/* -----------------------------  Tserial  ---------------------------- */
/* -------------------------------------------------------------------- */
class Tserial
{
    // -------------------------------------------------------- //
protected:
    char              port[10];                      // port name "com1",...
    int               rate;                          // baudrate
    serial_parity     parityMode;
    HANDLE            serial_handle;                 // ...

    // ++++++++++++++++++++++++++++++++++++++++++++++
    // .................. EXTERNAL VIEW .............
    // ++++++++++++++++++++++++++++++++++++++++++++++
public:
                  Tserial();
                 ~Tserial();
    int           connect          (char *port_arg, int rate_arg,
                                    serial_parity parity_arg);
    void          sendChar         (char c);
    void          sendArray        (char *buffer, int len);
    char          getChar          (void);
    int           getArray         (char *buffer, int len);
    int           getNbrOfBytes    (void);
    void          disconnect       (void);
};
/* -------------------------------------------------------------------- */

#endif TSERIAL_H


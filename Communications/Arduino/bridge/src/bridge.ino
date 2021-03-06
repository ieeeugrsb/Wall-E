/*
  CC2530 to PC bridge using Arduino Leonardo
  Info about CC2530: https://github.com/contiki-os/contiki/wiki/8051-Install-and-Use
  WARNING: CC2530 ports work at 3.3 V, not 5 V as Arduino, you need a converter.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/

const bool DEBUG = false;

void setup() {
  // The serial line settings for CC2530 communication are:
  // * Baud Rate: 115200
  // * Data Bits: 8
  // * Parity: none
  // * Stop Bits: 1
  // * Flow Control: None (no CTS / RTS pins)
  Serial1.begin(115200, SERIAL_8N1);

  // Serial line settings for PC communication
  Serial.begin(9600);
  while (!Serial) ;   // For leonardo compability
}

void loop() {
  if (Serial1.available() > 0)
    cc2530_to_pc();

  if (Serial.available() > 0)
    pc_to_cc2530();
}

void cc2530_to_pc() {
  // Read the output from CC2530
  String output = Serial1.readStringUntil('\n');

  // Send it by the USB Serial Communication (PC)
  if (output.length() > 0 && (DEBUG || output[0] != '#'))
    Serial.println(output);
}

void pc_to_cc2530() {
  // Read the input from the PC
  String input = Serial.readStringUntil('\n');

  // Send to CC2530
  if (input.length() > 0)
    Serial1.println(input);
}

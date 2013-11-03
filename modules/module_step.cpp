#include "module_step.h"
#include "../include/includes.h"

//*******************************************************************
//Class for accessing GNUBLIN Module-step
//*******************************************************************

//-------------gnublin_module_step-------------
/** @~english
* @brief Set irun and vmax to the default values (irun = 15, vmax = 8).
*
* @~german
* @brief Setzt die Standartwerte für irun und vmax (irun = 15, vmax = 8).
*
*/
gnublin_module_step::gnublin_module_step() {
  irun = 15;
  vmax = 8;
  stepmode = 12; // 1/16µStep mode
  accshape = 0;
}


//-------------getErrorMessage-------------
/** @~english
* @brief Get the last Error Message.
*
* This Funktion returns the last Error Message, which occurred in that Class.
* @return ErrorMessage as c-string
*
* @~german
* @brief Gibt die letzte Error Nachricht zurück.
*
* Diese Funktion gibt die Letzte Error Nachricht zurück, welche in dieser Klasse gespeichert wurde.
* @return ErrorMessage als c-string
*/
const char* gnublin_module_step::getErrorMessage() {
  return ErrorMessage.c_str();
}

//-------------setAddress-------------
/** @~english
* @brief Set the slave address
*
* With this function you can set the individual I2C Slave-Address of the module.
* @param Address new I2C slave Address
*
* @~german
* @brief Setzt Slave Adresse.
*
* Mit dieser Funktion kann die individuelle I2C Slave-Adresse des Moduls gesetzt werden.
* @param Address neue I2C slave Adresse
*/
void gnublin_module_step::setAddress(int Address) {
  i2c.setAddress(Address);
}

//-------------setDevicefile-------------
/** @~english
* @brief Set devicefile.
*
* With this function you can change the I2C device file. Default is "/dev/i2c-1"
* @param filename path to the I2C device file
*
* @~german
* @brief Setzt Device Datei.
*
* Mit dieser Funktion kann die I2C Gerätedatei geändert werden. Standartmäßig wird "/dev/i2c-1" benutzt.
* @param filename Pfad zur I2C Gerätedatei
*/
void gnublin_module_step::setDevicefile(std::string filename) {
  i2c.setDevicefile(filename);
}

//-------------setIrun-------------
/** @~english
* @brief Set Irun.
*
* This Funktion sets the new Irun value.
* @param newIrun values from 0 to 15 are possible
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Irun.
*
* Diese Funktion setzt den neuen Irun Wert
* @param newIrun Werte von 0 bis 15
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setIrun(unsigned int newIrun) {
  if(newIrun <= 15) {
    irun = newIrun;
    return 1;
  } else {
    return -1;
  }
}

//-------------setIhold-------------
/** @~english
* @brief Set Ihold.
*
* This Funktion sets the new Ihold value.
* @param newIhold values from 0 to 15 are possible
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Ihold
*
* Diese Funktion setzt den neuen Ihold Wert
* @param newIhold Werte von 0 bis 15
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setIhold(unsigned int newIhold) {
  if(newIhold <= 15) {
    ihold = newIhold;
    return 1;
  } else {
    return -1;
  }
}

//-------------setVmax-------------
/** @~english
* @brief Set Vmax.
*
* This Funktion sets the new Vmax value.
* @param newVmax values from 0 to 15 are possible
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Vmax
*
* Diese Funktion setzt den neuen Vmax Wert
* @param newVmax Werte von 0 bis 15
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setVmax(unsigned int newVmax) {
  if(newVmax <= 15) {
    vmax = newVmax;
    return 1;
  } else {
    return -1;
  }
}

//-------------setVmin-------------
/** @~english
* @brief Set Vmin.
*
* This Funktion sets the new Vmin value.
* @param newVmin values from 0 to 15 are possible
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Vmin
*
* Diese Funktion setzt den neuen Vmin Wert
* @param newVmin Werte von 0 bis 15
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setVmin(unsigned int newVmin) {
  if(newVmin <= 15) {
    vmin = newVmin;
    return 1;
  } else {
    return -1;
  }
}

//-------------setStepMode-------------
/** @~english
* @brief Set Step Mode.
*
* This Funktion sets the new StepMode value.
* @param newStepMode values: StepMode 0=Halfstep, 4=1/4µStep, 8=1/8µStep, 11=1/16µStep
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Step Mode
*
* Diese Funktion setzt den neuen Step Mode Wert
* @param newStepMode Werte: StepMode 0=Halfstep, 4=1/4µStep, 8=1/8µStep, 11=1/16µStep
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setStepMode(unsigned int newStepMode) {
  if((newStepMode == 0) || (newStepMode == 4) || (newStepMode == 8) || (newStepMode == 11)) {
    stepmode = newStepMode;
    return 1;
  } else {
    return -1;
  }
}

//-------------setAccShape-------------
/** @~english
* @brief Set Acc Shape.
*
* This Funktion sets the new AccShape value.
* @param newAccShape values: 0=AccShape off, 1=AccShape on
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Acc Shape
*
* Diese Funktion setzt den neuen AccShape Wert
* @param newAccShape Werte: : 0=AccShape aus, 1=AccShape ein
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setAccShape(unsigned int newAccShape) {
  if((newAccShape == 0) || (newAccShape == 1)) {
    accshape = newAccShape;
    return 1;
  } else {
    return -1;
  }
}

//-------------writeTMC-------------
/** @~english
* @brief Write to TMC.
*
* This funktion sends the amount of bytes (num) of the char array TxBuf to the TMC222 chip on the module-step.
* @param TxBuf Send buffer
* @param num amount of Bytes
* @return success: 1, failure: -1
*
* @~german
* @brief Sende an TMC.
*
* Diese Funktion sendet die Menge (num) an Bytes des Char Array TxBuf an den TMC222 Chip, welcher sich auf dem Moudule-step befindet.
* @param TxBuf Sende Puffer
* @param num Anzahl der zu sendenden Bytes
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::writeTMC(unsigned char* TxBuf, int num) {
  if(!i2c.send(TxBuf, num)) {
    return -1;
  } else {
    return 1;
  }
}

//-------------readTMC-------------
/** @~english
* @brief Read from TMC.
*
* This funktion reads the amount of bytes (num) of the TMC222 chip on the module-step and write it to the char array RxBuf.
* @param RxBuf Receive buffer
* @param num amount of Bytes
* @return success: 1, failure: -1
*
* @~german
* @brief Sende an TMC.
*
* Diese Funktion ließt die Menge (num) an Bytes vom TMC222 Chip, welcher sich auf dem Moudule-step befindet und übergibt das Ergebnis im Char Array RxBuf.
* @param RxBuf Empfangs Puffer
* @param num Anzahl der zu lesenden Bytes
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::readTMC(unsigned char* RxBuf, int num) {
  if(!i2c.receive(RxBuf, num)) {
    return -1;
  } else {
    return 1;
  }
}

//-------------burnNewAddress-------------
/** @~english
* @brief Burn a new I2C slave Address.
*
* This function calculates the needet bits to burn on the TMC222 chip, to get the new given slave-address. After that, the OTP bits are burned into the Chip.
* @param new_address the new slave address
* @return success: 1, failure: -1
*
* @~german
* @brief Brennt die übergebene slave Adresse.
*
* Diese Funktion berechnet die benötigten Bits, um die neue Adresse auf den Chip zu brennen. Anschließend werden die OTP bits gebrannt.
* Achtung: Sind die Bits einmal gesetzt, kann dies nicht wieder rückgängig gemacht werden. Falls eine Adresse eingegeben wird, die nicht möglich ist, wird dies von der Funktion gemeldet.
*
* @param new_address die neue Slave Adresse
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::burnNewAddress(int new_address) {

  //SetOTPParam
  unsigned char buffer[5];
  int new_ad = 0;
  int old_ad = 0;
  int slave_address = i2c.getAddress();
  std::string yes = "NO!";

  if(new_address <= slave_address) {
    printf("\tThe new address must be higher than the old one (0x%x)!\n", slave_address);
    return -1;
  } else if(new_address > 0x7f) {
    printf("The biggest slave address of the TMC222 is 0x7f\n");
    return -1;
  } else {
    old_ad = (slave_address & 0x1e) >> 1;
    new_ad = (new_address & 0x1e) >> 1;
    if(((new_ad & 0x1) < (old_ad & 0x1)) | ((new_ad & 0x2) < (old_ad & 0x2)) | ((new_ad & 0x4) < (old_ad & 0x4)) | ((new_ad & 0x8) < (old_ad & 0x8))) {
      printf("\tThis address could not be set, because the '1' cant be undone!\n"
             "\told OTP AD: 0x%x\n"
             "\tnew OTP AD: 0x%x\n", old_ad, new_ad);
      return -1;
    }
    if((new_address & 0x01) == 1) {
      printf("\tThe LSB address bit is set by the jumper on the module-step\n");
      new_address --;
      printf("\tThe new address will be set to: 0x%x \n", new_address);
    }

    printf("\tIf a bit of the OTP (on time programmable) is set, it cant be undone! \n\t"
           "If you are sure to write the new Address (0x%x) then type 'yes' in CAPITALS\n\n\t", new_address);
    std::cin >> yes;
    if(yes == "YES") {
      buffer[0] = 0x90; //SetOTPParam
      buffer[1] = 0xff; //N/A
      buffer[2] = 0xff; //N/A
      buffer[3] = 0x02; //set AD3 AD2 AD1 AD0
      buffer[4] = (unsigned char) new_ad;

      if(!i2c.send(buffer, 5)) {
        return -1;
      } else {
        printf("\tNew Address was successfully set to: 0x%x\n\tPlease replug the module.\n\n", new_address);
        return 1;
      }
    } else {
      printf("\tYou didn't type 'YES'\n");
      return -1;
    }
  }
}

//-------------getFullStatus-------------
/** @~english
* @brief Get full Status 1.
*
* This funktion sends the getFullStatus1 Command to the Chip.
* @return success: 1, failure: -1
*
* @~german
* @brief Get full Status 1.
*
* Diese Funktion sendet den getFullStatus1 Befehl an den Chip.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::getFullStatus1() {
  if(i2c.send(0x81)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------getFullStatus2-------------
/** @~english
* @brief Get full Status 2.
*
* This funktion sends the getFullStatus2 Command to the Chip.
* @return success: 1, failure: -1
*
* @~german
* @brief Get full Status 2.
*
* Diese Funktion sendet den getFullStatus2 Befehl an den Chip.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::getFullStatus2() {
  if(i2c.send(0xfc)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------runInit-------------
/** @~english
* @brief Run Init.
*
* This funktion sends the runInit Command to the Chip.
* @return success: 1, failure: -1
*
* @~german
* @brief Run Init.
*
* Diese Funktion sendet den runInit Befehl an den Chip.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::runInit() {
  if(i2c.send(0x88)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------setMotorParam-------------
/** @~english
* @brief Set motor parameter.
*
* This function sends the motor parameter to the module.
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Motor Parameter.
*
* Diese Funktion sendet die eingestellten Motor Parameter an das Modul.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setMotorParam() {
  unsigned char buffer[8];
  //SetMotorParam
  buffer[0] = 0x89; //SetMotorParam
  buffer[1] = 0xff; //N/A
  buffer[2] = 0xff; //N/A
  buffer[3] = (unsigned char)((irun * 0x10) + ihold);  //Irun & I hold
  buffer[4] = (unsigned char)((vmax * 0x10) + vmin);  //Vmax & Vmin
  buffer[5] = 0x00; //status
  buffer[6] = 0x00; //securePos
  buffer[7] = (unsigned char)((accshape * 0x10) + stepmode);  //AccShape & StepMode 00=Halfstep, 04=1/4µStep, 08=1/8µStep, 0c=1/16µStep

  if(i2c.send(buffer, 8)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------setMotorParam-------------
/** @~english
* @brief Set motor parameter.
*
* This function sends the motor parameter to the module.
* @param newIrun
* @param newIhold
* @param newVmax
* @param newVmin
* @param newAccShape
* @param newStepMode
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Motor Parameter.
*
* Diese Funktion sendet die übergebenen Motor Parameter an das Modul.
* @param newIrun
* @param newIhold
* @param newVmax
* @param newVmin
* @param newAccShape
* @param newStepMode
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setMotorParam(unsigned int newIrun, unsigned int newIhold, unsigned int newVmax, unsigned int newVmin, unsigned int newAccShape, unsigned int newStepMode) {
  irun = newIrun;
  ihold = newIhold;
  vmax = newVmax;
  vmin = newVmin;
  accshape = newAccShape;
  stepmode = newStepMode;

  unsigned char buffer[8];
  //SetMotorParam
  buffer[0] = 0x89; //SetMotorParam
  buffer[1] = 0xff; //N/A
  buffer[2] = 0xff; //N/A
  buffer[3] = (unsigned char)((irun * 0x10) + ihold);  //Irun & I hold
  buffer[4] = (unsigned char)((vmax * 0x10) + vmin);  //Vmax & Vmin
  buffer[5] = 0x00; //status
  buffer[6] = 0x00; //securePos
  buffer[7] = (unsigned char)((accshape * 0x10) + stepmode);  //AccShape & StepMode 00=Halfstep, 04=1/4µStep, 08=1/8µStep, 0c=1/16µStep

  if(i2c.send(buffer, 8)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------hardStop-------------
/** @~english
* @brief Hard stop.
*
* This funktion sends the hardStop Command to the Chip. The motor will stop immediately.
* @return success: 1, failure: -1
*
* @~german
* @brief Hard Stop.
*
* Diese Funktion sendet den hardStop Befehl an den Chip. Der Motor bleibt sofort Stehen.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::hardStop() {
  if(i2c.send(0x85)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------softStop-------------
/** @~english
* @brief Soft stop.
*
* This funktion sends the softStop Command to the Chip. The motor slows down and stops.
* @return success: 1, failure: -1
*
* @~german
* @brief Soft Stop.
*
* Diese Funktion sendet den SoftStop Befehl an den Chip. Der Motor fährt langsam herrunter.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::softStop() {
  if(i2c.send(0x8f)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------resetPosition-------------
/** @~english
* @brief Reset Position.
*
* This funktion sends the resetPosition Command to the Chip. The internal position counter of the chip is reset to 0.
* @return success: 1, failure: -1
*
* @~german
* @brief Reset Position.
*
* Diese Funktion sendet den resetPosition Befehl an den Chip. Der interne Positions Zähler des Chips wird auf 0 zurückgesetzt.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::resetPosition() {
  if(i2c.send(0x86)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------setPosition-------------
/** @~english
* @brief Set Position.
*
* This funktion sends the Position Command to the Chip. The motor drives to the given position.
* @return success: 1, failure: -1
*
* @~german
* @brief Setze Position.
*
* Diese Funktion sendet den Position Befehl an den Chip. Der Motor fährt an die übergebene Position.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::setPosition(int position) {
  unsigned char buffer[5];
  buffer[0] = 0x8B;   // SetPosition Command
  buffer[1] = 0xff;   // not avialable
  buffer[2] = 0xff;   // not avialable
  buffer[3] = (unsigned char)(position >> 8);   // PositionByte1 (15:8)
  buffer[4] = (unsigned char)  position;       // PositionByte2 (7:0)

  if(i2c.send(buffer, 5)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------drive-------------
/** @~english
* @brief Drive.
*
* This Funktion reads the actual position from the motor and adds the amount of given steps to drive. So you can let the motor drive an amount of steps, without heaving trouble with the absolute positions.
* @return success: 1, failure: -1
*
* @~german
* @brief Fahre.
*
* Diese Funktion ließt die aktuelle Position des Motors und addiert die anzahl der übergebenen Schritte. So kann man den Motor einfach um eine bestimmte Anzahl Schritte fahren lassen, ohne sich über die absoulute Position gedanken machen zu müssen.
* @return Erfolg: 1, Fehler: -1
*/
int gnublin_module_step::drive(int steps) {
  int old_position;
  int new_position;

  old_position = getActualPosition();
  new_position = old_position + steps;
  if(setPosition(new_position)) {
    return 1;
  } else {
    return -1;
  }
}

//-------------getMotionStatus-------------
/** @~english
* @brief Get motion status.
*
* This funktion Indicates the actual behavior of the position controller. <br>
* 0: Actual Position = Target Position; Velocity= 0 <br>
* 1: Positive Acceleration; Velocity > 0 <br>
* 2: Negative Acceleration; Velocity > 0 <br>
* 3: Acceleration = 0 Velocity = maximum pos Velocity <br>
* 4: Actual Position /= Target Position; Velocity= 0 <br>
* 5: Positive Acceleration; Velocity < 0 <br>
* 6: Positive Acceleration; Velocity < 0 <br>
* 7: Acceleration = 0 Velocity = maximum neg Velocity <br>
* -1: failure
*
* @return motionStatus
*
* @~german
* @brief Bewegungs-Status.
*
* Diese Funktion gibt den aktuellen bewegungs Status des module-step zurück. <br>
* 0: Aktuelle Position = Ziehl Position; Geschwindigkeit= 0 <br>
* 1: Positive Beschleunigung; Geschwindigkeit > 0 <br>
* 2: Negative Beschleunigung; Geschwindigkeit > 0 <br>
* 3: Beschleunigung = 0; Geschwindigkeit = maximum pos Geschwindigkeit <br>
* 4: Aktuelle Position /= Ziehl Position; Geschwindigkeit= 0 <br>
* 5: Positive Beschleunigung; Geschwindigkeit < 0 <br>
* 6: Positive Beschleunigung; Geschwindigkeit < 0 <br>
* 7: Acceleration = 0 Geschwindigkeit = maximum neg Geschwindigkeit  <br>
* -1: Fehler
* @return motionStatus
*/
int gnublin_module_step::getMotionStatus() {
  unsigned char RxBuf[8];
  int motionStatus = -1;
  if(getFullStatus1() == -1) {
    return -1;
  }


  if(!i2c.receive(RxBuf, 8)) {
    return -1;
  }
  motionStatus = (RxBuf[5] & 0xe0) >> 5;
  return motionStatus;

}


//-------------------get Switch status----------------
/** @~english
* @brief Get switch condition.
*
* This function checks the status of the connected switch and returns the value. <br>
* Switch closed: 1 <br>
* Switch open: 0 <br>
* Failure: -1
* @return success: swi
*
* @~german
* @brief Setze Position.
*
* Diese Funktion überprüft den Zustand des angeschlossenen Schalters und gibt den Wert zurück. <br>
* Schalter geschlossen: 1 <br>
* Schalter offen: 0 <br>
* Fehler: -1
* @return swi
*/
int gnublin_module_step::getSwitch() {
  unsigned char RxBuf[8];
  int swi = 0;

  if(getFullStatus1() == -1) {
    return -1;
  }


  if(i2c.receive(RxBuf, 8)) {

    if(RxBuf[5] & 0x10) {
      swi = 1;
    } else {
      swi = 0;
    }
    return swi;
  } else {
    return -1;
  }
}

//-------------------getActualPosition----------------
/** @~english
* @brief Get actual position.
*
* This funktion sends the getActualPosition Command to the Chip and returns its actiual position.
* @return actualPosition -1 if failure
*
* @~german
* @brief Aktuelle Position ausgeben.
*
* Diese Funktion sendet den getActualPosition Befehl an den Chip und gibt dessen aktuelle Position zurück.
* @return actualPosition -1 bei Fehler
*/
int gnublin_module_step::getActualPosition() {
  unsigned char RxBuf[8];
  int actualPosition = -1;

  if(getFullStatus2() == -1) {
    return -1;
  }

  if(i2c.receive(RxBuf, 8)) {
    actualPosition = (RxBuf[1] << 8 | RxBuf[2]);
  }
  return actualPosition;
}


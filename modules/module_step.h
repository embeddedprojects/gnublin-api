/**
* @class gnublin_module_step
* @~english
* @brief Class for accessing GNUBLIN module-step
*
* The GNUBLIN Module-step can be easily controlled with the gnublin_step API. The Module uses the I2C-Bus.
* @~german
* @brief Klasse für den zugriff auf das GNUBLIN module-step
*
* Das GNUBLIN module-step lässt sich mit Hilfe der gnublin_step API ganz einfach ansteuern. Das Modul nutzt die I2C-Schnittstelle.
*/
class gnublin_module_step {
    gnublin_i2c i2c;

    unsigned int irun;
    unsigned int ihold;
    unsigned int vmax;
    unsigned int vmin;
    unsigned int accshape;
    unsigned int stepmode;
    std::string ErrorMessage;
  public:
    gnublin_module_step();
    void setAddress(int Address);
    void setDevicefile(std::string filename);
    int setIrun(unsigned int newIrun);
    int setIhold(unsigned int newIhold);
    int setVmax(unsigned int newVmax);
    int setVmin(unsigned int newVmin);
    int setStepMode(unsigned int newStepMode);
    int setAccShape(unsigned int newAccShape);
    int writeTMC(unsigned char* buffer, int num);
    int readTMC(unsigned char* RxBuf, int num);
    int burnNewAddress(int new_address);
    int getFullStatus1();
    int getFullStatus2();
    int runInit();
    int setMotorParam();
    int setMotorParam(unsigned int newIrun, unsigned int newIhold, unsigned int newVmax, unsigned int newVmin, unsigned int newAccShape, unsigned int newStepMode);
    int hardStop();
    int softStop();
    int resetPosition();
    int setPosition(int position);
    int getSwitch();
    int getActualPosition();
    int drive(int steps);
    int getMotionStatus();
    const char* getErrorMessage();
};

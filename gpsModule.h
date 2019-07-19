#include "mbed.h"

class gpsModule
{
public:
    //Prototypes
    void pullGPGGAData(char *);
    void pullGPVTGData(char *);
    void pullGPRMCData(char *);
    double convertDMStoDD(double *);
    float getLatitude();
    float getLongitude();
    float getTime();
    int getLock();
    int getSats();
    int getRawLon();
    int getRawLat();
    float getMSLAltitude();
    int getDate();
    float getCourseOverGround();
    float getCourse();
    float getKnots();
    float getKMH();

    //Debug Only - MOVE to private.
    //void getSerial(char*);

    // Constructor
    gpsModule(PinName tx, PinName rx, int baud): serial(tx, rx, baud)
    {
        setMessageMode();
    };
    // Destructor
    //~gpsModule();

private:
    //Private Prototypes
    void setMessageMode();
    void getSerial(char*);
    Serial serial;
    
    //Private Vars
    int haveLock;
    int Sats;
    float time;
    float MeanSeaLevelAltitude;
    float HorizontalDilutionOfPrecision;
    double longitude;
    double latitude;
    char NSindicator;
    char EWindicator;
    int lon;
    int lat;

    float course;
    char courseRef;
    float magCourse; //Nothing shows in Serial Monitor!
    char magRef;
    float knots;
    char knotsUnits;
    float kilometers;
    char kiloUnits;

    char status;
    float courseOverGround;
    int date;    
};

// Get Serial Data from the GPS Module. Single Line, not a GULP!
void gpsModule::getSerial(char *retMes)
{
    // Get some GPS data                                   V
    while(serial.getc() != '$'); //Wait for start char. ie $GPGGA
    for (unsigned int j = 0; j < 256; j++) {
        retMes[j] = serial.getc();
        if (retMes[j] == '\n' || retMes[j] == '\r') { // if end of message.
            break;
        }
    }
}

//GGA - Global Positioning System Fixed Data
void gpsModule::pullGPGGAData(char *msg)
{
    //Get some GPGGA Data.*
    do {
    getSerial(msg); 
    }while(msg[2] != 'G' && msg[3] != 'G' && msg[4] != 'A');
    
    if (sscanf(msg,"GPGGA,%f,%lf,%c,%lf,%c,%d,%d,%f,%f", &time, &latitude, &NSindicator, &longitude, &EWindicator, &haveLock, &Sats, &HorizontalDilutionOfPrecision, &MeanSeaLevelAltitude) >=1) {
        if (NSindicator == 'S') { //We are in the southern hemi, therefore we need a negative value (is positive from GPS ?)
            latitude = latitude * -1;
        }
        lon = longitude * 10000.0;
        lat = latitude * 10000.0;
    } else {
        msg[0] = 'E';
        msg[1] = 'R';
        msg[2] = 'R';
        msg[3] = '!';
        msg[4] = '\n';
        msg[5] = '\r';
    }
}

//VTG-Course Over Ground and Ground Speed
void gpsModule::pullGPVTGData(char *msg)
{
    do {
    getSerial(msg);
    }while(msg[2] != 'V' && msg[3] != 'T' && msg[4] != 'G');

    if (sscanf(msg,"GPVTG,%f,%c,,%c,%f,%c,%f", &course, &courseRef, &magRef, &knots, &knotsUnits, &kilometers) >=1) {
    } else {
        msg[0] = 'E';
        msg[1] = 'R';
        msg[2] = 'R';
        msg[3] = '!';
        msg[4] = '\n';
    }
}

//RMC-Recommended Minimum Specific GNSS Data
void gpsModule::pullGPRMCData(char *msg)
{ 
    do {
    getSerial(msg); 
    }while(msg[2] != 'R' && msg[3] != 'M' && msg[4] != 'C');
 
    if (sscanf(msg,"GPRMC,%f,%c,%lf,%c,%lf,%c,%f,%f,%i", &time, &status, &latitude, &NSindicator, &longitude, &EWindicator, &knots, &courseOverGround, &date) >=1) {
       
        if (NSindicator == 'S') {
            //We are in the southern hemi, therefore we need a negative value (is positive from GPS ?)
            latitude = latitude * -1;
        }
        lon = longitude * 10000.0;
        lat = latitude * 10000.0;
    } else {
        msg[0] = 'E';
        msg[1] = 'R';
        msg[2] = 'R';
        msg[3] = '!';
        msg[4] = '\n';
    }
}

//Degrees, Minutes, Seconds to Decimal Degrees 
//Coverts Raw GPS longitude and latitude data (Degrees and minuites - DDMM.mmmm) to decimal degrees.
double gpsModule::convertDMStoDD(double *rawGPS)
{
    int degrees = (*rawGPS) / 100;
    float minutes = (*rawGPS - (degrees * 100));
    minutes = minutes / 60.0;
    float decimalDegrees = degrees + minutes;
    return decimalDegrees;
}

//Getters!
int gpsModule::getRawLat() //Interger, divide by 10,000
{
    return lat;
}

int gpsModule::getRawLon() //Interger, divide by 10,000
{
    return lon;
}

float gpsModule::getLatitude() //Returns lat with float precision error.
{
    return latitude;
}

float gpsModule::getLongitude() //Returns longitude with float precision error.
{
    return longitude;
}

float gpsModule::getTime() //GPS Time.
{
    return time;
}

int gpsModule::getLock() //GPS sat lock.
{
    return haveLock;
}

int gpsModule::getSats() //Return Number of satellite used.
{
    return Sats;
}

float gpsModule::getMSLAltitude() //Return Mean Sea Level Altitude
{
    return MeanSeaLevelAltitude;
}

float gpsModule::getCourse() //Return Heading
{
    return course;
}

float gpsModule::getKnots() // Return Knots
{
    return knots;
}

float gpsModule::getKMH() // Return km/h
{
    return kilometers;
}

int gpsModule::getDate() // Return GPS date.
{
    return date;
}

float gpsModule::getCourseOverGround() // Return Course Over Ground.
{
    return courseOverGround;
}

void gpsModule::setMessageMode()
{
    //Set the NMEA strings you need, turn off the ones you dont.
    //chechsum calculator - ie. PSRF103,1,0,0,1 = *25 -> https://nmeachecksum.eqth.net/
    //$PSRF103,<msg>,<mode>,<rate>,<cksumEnable>*CKSUM<CR><LF>
    //<msg> 0=GGA, 1=GLL, 2=GSA, 3=GSV, 4=RMC, 5=VTG
    //<mode> 0=SetRate,1=Query

    serial.printf("$PSRF103,0,0,1,1*25\r\n");//Enable GGA
    //serial.printf("$PSRF103,0,0,0,1*24\r\n");//Disable GGA

    //serial.printf("$PSRF103,1,0,1,1*24\r\n");//Enable GLL
    serial.printf("$PSRF103,1,0,0,1*25\r\n");//Disable GLL

    //serial.printf("$PSRF103,2,0,1,1*27\r\n");//Enable GSA
    serial.printf("$PSRF103,2,0,0,1*26\r\n");//Disable GSA

    //serial.printf("$PSRF103,3,0,1,1*26\r\n");//Enable GSV
    serial.printf("$PSRF103,3,0,0,1*27\r\n");//Disable GSV

    serial.printf("$PSRF103,4,0,1,1*21\r\n");//Enable RMC
    //serial.printf("$PSRF103,4,0,0,1*20\r\n");//Disable RMC

    serial.printf("$PSRF103,5,0,1,1*20\r\n");//Enable VTG
    //serial.printf("$PSRF103,5,0,0,1*21\r\n");//Disable VTG
}

#include "mbed.h"
#include "gpsModule.h"
#include "pointInPolyCalculator.h"
#include "gpsCoordsFile.h"


//prototypes 
void testGPS(gpsModule *gps);
void testPointInPoly(gpsCoordsFile *gpsFile, gpsModule *gps, pointInPolyCalculator *polyCalc);
//

DigitalOut myled(LED1);
Serial pc(USBTX,USBRX,115200);//PC


int main() {
    
    char fName[21] = "/sd/mydir/sdtest.txt";
    
    
    pointInPolyCalculator polyCalc;
  //gpsCoordsFile x(<file name>, <mosi>, <miso>, <sclk>, <cd>)
    gpsCoordsFile gpsFile(fName, D4, D5, D3, D8);
    gpsModule gps(A0, A1, 4800); //GPS instance
    
    pc.printf("\r\nGPS Class Test! ->RUNNING\r\n");
    wait(3);
    
    /*while(1) //Debug
    {
        gps.getSerial(msg); //private member!
        pc.printf("%s", msg);
        pc.printf("\r\r\r\n\n\n");
    }*/
    
    while(1)
    {
        testPointInPoly(&gpsFile, &gps, &polyCalc);
        wait(2);
    }
    //testGPS(&gps);
   
    

}

void testPointInPoly(gpsCoordsFile *gpsFile, gpsModule *gps, pointInPolyCalculator *polyCalc)
{
    char location[80];
    bool response = false;
    double correctLongitude;
    double correctLatitude;
    char gga[150];
    const unsigned int polySides = 5; // 4 sides in your polygon (<5)
    int pS = 5;
    float polyX[polySides];      //= {  -31.863979, -31.863923, -31.864014,-31.864081 , -31.864161}; // X    <- Eg.
    float polyY[polySides];     // = {  115.949594, 115.950019,115.950048, 115.950005 , 115.949652}; //Y
    float latDD;
    float lonDD;
    
    gpsFile->getGPSCoords(0, location, &polyX[0], &polyY[0]);
    
    pc.printf("Checking location = %s\r\n", location);
    
    //Get some GPGGA Data. GGA-Global Positioning System Fixed Data. - You can use the same var and call each data pull then do what you need then pull the next. 
    gps->pullGPGGAData(gga); 

    
    correctLatitude = (gps->getRawLat()) / 10000.0;
    correctLongitude = (gps->getRawLon()) / 10000.0; //Precision getRawLon returns an int.
    
    pc.printf("CorrecLat = %f\r\n", correctLatitude);
    pc.printf("CorrecLon = %f\r\n", correctLongitude);
       
    latDD = gps->convertDMStoDD(&correctLatitude);
    lonDD = gps->convertDMStoDD(&correctLongitude);
    pc.printf("latDD = %f\r\n", latDD);
    pc.printf("lonDD = %f\r\n", lonDD);
    
    response = polyCalc->pointInPolygon(&latDD, &lonDD, &pS, polyX, polyY);
    pc.printf("Response = %i\r\n", response);
    
}


void testGPS(gpsModule *gps)
{
    const int boarder = 100;
    char gga[150];
    char vtg[150];
    char rmc[150];
    double incorrectLatitude;
    double incorrectLongitude;
    double correctLongitude;
    double correctLatitude;

        while(1)
    {
        //Get some GPGGA Data. GGA-Global Positioning System Fixed Data. - You can use the same var and call each data pull then do what you need then pull the next. 
        gps->pullGPGGAData(gga); 

        //Get some GPRMC Data. RMC-Recommended Minimum Specific GNSS Data.
        gps->pullGPRMCData(rmc);
       
        //Get some GPVTG Data. VTG-Course Over Ground and Ground Speed.
        gps->pullGPVTGData(vtg);
        
        //The float conversions inside the class cause small precision errors.
        //To get around this we convert them to INTs and then, convert them to floats as we need them,
        //See correctLongitude below.
        incorrectLatitude = gps->getLatitude(); 
        incorrectLongitude = gps->getLongitude(); //Last three to four decimal places are WRONG!, estimates. Real Bad Shit; RBS.
        
        pc.printf("\r\n");
        pc.printf("\r\n");
        pc.printf("GPS GGA String : %s\r\n", gga);
        pc.printf("Have Lock = %i\r\n", gps->getLock());
        pc.printf("Time = %.0f\r\n", gps->getTime()+ 80000);
        pc.printf("Sats = %i\r\n", gps->getSats());
        pc.printf("MSL Altitude = %.1f\r\n", gps->getMSLAltitude());      
        pc.printf("Incorrect Latitude = %f\r\n", incorrectLatitude);
        pc.printf("Incorrect Longatude = %f\r\n", incorrectLongitude);
        
        //Convert to INTs to floats
        correctLongitude = (gps->getRawLon()) / 10000.0; //Precision getRawLon returns an int.
        correctLatitude = (gps->getRawLat()) / 10000.0;
        pc.printf("Correct Latitude =  %f\r\n", correctLatitude);
        pc.printf("Correct Longitude = %f\r\n", correctLongitude);
        pc.printf("Decimal Degrees Latitude = %f\r\n", gps->convertDMStoDD(&correctLatitude));
        pc.printf("Decimal Degrees Longitude = %f\r\n", gps->convertDMStoDD(&correctLongitude));
        pc.printf("\r\n");  
               
        pc.printf("GPS RMC String : %s\r\n", rmc);
        pc.printf("Date = %i\r\n", gps->getDate());
        correctLongitude = (gps->getRawLon()) / 10000.0; //precision getRawLon returns an int.
        correctLatitude = (gps->getRawLat()) / 10000.0;
        pc.printf("Correct Latitude =  %f\r\n", correctLatitude);
        pc.printf("Correct Longitude = %f\r\n", correctLongitude);
        pc.printf("Course over ground = %f\r\n", gps->getCourseOverGround());
        pc.printf("\r\n");  
        
        pc.printf("GPS VTG String : %s\r\n", vtg);
        pc.printf("Course Degrees = %.2f\r\n", gps->getCourse());
        pc.printf("Knots = %.2f\r\n", gps->getKnots());
        pc.printf("km/h = %.2f\r\n", gps->getKMH());
        pc.printf("\r\n");   
        
        for (unsigned int i = 0; i < boarder; i++)
        {
            pc.printf("-");
        }  
    }
}

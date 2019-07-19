#include "mbed.h"
#include "SDFileSystem.h"

class gpsCoordsFile
{
    public:
    //Prototypes
    void getGPSCoords(int, char*, float*, float*);
    void getLine(FILE *, char *, char *, char *, int);
    
    //Constructor
    gpsCoordsFile(char *coordsFile,  PinName mosi, PinName miso, PinName sclk, PinName cs)
    {
        //strcpy(fileName, coordsFile);
       // fileName = "/sd/mydir/sdtest.txt";
        strncpy(fileName, coordsFile, sizeof(fileName));
        printf("%s  ", fileName);
        printf("\r\n");
        
        this->mosi = mosi;
        this->miso = miso;
        this->sclk = sclk;
        this->cs = cs;
        //SDFileSystem sd(mosi, miso, sclk, cs, "sd"); 
    }
    
    //~gpsCoordsFile();
    
    private:
    char fileName[30];
    PinName mosi;
    PinName miso;
    PinName sclk;
    PinName cs;

    
};

//GETS current GPS coordinates saved on SD card Note: uses pointers.
//lineNumber is which line to read. Which will be the Location Line. The next lines will be read also.
//Location is the GPS coordinates Name. Line 1.
//Float X and Y are arrays to fill with gps coords. Line 2 and 3.

void gpsCoordsFile::getGPSCoords(int lineNumber, char* location, float* X, float* Y)
{
    SDFileSystem sd(gpsCoordsFile::mosi, gpsCoordsFile::miso, gpsCoordsFile::sclk, gpsCoordsFile::cs, "sd"); 
    FILE *fp = fopen(fileName, "r");
    if (fp != NULL) {
        char bufferX[80] = {0};
        char bufferY[80] = {0};
        char locBuf[80] = {0};

        float G,H,I,J,K;
        //Get the X and Y Coordinates
        getLine(fp, locBuf, bufferX, bufferY, lineNumber);
        sscanf(locBuf,"%s", location);
        //printf("locatation = : %s \n\r", location); //DEBUG
        // pull out floats from char array X.
        sscanf(bufferX,"%f, %f, %f, %f, %f,",&G,&H,&I,&J,&K);
        // Adjust pointers values
        *(X) = G;
        *(X+1) = H;
        *(X+2) = I;
        *(X+3) = J;
        *(X+4) = K;
        // pull out floats from char array Y.
        sscanf(bufferY,"%f, %f, %f, %f, %f,",&G,&H,&I,&J,&K);
        *(Y) = G;
        *(Y+1) = H;
        *(Y+2) = I;
        *(Y+3) = J;
        *(Y+4) = K;

        fclose(fp);
    } else {
        printf("read: failed!\r\n");
        while(1) { //Lock it up.
            //GREEN = 1;
            //wait(0.25);
            //GREEN = 0;
            //wait(0.25);
        }
    }
}

// getGPSCoords Helper, Iterates through file to get required line.
void gpsCoordsFile::getLine(FILE *FP, char *locBuf, char *latBuf, char *lonBuf, int lineCounter)
{
    int count = 0;
    char dummy[80] = {0};
    while(!feof(FP)) {

        if(count == lineCounter) {
            fgets(locBuf, '\n\r', FP);
            fgets(latBuf, '\n\r', FP);
            fgets(lonBuf, '\n\r', FP);
            break;
        } else {
            fgets(dummy, '\n\r', FP);
        }
        count++;
    }
}

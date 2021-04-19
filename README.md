# GPS_MBED
GPS class and helper demo
Point In a Polygon. See if device is inside a pre defined shape mapped out using GPS coordinates.
See main.cpp

'sdtest.txt' should be set out as so.     
Location,       
Latitude, Latitude, Latitude, Latitude etc,  
Longatude, Longatude, Longatude, Longatude etc.  

for example,  
Marks_House,    
-31.863979, -31.863923, -31.864014, -31.864081, -31.864161,    
115.949594, 115.950019, 115.950048, 115.950005, 115.949652,    

and in folder on sd card.     
/mydir/sdtest.txt      

Make sure your coodinates start at the top right and go clockwise round the polygon.       

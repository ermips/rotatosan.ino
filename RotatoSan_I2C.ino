#include <Wire.h>
 
 int reading = 0;
void setup()
{
  //Wire.begin();
  Wire.begin (21, 22);   // sda= GPIO_21 /scl= GPIO_22   SDA 21 SCL 18
  Serial.begin(9600);
scanI2C();
 Serial.println("start");


       
}
 
 
void loop()
{
//T9602-3-D
 showthedata();

}


void scanI2C(){
   byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
        Serial.print(address);
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0){
  Serial.print("Adresse :");
  Serial.println(address);
    Serial.println("No I2C devices found\n");
  }
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}

void getdata(byte *a, byte *b, byte *c, byte *d)
{
  Wire.beginTransmission(0x28);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(0x28, 4);
  *a = Wire.read();
  *b = Wire.read();
  *c = Wire.read();
  *d = Wire.read();
}
 void showthedata()
{
  byte aa,bb,cc,dd;
  float temperature=0.0;  float humidity=0.0;
  getdata(&aa,&bb,&cc,&dd);
 
// humidity = (rH_High [5:0] x 256 + rH_Low [7:0]) / 16384 x 100
  humidity = (float)(((aa & 0x3F ) << 8) + bb) / 16384.0 * 100.0;

// temperature = (Temp_High [7:0] x 64 + Temp_Low [7:2]/4 ) / 16384 x 165 - 40
  temperature = (float)((unsigned)(cc  * 64) + (unsigned)(dd >> 2 )) / 16384.0 * 165.0 - 40.0;

//static void cc2_calculations(void)
//{
//  raw_rh =
//    (uint32_t)(((uint16_t)(i2c_buf[0] & 0x3F) * 256) + (uint16_t)i2c_buf[1]);
//  cooked_rh = ((double)raw_rh / 16384.0) * 100.0;

//  raw_tc =
//    (int32_t)(((uint16_t)i2c_buf[2] * 64) + ((uint16_t)(i2c_buf[3] & 0xFC) >> 2));
//  cooked_tc = ((double)raw_tc / 16384.0) * 165.0 - 40.0;
//} // cc2_calculations



Serial.print(temperature);Serial.print(" degC  ");Serial.print(humidity);Serial.println(" %rH");;
 delay(5000);  
}

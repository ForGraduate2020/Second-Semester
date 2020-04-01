#include <SPI.h>
#include <SD.h>

#define BUFSIZE 1024

unsigned long timer = 0;
unsigned long globalTimer = 0;
unsigned long initialTime = 0;
const int micPin = 0;
const int bufferSize = 441;

unsigned long oldMicro = micros();
byte sampleBuffer[BUFSIZE];
byte oldSample = 0;

int sampleIndex = 0;

PROGMEM const byte header [44] =
  // This contains the header of a WAV file. Without this, it is not recognized as such.
  // 44 bytes long.
{
  0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45,
  0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
  0x00, 0x0F, 0x00, 0x00, 0x40, 0x1F, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00,
  0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00
};

File myFile;

void WriteHeader()
{
  for (byte pos = 0; pos < sizeof(header); pos++)
  {
    myFile.write(pgm_read_byte(&header[pos]));
  }
}

void Finalize()
{
  // 4  : format chunk size = file size - 8
  // 24 : sample rate = file size - 44 / record time
  // 40 : data chunk size = file size - 44

  byte finalValue[4];
  unsigned long fileSize = myFile.size();
  unsigned long riffSize = fileSize - 8;
  unsigned long dataSize = fileSize - 44;
  unsigned long sampleRate = dataSize / 20;  // sample count / sec

  Serial.print("sample rate : ");
  Serial.println(sampleRate);

  // write format chunk size
  finalValue[0] = riffSize & 0xff;
  finalValue[1] = (riffSize >> 8) & 0xff;
  finalValue[2] = (riffSize >> 16) & 0xff;
  finalValue[3] = (riffSize >> 24) & 0xff;
  // Is possible to make a fuction that returns an array of bytes?

  myFile.seek(4);
  myFile.write(finalValue, 4);

  // write data chunk size
  finalValue[0] = dataSize & 0xff;
  finalValue[1] = (dataSize >> 8) & 0xff;
  finalValue[2] = (dataSize >> 16) & 0xff;
  finalValue[3] = (dataSize >> 24) & 0xff;
  // Is possible to make a fuction that returns an array of bytes?

  myFile.seek(40);
  myFile.write(finalValue, 4);

  // write sample rate
  finalValue[0] = sampleRate & 0xff;
  finalValue[1] = (sampleRate >> 8) & 0xff;
  finalValue[2] = (sampleRate >> 16) & 0xff;
  finalValue[3] = (sampleRate >> 24) & 0xff;

  myFile.seek(24);
  myFile.write(finalValue, 4);

  // Check if already in little-endian order
  myFile.close();
}

void setup()
{
  // Open serial communications and wait for port to open:

  Serial.begin(9600);
  pinMode(micPin, INPUT);
  analogReadResolution(16);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!SD.begin(4))
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  if (SD.exists("rec.wav"))
  {
    Serial.println("remove exist file");
    SD.remove("rec.wav");
  }

  myFile = SD.open("rec.wav", O_WRITE | O_READ | O_CREAT);

  // if the file opened okay, write to it:
  if (myFile)
  {
    WriteHeader();
    Serial.println("Record start... 16bit sampling");
    initialTime = millis();
  }
  else
  {
    Serial.println("error opening");
  }
}

void loop()
{
  word sample;
  byte sampleWord[2];
  
  if (myFile)
  {
    if (micros() - oldMicro > 75) // sample every 75 us => 8kHz
    {
      oldMicro = micros();
      sample = analogRead(micPin); // noise reduce
      sampleWord[1] = sample & 0xff;  // lword
      sampleWord[0] = sample >> 8;    // hword
      myFile.write(sampleWord, 2);
    }

    if (millis() - initialTime > 10000) // 10sec record
    { 
      Finalize();
      
      Serial.print("Record finished... Record Time : ");
      Serial.println(millis() - initialTime);
      
      while (1)
      {
        // done
      }
    }
  }

}

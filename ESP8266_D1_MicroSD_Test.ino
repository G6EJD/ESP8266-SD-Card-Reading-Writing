/* (c) D Bird 2016
 * Micro SD Shield - Append data to file
 *
 * This example creates a file, writes data to it, then closes it, reads and dsiplays the data, then adds more data. Use it as the basis of a data logger.
 * The following data pins are used on the Wemos D1 Mini
 * D5, D6, D7, D8, 3V3 and G
 * The SD-Card Reader uses the SPI bus pins:
 * WEMOS    SD-CARD Reader
 * D5   <-> SCK/CLK
 * D6   <-> MISO
 * D7   <-> MOSI
 * D8   <-> CS
 * Vcc  <-> Vcc
 * Gnd  <-> Gnd
 * 
 * Note that the SD card library uses 8.3 format filenames and is case-insensitive so filenames such as IMAGE.JPG is the same as image.jpg
 * When you use file.write() or file.print() etc, it doesn't write to the card until you flush() or close().
 * Whenever you open a file, be sure to close it to save your data. 
 */
 /*
Commands aviable with the SD-Card filing system

SD.begin(ChipSelectPin)
SD.exists(filename) returns TRUE is found the name of the file to test for existence, which can include directories (delimited by forward-slashes, /) 
SD.open(filename,OPEN_READ|OPEN_WRITE) SD.open(filename) defaults to OPEN_READ, if OPEN_WRITE parameter used and if file does not exist, it is created
SD.mkdir(filename) where filename is the directory name to create, with sub-directories separated by forward-slashes / Returns true if directory creation succeeded
SD.rmdir(filename) Remove a directory from the SD card. The directory must be empty, returns true if it succeeds.
SD.remove(filename) Remove filename as the name of the file to remove, which can include directories (delimited by forward-slashes, / Returns true if successful

file.read() 
file.read(buf,len) Where buf is an array of characters or bytes and len the number of elements in buf 
file.write()
file.print()
file.println()

file.available() Check if there are bytes available for reading from the file. Returns the number of bytes available (int) 
file.seek(location) and returns status if a function so TRUE if location achieved, i.e. not past EOF
file.position() Get the current position within the file i.e. the location to which the next byte will be read from or written to
file.close() Close the file, and ensure that any data written to it is physically saved to the SD card
file.flush() Ensures that any bytes written to the file are physically saved to the SD card. This is done automatically when the file is closed. 
file.peek() Read a byte from the file without advancing to the next one. Successive calls to peek() will return the same value, as will the next call to read(). 
file.size() Returns the size of the file in bytes (unsigned long) 
file.isDirectory() Directories (or folders) are special kinds of files and this function reports if the current file is a directory or not. Returns true if it is
file.openNextFile() Reports the next file or folder in a directory
file.rewindDirectory() Brings you back to the first file in the directory, used in conjunction with openNextFile(). 

*/

#include <SPI.h>
#include <SD.h>

const int chipSelect = D8; // use D0 for Wemos D1 Mini
File root;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print("\r\nWaiting for SD card to initialise...");
  if (!SD.begin(chipSelect)) { // CS is D8 in this example
    Serial.println("Initialising failed!");
    return;
  }
  Serial.println("Initialisation completed");
}

void loop() {
  // SD.open(filename,OPEN_READ|OPEN_WRITE) SD.open(filename) defaults to OPEN_READ
  root = SD.open("/");
  root.rewindDirectory();
  printDirectory(root, 0); //Display the card contents
  root.close();
  Serial.println("\r\nOPEN FILE example completed");
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  //-------------------------------------------------------------------------------
  // SD.exists(filename) returns TRUE is found the name of the file to test for existence, which can include directories (delimited by forward-slashes, /) 
  Serial.println("Look for a file called 'testfile.txt', if found remove it");
  if (SD.exists("testdata.txt")) {
    Serial.println("File testfile.txt found");
    if (SD.remove("testdata.txt")) Serial.println("File successfully deleted"); else Serial.println("Error - file not deleted");
  }
  if (!SD.exists("testdata.txt")); {
    Serial.println("Following confirmation checks, 'testfile.txt' now deleted\r\n");
  }
  root = SD.open("/");
  root.rewindDirectory();
  printDirectory(root, 0); //Display the card contents
  root.close();
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  //-------------------------------------------------------------------------------
  // SD.open(filename,OPEN_READ|OPEN_WRITE) SD.open(filename) defaults to OPEN_READ
  Serial.println("Open a new file called 'testfile.txt' and write 1 to 5 to it");
  File testfile = SD.open("testdata.txt", FILE_WRITE); // FILE_WRITE opens file for writing and moves to the end of the file, returns 0 if not available
  if (testfile) {
    for (int entry = 0; entry <= 5; entry = entry + 1) {
      testfile.print(entry);
      Serial.print(entry); 
    }
  }
  Serial.println("\r\nCompleted writing data 1 to 5\r\n");
  testfile.close();
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  //-------------------------------------------------------------------------------
  Serial.println("Open a file called 'testfile.txt' and read the data from it");
  testfile = SD.open("testdata.txt", FILE_READ); // FILE_WRITE opens file for writing and moves to the end of the file
  while (testfile.available()) {
    Serial.write(testfile.read());
  }
  Serial.println("\r\nCompleted reading data from file\r\n");
  // close the file:
  testfile.close();
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  //-------------------------------------------------------------------------------
  Serial.println("Open a file called 'testfile.txt' and append 5 downto 1 to it");
  testfile = SD.open("testdata.txt", FILE_WRITE); // FILE_WRITE opens file for writing and moves to the end of the file
  for (int entry = 5; entry >= 0; entry = entry - 1) {
    testfile.print(entry);
    Serial.print(entry);
  }
  Serial.println("\r\nCompleted writing data 5 to 1\r\n");
  testfile.close();
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  //-------------------------------------------------------------------------------
  Serial.println("Open a file called 'testfile.txt' and read it");
  testfile = SD.open("testdata.txt", FILE_READ); // FILE_WRITE opens file for writing and moves to the end of the file
  while (testfile.available()) {
    Serial.write(testfile.read());
  }
  Serial.println("\r\nCompleted reading from the file\r\n");
  // close the file:
  testfile.close();
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  //012345543210
  //-------------------------------------------------------------------------------
  Serial.println("\r\nOpen a file called 'testfile.txt' and move to position 8 in the file then print the data (should be 3)");
  testfile = SD.open("testdata.txt", FILE_READ); // FILE_WRITE opens file for writing and moves to the end of the file
  Serial.print("Data at file location (8): ");
  testfile.seek(8);
  Serial.write(testfile.read());
  //-------------------------------------------------------------------------------
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  Serial.println("\r\nOpen a file called 'testfile.txt' and move to position 6 in the file then print the data (should be 5)");
  Serial.print("Data at file location (6): ");
  testfile.seek(6);
  Serial.write(testfile.read());
  //-------------------------------------------------------------------------------
  Serial.print("\r\nFile pointer is at file location: ");
  Serial.println(testfile.position());
  Serial.print("\r\nData at current file location (should be 4): ");
  Serial.println(char(testfile.peek()));
  //-------------------------------------------------------------------------------
  // close the file:
  testfile.close();
  //-------------------------------------------------------------------------------
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  //-------------------------------------------------------------------------------
  Serial.println("\r\nOpen a file called 'testfile.txt' and write some data records");
  testfile = SD.open("testdata.txt", FILE_WRITE); // FILE_WRITE opens file for writing and moves to the end of the file
  int hours = 10;
  int mins  = 00;
  String names = "Mr Another";
  testfile.print("\r\n"+String(hours));
  testfile.print(":");
  testfile.print(String(mins,2));
  testfile.println(" "+names);
  Serial.print(hours);
  Serial.print(":");
  Serial.print(mins);
  Serial.println(names);
  names = "Mr And Another";
  testfile.print(String(hours+1));
  testfile.print(":");
  testfile.print(String(mins+1));
  testfile.println(" "+names+"\r\n");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(mins,2);
  Serial.print(names);
  Serial.println("\r\nCompleted writing data records to the file\r\n");
  testfile.close();
  Serial.flush(); Serial.begin(9600); while(!Serial.available());

  // Now read file contents
  Serial.println("\r\nOpen a file called 'testfile.txt' and move to position 8 in the file then print the data (should be 4)");
  testfile = SD.open("testdata.txt", FILE_READ); // FILE_WRITE opens file for writing and moves to the end of the file
  while (testfile.available()) {
    Serial.write(testfile.read());
  }
  Serial.println("\r\nCompleted reading records from the file\r\n");
  Serial.flush(); Serial.begin(9600); while(!Serial.available());
  Serial.println("\r\nStarting again");
}

void printDirectory(File dir, int numTabs) {
int colcnt =0;
while(true) {
  File entry =  dir.openNextFile();
  if (! entry) {
    // no more files
    break;
   }
   if (numTabs > 0) {
     for (uint8_t i=0; i<=numTabs; i++) {
       Serial.print('\t');
     }
   }
   Serial.print(entry.name());
   if (entry.isDirectory()) {
     Serial.println("/");
     printDirectory(entry, numTabs+1);
   } else
   {
     // files have sizes, directories do not
     Serial.print("\t");
     Serial.println(entry.size(), DEC);
   }
   entry.close();
  }
}


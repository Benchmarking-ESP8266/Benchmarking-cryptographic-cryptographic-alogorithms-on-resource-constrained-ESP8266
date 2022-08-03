#include <Crypto.h>
#include <AES.h>
#include <string.h>

AES aes;

unsigned long startTime;
unsigned long lastYield = 0;
bool useRNG = true;

int randomBitRaw(void) {
  if (lastYield == 0 || millis() - lastYield >= 50) {
    yield();
    lastYield = millis();
  }
  uint8_t bit = analogRead(A0);
      //using A0 / TOUT
  return bit & 1;
}

byte *randomBitRaw2(){
  static byte r[192];
  for(int i=0;i<192;i++){
    r[i]=randomBitRaw();
  }
  return r;
}
void setup() {
  Serial.begin(115200);
  
}
void loop() 
{
  unsigned long start;
  unsigned long elapsed;
  byte *key;
  key=randomBitRaw2();
  Serial.print("Secret key:");
  for(int i=0;i<192;i++){
    Serial.print(key[i]);
}
  
  Serial.println();
  byte plaintext[16]={0x00,0x1A,0x8B,0x7A,0x6C,0x2E,0x01,0x24,0x4C,0x63,0x7B,0xC7,0xD4,0xE5,0xE7,0xA1};
  start = micros();// Setting Key for AES
  elapsed = micros()-start;
  Serial.print("key setup:");
  Serial.print(elapsed);
  Serial.print("us per operation, ");
  /*Serial.print((1000.0 * 1000000.0) / elapsed);
  Serial.println(" per second");*/
  Serial.println();
  Serial.print("Size of state:");
  Serial.print(sizeof(AES));
  Serial.print("\nBefore Encryption:");
  for(int i=0; i<sizeof(plaintext); i++)
  {
    Serial.print(plaintext[i],HEX);
  }

   Serial.println();
   int count;
   
   start = micros();
   byte cypher[16];
   aes.encrypt(cypher,plaintext);//cypher->output block and plaintext->input block
   Serial.print("encrypted: ");
   for (int i=0; i<sizeof(cypher); i++) 
   {
    Serial.print(cypher[i], HEX);
    Serial.print(" "); //separator
   }
   Serial.println();
   elapsed=micros()-start;
   Serial.print(elapsed/1000.0);
   Serial.println("us per operatin");
   
   
   start = micros();
   //aes.set_key(key,16);
   byte decryptedtext[16];
   aes.decrypt(decryptedtext,cypher);
   Serial.print("After Decryption:");
   for(int i=0; i<sizeof(decryptedtext); i++)
   {
    Serial.print(decryptedtext[i],HEX);
   }
   Serial.println();
   elapsed=micros()-start;
   Serial.print(elapsed/1000.0);
   Serial.println("us per operatin");
   Serial.print((sizeof(cypher)));
   Serial.println(" bytes per second");
   Serial.println();
   
   delay(5000);
}

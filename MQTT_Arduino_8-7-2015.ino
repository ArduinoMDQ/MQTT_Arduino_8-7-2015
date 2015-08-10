#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 1, 1 };
byte ip[]     = { 192, 168, 1, 2 };


// Set what PINs our Led's are connected to
int redPin = 5;                
int greenPin = 6;
int bluePin = 13;

byte on[2] = "1";
byte off[2] = "0";

EthernetClient ethClient;

PubSubClient client(server, 1883, callback, ethClient);

// handles messages that are returned from the broker on our subscribed channel
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("New message from broker on topic:");
  Serial.println(topic);
  
  Serial.print("Payload:");
  Serial.write(payload, length);
  
  
  // This will blink our green LED
  blink(greenPin);
  
  // Check and see if our payload matches our simple trigger test
  if ((length == 1) & (memcmp(payload, on, 1) == 0) )
  {
    Relay(bluePin,1);
  }
  if ((length == 1) & (memcmp(payload, off, 1) == 0) )
  {
  Relay(bluePin,0);
  }

/*
  byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer
  memcpy(p,payload,length);
  client.publish("outTopic", p, length);
  // Free the memory
  free(p);
*/
}


void setup()
{
  
  
  // Open serial communications
  Serial.begin(9600);
  
  // Setup our Leds
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Ethernet.begin(mac, ip);
   
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());
 
   if (client.connect("arduinoClient")) {
      // Good, we connected turn on the red led
    digitalWrite(redPin, HIGH);
   // Publish a message to the status topic
    client.publish("control","-MSG: Juan El Arduino esta online");
    
    // Listen for messages on the control topic
    client.subscribe("control");
  }
}

void loop()
{
  client.loop();
}


// Anything with flashing lights.
void blink(int targetLed) 
{
 static boolean led = HIGH;
 static int count = 0;
 
 Serial.print("Starting to blink...");
 
 while (count < 6) 
 {
   digitalWrite(targetLed, led);
   
   count++;
   
   if (led == HIGH) 
   {
     led = LOW;
   } 
   else 
   {
     led = HIGH;
   }
   
   
   delay(50);
 }
 
 count = 0;
 
 Serial.print("done.");
}



// Anything with flashing lights.
void Relay(int pin, int estado) 
{
 
 
 switch(estado){
   case 0: digitalWrite(pin, LOW);
   client.publish("led","-MSG:Juan me apagaste el LED !!! ");
   break;
   case 1: digitalWrite(pin, HIGH);
    client.publish("led","-MSG:Juan prendiste la lampara de  ALADINO !!! ");
 
 break;
 default:
  client.publish("led","Error de Comando");
 
 break;
 }
 
   
 Serial.print("done.");
}


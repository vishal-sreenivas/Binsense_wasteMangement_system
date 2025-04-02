
#define BLYNK_TEMPLATE_ID "TMPL3xu8QqMZM"
#define BLYNK_TEMPLATE_NAME "BinSense"
#define BLYNK_AUTH_TOKEN "bn2hTGw99Fch6TpZ-DdA6HfDsRG1Y9w2"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "R's vivo";
char pass[] = "janvi@2003";

BlynkTimer timer;

#include <Servo.h>
#define echoPin D5
#define trigPin D6
#define irPin D7

Servo servo;
long duration;
int distance;
int binLevel = 0;

void detectAndMeasure()
{
  int irState = digitalRead(irPin); // Read IR sensor state

  if (irState == HIGH) // If an object is detected
  {
    Serial.println("No object detected by IR sensor.");
  }
  else
  {
    
    Serial.println("Object detected by IR sensor.");
    servo.write(180); // Rotate the servo motor 90 degrees
    delay(1000);     // Wait for the servo to complete rotation
    servo.write(0);  // Reset servo to 0 degrees
    delay(500);

    // Measure distance of the object
    ultrasonic();

    Serial.print("Distance to object: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Display distance in Blynk app
    Blynk.virtualWrite(V0, distance);
  }

  // Measure and display bin level percentage
  ultrasonic();
  Serial.print("Bin Level: ");
  Serial.print(binLevel);
  Serial.println(" %");

  // Send bin level to Blynk
  Blynk.virtualWrite(V1, binLevel);
}

void ultrasonic()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance
  distance = duration * 0.034 / 2;

  // Map distance to percentage
  binLevel = map(distance, 21, 0, 0, 100); // Adjust bin height here
}

void setup()
{
  Serial.begin(9600);
  servo.attach(D2);

  pinMode(irPin, INPUT);    // Set IR sensor pin as input
  pinMode(trigPin, OUTPUT); // Set ultrasonic sensor trigger pin as output
  pinMode(echoPin, INPUT);  // Set ultrasonic sensor echo pin as input

  Blynk.begin(auth, ssid, pass);
  delay(2000);

  // Set a timer to check the IR sensor and ultrasonic sensor periodically
  timer.setInterval(1000L, detectAndMeasure);

  Serial.println("System initialized.");
}

void loop()
{
  Blynk.run();
  timer.run();
}

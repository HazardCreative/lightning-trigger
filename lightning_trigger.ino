// Lightning trigger v3
// 2011-09-09
// Rewrite to remove need for serial output; 
// improves response time

// Based on schematic by Maurice Ribble
// http://www.glacialwanderer.com/hobbyrobotics

// This code waits for a sudden increase in light intensity
// and then triggers the camera.

// v1 - Initial build
// v2 - Various improvements
// v3 - Improved threshold monitoring
// v4 - Holds camera focus on at all times to avoid standby
//    - Improves trigger sensitivity
//    - Speeds polling operations

#define FOCUS_PIN 2
#define SHUTTER_PIN 3
#define LED_PIN 13
#define LIGHTNING_TRIGGER_ANALOG_PIN 0
#define TRIGGER_THRESHHOLD 5
#define VAL_SIZE 100

int lightningVal;
int val_index = 0;
int lightvals[VAL_SIZE];
long val_total;

void activate_shutter() {
//  digitalWrite(FOCUS_PIN, HIGH);
  digitalWrite(SHUTTER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("trip-------");
  delay(4); // 1/250th
//  digitalWrite(FOCUS_PIN, LOW);
  digitalWrite(SHUTTER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  pinMode(FOCUS_PIN, OUTPUT);
  pinMode(SHUTTER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(FOCUS_PIN, HIGH);
  digitalWrite(SHUTTER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

//  Serial.begin(115200); // open serial

  int i;
  for (i = 0; i < VAL_SIZE; i++) {
    val_total += lightvals[i] = analogRead(LIGHTNING_TRIGGER_ANALOG_PIN);
  }
}

void loop() {
  val_total -= lightvals[val_index];
  val_total += lightvals[val_index] = analogRead(LIGHTNING_TRIGGER_ANALOG_PIN);
//  Serial.println(val_total, DEC);
//  Serial.println(lightvals[val_index], DEC);
  
  if (val_total / VAL_SIZE + TRIGGER_THRESHHOLD < lightvals[val_index]) {
    activate_shutter();

    // completely dump and refill array
    int i;
    val_total = 0;
    val_index = 0;
    for (i = 0; i < VAL_SIZE; i++) {
      val_total += lightvals[i] = analogRead(LIGHTNING_TRIGGER_ANALOG_PIN);
    }
  }

  val_index++;
  val_index %= VAL_SIZE;
}

int r,g,b = 0;

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(115200);
  
  for(int i=6; i<54; i++)
    pinMode(i, INPUT);
    
  for(int i=2; i<6; i++)
    pinMode(i, OUTPUT);
    
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  
  for(int i=0; i<10; i++){
    digitalWrite(5, LOW);
    delay(100);
    digitalWrite(5, HIGH);
    delay(100);
  }
  
}

int data[] = {0,0,0,0};

void loop() {
  if(Serial.available() == 4){
    data[0] = Serial.read();
    data[1] = Serial.read();
    data[2] = Serial.read();
    data[3] = Serial.read();
    
    if(data[0] == 0xff){
      r = data[1];
      g = data[2];
      b = data[3];
    }
    else if(data[1] == 0xff){
      r = data[2];
      g = data[3];
      b = data[0];
    }
    else if(data[2] == 0xff){
      r = data[3];
      g = data[0];
      b = data[1];
    }
    else if(data[3] == 0xff){
      r = data[0];
      g = data[1];
      b = data[2];
    }
    
    if(r == 0xfe)
      r = 0xff;
    if(g == 0xfe)
      g = 0xff;
    if(b == 0xfe)
      b = 0xff;
    
    analogWrite(2, 255 - r);
    analogWrite(4, 255 - g);
    analogWrite(5, 255 - b);
    
  }
  
  delay(10);
  
}

void checkSonic() {
  for (int i = 0; i<9; i++)//start of for loop
  {
        //***For the Upper sensor
        //Digital write to trigPin LOW, as we just want to have it start off in a low state
        digitalWrite(trigPinU, LOW);
        delayMicroseconds(2);
        //Digital write to trigPin HIGH, 
        digitalWrite(trigPinU, HIGH);
        delayMicroseconds(10);
        //set trigPin back to Low, to end the pulse until the next loop
        digitalWrite(trigPinU, LOW);
        //Measuring HIGH pulse from HC SR04
        durationU = pulseIn(echoPinU, HIGH);
        //***End of Upper Sensor Loop
      
      
      
        //***For the Lower Sensor
        //Digital write to trigPin LOW, as we just want to have it start off in a low state
        digitalWrite(trigPinL, LOW);
        delayMicroseconds(2);
        //Digital write to trigPin HIGH, 
        digitalWrite(trigPinL, HIGH);
        delayMicroseconds(10);
        //set trigPin back to Low, to end the pulse until the next loop
        digitalWrite(trigPinL, LOW);
        //Measuring HIGH pulse from HC SR04
        durationL = pulseIn(echoPinL, HIGH);
        //***End of Lower Sensor Loop
        
        //***Calculations***
        //using 343 m/s as speed of sound
        //duration = 2* the distance, as it takes the time to hit the wall + time to bounce back
        //distance = (duration/2)*speed of sound (km/s)/1000m per km
        distanceU = (durationU/2)*343/10000;
        distanceL = (durationL/2)*343/10000;
        predur[i] = distanceU; // Setting the array 
        
  }//end of for loop
  float sum = 0;
  for( int i = 0; i < 9; i++ )
  {
    sum += (predur[i]);
  }
  
  float avg = sum/10;
  
  if ( distanceL < 40 || avg > (predur[1]+10))
  {
     int i = 0;
        while ( ((distanceU <= (predur[1] + 10 )) && (distanceL <=30))|| (i<10))
        {
          digitalWrite(trigPinU, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPinU, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPinU, LOW);
          durationU = pulseIn(echoPinU, HIGH);
          distanceU = (durationU/2)*343/10000;
          digitalWrite(trigPinL, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPinL, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPinL, LOW);
          durationL = pulseIn(echoPinL, HIGH);
          distanceL = (durationL/2)*343/10000;

          
          digitalWrite(vibPin, HIGH);            
          i++;
        }
       
        
    
  }
  digitalWrite(vibPin, LOW);
}


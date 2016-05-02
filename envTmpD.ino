void envTmpD(){

  if(myserial.available() > 0){                                           //if we see that the ENV-TMP-D has sent a character.
     received_from_sensor=myserial.readBytesUntil(13,tmp_data,20); //we read the data sent from ENV-TMP-D untill we see a <CR>. We also count how many character have been recived.  
     tmp_data[received_from_sensor]=0; //we add a 0 to the spot in the array just after the last character we recived. This will stop us from transmiting incorrect data that may have been left in the buffer. 
     string_received=1;                //a flag used when the arduino is controlling the ENV-TMP-D to let us know that a complete string has been received.
//     Serial.println(tmp_data);         //lets transmit that data received to the serial monitor.
     }    
  
  if(arduino_only==1){               //if you set arduino_only to = 1.  
     if (startup==0){                //if the arduino just booted up, we need to set some things up first.  
         pc_debug=0;                 //make sure pc_debug is set to 0. You will no longer be able to write commands to the ENV-TMP-D   in the serial monitor.  
          myserial.print("e\r");     //take the ENV-TMP-D out of continues mode. 
          delay(50);                 //on start up sometimes the first command is missed. 
          myserial.print("e\r");     //so, let’s send it twice.
          delay(50);                 //a short delay after the ENV-TMP-D was taken out of continues mode is used to make sure we don’t over load it with commands.
          startup=1;                 //startup is completed, let's not do this again during normal operation. 
      }
    
  delay(800);                         //we will take a reading ever 800ms. You can make this longer or shorter if you like.
   myserial.print("R\r");             //send it the command to take a single reading.
   if(string_received==1){            //did we get data back from the ENV-TMP-D?
     float_tmp=atof(tmp_data);        //many people ask us "how do I convert a sting into a float?" this is how...(pretty tough) 
//     if(float_tmp>=25){Serial.println("high\r");} //This is the proof that it has been converted into a string.
//     if(float_tmp<25){Serial.println("low\r");}  //This is the proof that it has been converted into a string.


  sendPayload(deviceName,"TMP-D","Temperature","F",readTime,String(float_tmp),"M");



     string_received=0;}              //reset the string received flag.
  }   
  
}

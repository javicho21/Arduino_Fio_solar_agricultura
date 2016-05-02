 void serialEvent(){           //this interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received.    
        if(pc_debug==1){       //if pc debug is set to 0 this function will be bypassed.  
           received_from_computer=Serial.readBytesUntil(13,computerdata,20); //we read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received.      
           computerdata[received_from_computer]=0; //we add a 0 to the spot in the array just after the last character we recived. This will stop us from transmiting incorrect data that may have been left in the buffer. 
           myserial.print(computerdata);          //we transmit the data received from the serial monitor(pc/mac/other) through the soft serial port to the ENV-TMP-D  . 
           myserial.print('\r');                  //all data sent to the ENV-TMP-D must end with a <CR>.  
          }    
 }


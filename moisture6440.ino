void moisture6440(){

  moisture6440_level = map(analogRead(A0),0,1023,0,200);  //ajustar valores conforme especificacoes de resistencia do sensor

  sendPayload(deviceName,"Davis_6440_moisture","humidity","cb",readTime,String(moisture6440_level),"M");
  
}

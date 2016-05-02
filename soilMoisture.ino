void soilMoisture(){

  soilMoisture_level_1 = map(analogRead(A1),0,1023,0,200);  //ajustar valores conforme especificacoes de resistencia do sensor

  sendPayload(deviceName,"soil_moisture_1","humidity","cb",readTime,String(soilMoisture_level_1),"M");



  soilMoisture_level_2 = map(analogRead(A2),0,1023,0,200);  //ajustar valores conforme especificacoes de resistencia do sensor

  sendPayload(deviceName,"soil_moisture_2","humidity","cb",readTime,String(soilMoisture_level_2),"M");

  

  
  
}

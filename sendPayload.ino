void sendPayload(String deviceName, String sensorName, String sensorType, String sensorUnit, String readTime, String reading,String messageType){

  stringPayload = deviceName;
  stringPayload += ",";

  stringPayload += sensorName;
  stringPayload += ",";

  stringPayload += sensorType;
  stringPayload += ",";

  stringPayload += sensorUnit;
  stringPayload += ",";

  stringPayload += readTime;
  stringPayload += ",";

  stringPayload += reading;
  stringPayload += ",";

  stringPayload += messageType;
  stringPayload += ",";

  for (int i=0 ; i < stringPayload.length() ; i++){
    payload[i]= stringPayload.charAt(i);
  }

  xbee.send(zbTx);

}

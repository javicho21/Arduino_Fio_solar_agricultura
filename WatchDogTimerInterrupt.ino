// Define watchdog timer interrupt.
ISR(WDT_vect){
  // Set the watchdog activated flag.
  watchdogActivated = true;
}

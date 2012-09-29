/**
  \page heartbeat Heartbeat

  The logging functions can relay the log category to a further function. By
  default an function is called which checks for an instance of CHeartbeat
  and feeds it. CHeartbeat controlls the blinking of an LED.

  If you want a different behaviour you have to overload
  ```logCode(ECode code);.```

  \image html heartbeat.png
*/

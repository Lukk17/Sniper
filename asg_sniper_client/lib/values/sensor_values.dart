class SensorValues {
  static const String SENSOR_VALUE = "Sensor value";
  static const String SERVER_INFO = "Welcome: ";
  static const String PLACE_HOLDER = "Start !";

  static Map<String, String> pinMapping() {
    Map<String, String> pins = new Map();
    pins.putIfAbsent("D1 - pin 5", () => "5");
    pins.putIfAbsent("D2 - pin 4", () => "4");
    pins.putIfAbsent("D5 - pin 14", () => "14");
    pins.putIfAbsent("D6 - pin 12", () => "12");
    pins.putIfAbsent("D7 - pin 13", () => "13");
    return pins;
  }

  static Map<String, String> reversePinMapping() {
    Map<String, String> pins = new Map();
    pins.putIfAbsent("5", () => "D1 - pin 5");
    pins.putIfAbsent("4", () => "D2 - pin 4");
    pins.putIfAbsent("14", () => "D5 - pin 14");
    pins.putIfAbsent("12", () => "D6 - pin 12");
    pins.putIfAbsent("13", () => "D7 - pin 13");
    return pins;
  }
}

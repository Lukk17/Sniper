class SensorValues {
  static final String SENSOR_VALUE = "Sensor value: ";
  static final String SERVER_INFO = "Welcome: ";
  static final String PLACE_HOLDER = "Start !";

  static Map<String, String> pinMapping() {
    Map<String, String> pins = new Map();
    pins.putIfAbsent("D1 - pin 5", () => "5");
    pins.putIfAbsent("D2 - pin 4", () => "4");
    pins.putIfAbsent("D5 - pin 14", () => "14");
    pins.putIfAbsent("D6 - pin 12", () => "12");
    pins.putIfAbsent("D7 - pin 13", () => "13");
    return pins;
  }
}

class ServerInfo {
  String _SSID;
  String _wifiPass;
  String _apIpAddress;
  String _webServerPort;
  String _webSocketInterval;
  String _lastNotificationTime;
  String _ledPin;
  String _sensorPin;

  ServerInfo(
      this._SSID,
      this._wifiPass,
      this._apIpAddress,
      this._webServerPort,
      this._webSocketInterval,
      this._lastNotificationTime,
      this._ledPin,
      this._sensorPin);

  ServerInfo.clean()
      : this._SSID = "",
        this._wifiPass = "",
        this._apIpAddress = "",
        this._webServerPort = "",
        this._webSocketInterval = "",
        this._lastNotificationTime = "",
        this._ledPin = "",
        this._sensorPin = "";

  String get SSID => _SSID;

  String get sensorPin => _sensorPin;

  String get ledPin => _ledPin;

  String get lastNotificationTime => _lastNotificationTime;

  String get webSocketInterval => _webSocketInterval;

  String get webServerPort => _webServerPort;

  String get apIpAddress => _apIpAddress;

  String get wifiPass => _wifiPass;

  ServerInfo.fromJson(Map<String, dynamic> json)
      : _SSID = json["SSID"]!.toString(),
        _wifiPass = json["WiFi pass"]!.toString(),
        _apIpAddress = json["AP IP address"]!.toString(),
        _webServerPort = json["WebServer port"]!.toString(),
        _webSocketInterval = json["Websocket interval"]!.toString(),
        _lastNotificationTime = json["Last notification time[s]"]!.toString(),
        _ledPin = json["LED pin"]!.toString(),
        _sensorPin = json["Sensor pin"]!.toString();

  @override
  String toString() {
    return 'ServerInfo{_SSID: $_SSID, _wifiPass: $_wifiPass, '
        '_apIpAddress: $_apIpAddress, _webServerPort: $_webServerPort, '
        '_webSocketInterval: $_webSocketInterval, '
        '_lastNotificationTime: $_lastNotificationTime, '
        '_ledPin: $_ledPin, _sensorPin: $_sensorPin}';
  }
}

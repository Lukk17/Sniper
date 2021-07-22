class ServerInfo {
  String _SSID;
  String _wifiPass;
  String _apIpAddress;
  String _webServerPort;
  String _webSocketInterval;
  String _lastNotificationTime;
  String _ledPin;
  String _sensorPin;

  ServerInfo(this._SSID,
      this._wifiPass,
      this._apIpAddress,
      this._webServerPort,
      this._webSocketInterval,
      this._lastNotificationTime,
      this._ledPin,
      this._sensorPin);

  ServerInfo.clean()
      :
        this._SSID = "",
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
}

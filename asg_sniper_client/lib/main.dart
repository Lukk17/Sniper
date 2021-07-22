import 'dart:convert';

import 'package:asg_sniper/dto/serverInfo.dart';
import 'package:asg_sniper/validators/numeric_validator.dart';
import 'package:asg_sniper/values/sensor_values.dart';
import 'package:flutter/material.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

import 'values/web_socket_values.dart';

void main() {
  runApp(AsgSniper());
}

class AsgSniper extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'ASG Sniper',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: HomePage(title: 'ASG Sniper'),
    );
  }
}

class HomePage extends StatefulWidget {
  HomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final _channel = WebSocketChannel.connect(Uri.parse(WebSocketValues.URI));
  Stream _sensorStream = new Stream.empty();
  ServerInfo _serverInfo = new ServerInfo.clean();
  String _sensorValues = SensorValues.PLACE_HOLDER;
  Map<String, String> _pinMapping = SensorValues.pinMapping();
  String? _selectedLedPin;
  final GlobalKey<FormState> _intervalFormKey = GlobalKey();

  @override
  void initState() {
    // ned to be initialized here not in widget build method,
    // because stream can be accessed only once.
    _sensorStream = _channel.stream.asBroadcastStream();
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return DefaultTabController(
      length: 3,
      initialIndex: 1,
      child: Scaffold(
        appBar: AppBar(
          bottom: TabBar(
            tabs: [
              Tab(icon: Icon(Icons.whatshot)),
              Tab(icon: Icon(Icons.info_outline)),
              Tab(icon: Icon(Icons.settings)),
            ],
          ),
        ),
        body: TabBarView(children: [
          _shootTableView(),
          _serverInfoView(),
          _settingsView(),
        ]),
      ),
    );
  }

  Widget _shootTableView() {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          Text(
            'Shoot',
          ),
          StreamBuilder(
            stream: _sensorStream,
            builder: (context, snapshot) {
              if (snapshot.hasData) {
                _handleMessage(snapshot);
              }
              return Text(
                '$_sensorValues',
                style: Theme.of(context).textTheme.headline4,
              );
            },
          ),
          MaterialButton(
            onPressed: clearShootTable,
            color: Colors.blue,
            child: Text("Clear"),
          )
        ],
      ),
    );
  }

  Widget _serverInfoView() {
    return Center(
      child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              'Info\n'
              'SSID: ${_serverInfo.SSID}\n'
              'WiFi pass: ${_serverInfo.wifiPass}\n'
              'AP IP address: ${_serverInfo.apIpAddress}\n'
              'WebServer port: ${_serverInfo.webServerPort}\n'
              'WebSocket interval: ${_serverInfo.webSocketInterval}\n'
              'Last notification time:  ${_serverInfo.lastNotificationTime} sec\n'
              'LED pin: ${_serverInfo.ledPin}\n'
              'Sensor pin: ${_serverInfo.sensorPin}\n',
            ),
            MaterialButton(
              onPressed: _updateServerInfo,
              color: Colors.lightGreen,
              child: Text("Update"),
            )
          ]),
    );
  }

  Widget _settingsView() {
    return Center(
      child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text("Change WebSocket interval"),
            SizedBox(
              width: 100,
              child: Form(
                key: this._intervalFormKey,
                child: Column(children: <Widget>[
                  TextFormField(
                    onSaved: (value) => _updateWebSocketInterval(value),
                    validator: (value) => NumericValidator.validate(value,
                        max: 50, integer: true),
                  ),
                ]),
              ),
            ),
            MaterialButton(
              color: Colors.lightGreen,
              child: Text("Submit"),
              onPressed: () => {
                if (this._intervalFormKey.currentState!.validate())
                  {
                    setState(() {
                      this._intervalFormKey.currentState!.save();
                    })
                  }
              },
            ),
            SizedBox(height: 50),
            Text("Change LED pin"),
            SizedBox(
              width: 100,
              child: DropdownButton<String>(
                // hint: Text('Please choose a LED pin'),
                value: _selectedLedPin,
                items: _pinMapping.keys.toList().map((String value) {
                  return DropdownMenuItem<String>(
                    value: value,
                    child: new Text(value),
                  );
                }).toList(),
                onChanged: (newValue) {
                  setState(() {
                    _selectedLedPin = newValue;
                  });
                },
              ),
            ),
            MaterialButton(
              color: Colors.lightGreen,
              child: Text("Submit"),
              onPressed: () => _changeLedPin(_selectedLedPin!),
            ),
          ]),
    );
  }

  void _handleMessage(AsyncSnapshot<Object?> snapshot) {
    String event = snapshot.data.toString();
    String message = "";

    if (event.contains(SensorValues.SERVER_INFO)) {
      message = event.toString().split(SensorValues.SERVER_INFO)[1];
      ServerInfo serverInfo = jsonDecode(message);
      print(serverInfo.toString());
    }

    if (event.toString().contains(SensorValues.SENSOR_VALUE)) {
      message = event
          .toString()
          .replaceAll("\"", "")
          .split(SensorValues.SENSOR_VALUE)[1];

      if (_sensorValues == SensorValues.PLACE_HOLDER) {
        _sensorValues = message;
      } else {
        _sensorValues += "$message\n";
      }
    }
    print(message);
  }

  void _updateServerInfo() {
    _channel.sink.add(WebSocketValues.UPDATE_STATS);
  }

  void _changeLedPin(String value) {
    String pinValue = _pinMapping[value]!;
    print("updateLedPin $pinValue");
    _channel.sink.add("${WebSocketValues.CHANGE_LED_PIN}$pinValue");
  }

  void _updateWebSocketInterval(value) {
    int intervalValue = int.parse(value);
    print("updateWebSocketInterval $intervalValue");
    _channel.sink.add("${WebSocketValues.CHANGE_INTERVAL}$intervalValue");
  }

  @override
  void dispose() {
    _channel.sink.close();
    super.dispose();
  }

  void clearShootTable() {
    _sensorValues = SensorValues.PLACE_HOLDER;
  }
}

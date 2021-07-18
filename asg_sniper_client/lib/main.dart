import 'package:flutter/material.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'ASG Sniper',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  String _sensorValues = "Start !";

  final _channel = WebSocketChannel.connect(
    Uri.parse('ws://192.168.4.1/ws'),
  );

  void handleMessage(AsyncSnapshot<Object?> snapshot) {

      String event = snapshot.data.toString();
      String message = "";

      if (event.contains("Welcome")) {
        message = event.toString().split("Welcome: ")[1];
      }

      if (event.toString().contains("Sensor value")) {
        message = event.toString().replaceAll("\"", "").split("Sensor value: ")[1];

        if (_sensorValues == "Start !") {
          _sensorValues = message;
        } else {
          _sensorValues += "$message\n";
        }
      }
      print(message);

  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              'Shoot',
            ),
            StreamBuilder(
              stream: _channel.stream,
              builder: (context, snapshot) {
                if(snapshot.hasData) {
                  handleMessage(snapshot);
                }
                return Text(
                  '$_sensorValues',
                  style: Theme.of(context).textTheme.headline4,
                );
              },
            ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    print("aa");
    _channel.sink.close();
    super.dispose();
  }
}

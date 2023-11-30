// home.dart

import 'package:flutter/material.dart';
import 'dart:convert';
import 'package:http/http.dart' as http;

class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  final String _channelId = '2331704';
  final String _apiKey = '023G7AEMDBPTODGE';
  String _latestLatitude = '';
  String _latestLongitude = '';
  String _latestVelocity = '';

  @override
  void initState() {
    super.initState();
    _fetchData();
  }

  Future<void> _fetchData() async {
    final response = await http.get(
      Uri.parse(
          'https://api.thingspeak.com/channels/$_channelId/feeds.json?api_key=$_apiKey'),
    );

    if (response.statusCode == 200) {
      final data = json.decode(response.body);
      if (data['feeds'].isNotEmpty) {
        final latestFeed = data['feeds'].last;
        print('Latest Feed: $latestFeed');
        setState(() {
          _latestLatitude =
              latestFeed['field1'] ?? 'N/A'; // Provide a default value if null
          _latestLongitude = latestFeed['field2'] ?? 'N/A';
          _latestVelocity = latestFeed['field3'] ?? 'N/A';
        });
      } else {
        print('No data available in feeds.');
      }
    } else {
      print('Failed to load data. Status Code: ${response.statusCode}');
      throw Exception('Failed to load data');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Odometry Data'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Card(
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(12.0), // Rounded corners
              ),
              elevation: 4, // Add some elevation for a shadow effect
              margin: EdgeInsets.all(16.0), // Add margin to the card
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  children: [
                    Text('Latest Latitude', style: TextStyle(fontSize: 18)),
                    Text(_latestLatitude, style: TextStyle(fontSize: 24)),
                  ],
                ),
              ),
            ),
            Card(
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(12.0),
              ),
              elevation: 4,
              margin: EdgeInsets.all(16.0),
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  children: [
                    Text('Latest Longitude', style: TextStyle(fontSize: 18)),
                    Text(_latestLongitude, style: TextStyle(fontSize: 24)),
                  ],
                ),
              ),
            ),
            Card(
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(12.0),
              ),
              elevation: 4,
              margin: EdgeInsets.all(16.0),
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  children: [
                    Text('Latest Velocity', style: TextStyle(fontSize: 18)),
                    Text(_latestVelocity, style: TextStyle(fontSize: 24)),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

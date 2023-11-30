import 'package:flutter/material.dart';
import 'home.dart'; // Import the HomeScreen from home.dart

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Odometry App',
      theme: ThemeData.dark(), // Switch to dark mode
      home: HomeScreen(), // Set HomeScreen as the main screen
    );
  }
}

import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:intl/intl.dart';
import 'package:location/location.dart';
import '/widgets/new_entry_body.dart';


class AddEntry extends StatefulWidget {
  final String url;
  const AddEntry({Key? key, required this.url}) : super(key: key);

  @override
  _AddEntryState createState() => _AddEntryState();
}

class _AddEntryState extends State<AddEntry> {
  TextEditingController numItems = TextEditingController();
  LocationData? locationData;
  var locationService = Location();
  final formKey = GlobalKey<FormState>();

  @override
  void initState() {
    super.initState();
    getLocation();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('New Post'),
        centerTitle: true,
      ),
      body: NewEntryBody(formKey: formKey, url: widget.url, numItems: numItems),
      //https://stackoverflow.com/questions/54395233/widget-not-going-to-bottom-of-screen-in-flutter
      bottomNavigationBar: BottomAppBar(
        child: ElevatedButton(
          //https://stackoverflow.com/questions/50014342/button-width-match-parent
          style: ElevatedButton.styleFrom(
            minimumSize: Size(double.infinity, 50)
          ),
          onPressed: () {
            if (formKey.currentState!.validate()) {
              uploadData(numItems.text);
              Navigator.pushNamed(context, '/').then(onGoBack);
            }
          },
          child: const Icon(Icons.cloud_upload_outlined),
        ),
      ),
    );
  }

  void uploadData(String numItems) {
    var formatDate = DateFormat('EEEE, MMMM d, y').format(DateTime.now());
    FirebaseFirestore.instance.collection('posts').add({
      'date': formatDate, 
      'numItems': int.parse(numItems), 
      'url': widget.url,
      'latitude': locationData!.latitude,
      'longitude': locationData!.longitude
    });
  }

  void getLocation() async {
    try {
      var _serviceEnabled = await locationService.serviceEnabled();
      if (!_serviceEnabled) {
        _serviceEnabled = await locationService.requestService();
        if (!_serviceEnabled) {
          print('Failed to enable service. Returning.');
          return;
        }
      }

      var _permissionGranted = await locationService.hasPermission();
      if (_permissionGranted == PermissionStatus.denied) {
        _permissionGranted = await locationService.requestPermission();
        if (_permissionGranted != PermissionStatus.granted) {
          print('Location service permission not granted. Returning.');
        }
      }

      locationData = await locationService.getLocation();
    } on PlatformException catch (e) {
      print('Error: ${e.toString()}, code: ${e.code}');
      locationData = null;
    }
    locationData = await locationService.getLocation();
  }

  //https://codepen.io/nitishk72/pen/YzwGELX
  onGoBack(dynamic value) {
    setState(() {
    });
  }
}
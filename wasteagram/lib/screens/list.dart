import 'dart:io';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/cupertino.dart';
import 'package:firebase_storage/firebase_storage.dart';
import 'package:flutter/material.dart';
import 'package:image_picker/image_picker.dart';
import 'add_entry.dart';
import '/widgets/build_list_body.dart';

class ListScreen extends StatefulWidget {
  const ListScreen({Key? key}) : super(key: key);

  @override
  _ListScreenState createState() => _ListScreenState();
}

class _ListScreenState extends State<ListScreen> {
  final scaffoldKey = GlobalKey<ScaffoldState>();
  var total;
  bool _uploadController = false;

  @override
  void initState() {
    super.initState();
    getItemTotal();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      key: scaffoldKey,
      appBar: AppBar(
        title: Text('Wasteagram - ' + total.toString()),
        centerTitle: true,
      ),
      body: _uploadController ? Center(child: CircularProgressIndicator()) : BuildListBody(),
      floatingActionButton: Semantics(
        button: true,
        onTapHint: 'Select or take an image',
        label: 'Button that opens a popup to ask if you\'d like to take an image or select an image for a new food waste entry',
        child: FloatingActionButton(
          onPressed: () {
            showAlert(context);
          },
          child: const Icon(Icons.camera_alt_outlined)
        )
      ),
      floatingActionButtonLocation: FloatingActionButtonLocation.centerFloat,
    );
  }

  Future<dynamic> showAlert(BuildContext context) {
    return showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text('Would you like to take a picture or select a picture from your gallery?'),
          content: Text('Select an option:'),
          actions: <Widget>[
            CupertinoDialogAction(
              child: Text('Take photo'),
              onPressed: () {
                getImage(context, ImageSource.camera);
                Navigator.of(context, rootNavigator: true).pop('dialog');
              }
            ),
            CupertinoDialogAction(
              child: Text('Select photo'),
              onPressed: () {
                getImage(context, ImageSource.gallery);
                Navigator.of(context, rootNavigator: true).pop('dialog');
              }
            )
          ]
        );
      }
    );
  }

  getImage(BuildContext context, ImageSource source) async {
    File? image;
    final picker = ImagePicker();

    final pickedFile = await picker.pickImage(source: source);
    image = File(pickedFile!.path);

    var fileName = DateTime.now().toString() + '.jpg';
    Reference storageReference = FirebaseStorage.instance.ref().child(fileName);
    UploadTask uploadTask = storageReference.putFile(image);
    setState(() {
      _uploadController = true;
    });
    await uploadTask;
    final url = await storageReference.getDownloadURL();
    setState(() {
      _uploadController = false;
    });
    Navigator.push(scaffoldKey.currentContext!, MaterialPageRoute(builder: (context) => AddEntry(url: url)));
  }

  //https://stackoverflow.com/questions/58670907/how-to-combine-the-sum-of-identical-keyvalue-pairs-in-firestore-query
  getItemTotal() async {
    int temp = 0;
    QuerySnapshot snapshot = await FirebaseFirestore.instance.collection("posts").get();

    snapshot.docs.forEach((doc) {
      temp += doc['numItems'] as int;
    });

    setState(() {
      total = temp;
    });
  }
}
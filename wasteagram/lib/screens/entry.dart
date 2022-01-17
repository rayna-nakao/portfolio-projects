import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import '/models/waste_entry.dart';

class Entry extends StatelessWidget {
  final WasteEntry entry;
  const Entry({Key? key, required this.entry}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Wasteagram'),
        centerTitle: true,
      ),
      body: SingleChildScrollView(
        child: Center(
          child: Column(
            children: [
              Padding(
                padding: EdgeInsets.all(5),
                child: Text('${entry.date}', style: TextStyle(fontSize: 28))
              ),
              Padding(
                padding: EdgeInsets.all(5),
                child: Image.network('${entry.url}')
              ),
              Padding(
                padding: EdgeInsets.all(5),
                child: Text('${entry.numItems}' + ' Items', style: TextStyle(fontSize: 25))
              ),
              Padding(
                padding: EdgeInsets.all(5),
                child: Text('(' + '${entry.latitude}' + ', ' + '${entry.longitude}' + ')', 
                  style: TextStyle(fontSize: 20)
                )
              )
            ],
          )
        )
      )
    );
  }
}
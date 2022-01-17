import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import '/models/waste_entry.dart';
import '/screens/entry.dart';

class BuildListBody extends StatelessWidget {
  const BuildListBody({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return StreamBuilder(
      stream: FirebaseFirestore.instance.collection('posts').snapshots(),
      builder: (BuildContext context, AsyncSnapshot<QuerySnapshot> snapshot) {
        if (snapshot.hasData && snapshot.data!.docs != null && snapshot.data!.docs.length > 0) {
          return ListView.builder(
            itemCount: snapshot.data!.docs.length,
            itemBuilder: (context, index) {
              var post = snapshot.data!.docs[index];
              return Semantics(
                button: true,
                onTapHint: 'Opens a food waste entry',
                label: 'List of food waste entries, each can be tapped to view entry details',
                child: GestureDetector(
                  child: Container(
                    height: 60,
                    child: Card(
                      child: Padding(
                        padding: EdgeInsets.all(10),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceBetween,
                          children: [
                            Text(post['date'].toString(), style: TextStyle(fontWeight: FontWeight.w700, fontSize: 23)),
                            Text(post['numItems'].toString(), style: TextStyle(fontSize: 25))
                          ],
                        )
                      )
                    ),
                  ),
                  onTap: () {
                    Navigator.push(context, MaterialPageRoute(builder: (context) => Entry(
                      entry: WasteEntry(
                        date: post['date'], 
                        numItems: post['numItems'].toString(), 
                        url: post['url'], 
                        latitude: post['latitude'].toString(), 
                        longitude: post['longitude'].toString())
                    )));
                  }
                )
              );
            }
          );
        }
        else {
          return Center(child: CircularProgressIndicator());
        }
      }
    );
  }
}
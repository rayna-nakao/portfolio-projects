import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

class NewEntryBody extends StatelessWidget {
  final formKey;
  final url;
  final numItems;
  const NewEntryBody({Key? key, required this.formKey, required this.url, required this.numItems}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Form(
        key: formKey,
        child: Column(
          children: [
            //https://api.flutter.dev/flutter/widgets/Image/loadingBuilder.html
            Image.network(url,
              loadingBuilder: (BuildContext context, Widget child, ImageChunkEvent? loadingProgress) {
                if (loadingProgress == null) {
                  return child;
                }
                return Center(
                  child: Padding(
                    padding: EdgeInsets.all(10),
                    child: CircularProgressIndicator()
                  )
                );
              },
            ),
            Padding(
              padding: EdgeInsets.all(20),
              child: Semantics(
                onTapHint: 'Opens a numeric keyboard to enter the number of items wasted',
                label: 'Text field to enter the number of items wasted',
                textField: true,
                child: TextFormField(
                  style: TextStyle(fontSize: 25),
                  controller: numItems,
                  textAlign: TextAlign.center,
                  decoration: InputDecoration(
                    labelText: 'Number of wasted items',
                    labelStyle: TextStyle(fontSize: 18)
                  ),
                  //https://stackoverflow.com/questions/49577781/how-to-create-number-input-field-in-flutter
                  keyboardType: TextInputType.number,
                  inputFormatters: <TextInputFormatter>[
                    FilteringTextInputFormatter.digitsOnly
                  ],
                  validator: (value) {
                    if (value!.isEmpty) {
                      return 'Please enter a value';
                    }
                  },
                )
              )
            ),
          ],
        )
      )
    );
  }
}
import 'package:test/test.dart';
import 'package:wasteagram/models/waste_entry.dart';

void main() {
  group('Entry', () {
    test('Successfully created waste_entry object - proper values', () {
      final testDate = 'Tuesday, August 10, 2021';
      const testItems = '10';
      const testUrl = 'some url';
      const testLatitude = '23.4';
      const testLongitude = '30.1';

      final test_entry = WasteEntry(
        date: testDate, 
        numItems: testItems, 
        url: testUrl, 
        latitude: testLatitude, 
        longitude: testLongitude
      );

      expect(test_entry.date, testDate);
      expect(test_entry.numItems, testItems);
      expect(test_entry.url, testUrl);
      expect(test_entry.latitude, testLatitude);
      expect(test_entry.longitude, testLongitude);
    });

    test('Successfully created waste_entry object - fields are not empty', () {
      final testDate = 'Monday, August 9, 2021';
      const testItems = '4';
      const testUrl = 'some url';
      const testLatitude = '13.0';
      const testLongitude = '28.1';

      final test_entry = WasteEntry(
        date: testDate, 
        numItems: testItems, 
        url: testUrl, 
        latitude: testLatitude, 
        longitude: testLongitude
      );

      expect(test_entry.date, isNotEmpty);
      expect(test_entry.numItems, isNotEmpty);
      expect(test_entry.url, isNotEmpty);
      expect(test_entry.latitude, isNotEmpty);
      expect(test_entry.longitude, isNotEmpty);
    });
  });
}
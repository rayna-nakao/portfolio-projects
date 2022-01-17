# Wasteagram
### Oreogon State University, CS 492, Portfolio Project
#### Program Requirements: <br />
Build an application for recording food waste. Practice applying the concepts of location services, camera / image picker, permissions, forms, navigation, lists, asynchronous programming, streams, and Firebase backend services. Enhance your application with analytics, crash reporting, accessibility, internationalization, debugging and automated testing.

Implement Wasteagram, a mobile app that enables coffee shop employees to document daily food waste in the form of "posts" consisting of a photo, number of leftover items, the current date, and the location of the device when the post is created. The application should also display a list of all previous posts.

The functional requirements are: <br />
Display a circular progress indicator when there are no previous posts to display in the List Screen. <br />
The List Screen should display a list of all previous posts, with the most recent at the top of the list. <br />
Each post in the List Screen should be displayed as a date, representing the date the post was created, and a number, representing the total number of wasted items recorded in the post. <br />
Tapping on a post in the List Screen should cause a Detail Screen to appear. The Detail Screen's back button should cause the List Screen to appear. <br />
The Detail Screen should display the post's date, photo, number of wasted items, and the latitude and longitude that was recorded as part of the post. <br />
The List Screen should display a large button at the center bottom area of the screen. <br />
Tapping on the large button enables an employee to capture a photo, or select a photo from the device's photo gallery. <br />
After taking a new photo or selecting a photo from the gallery, the New Post screen appears. <br />
The New Post screen displays the photo of wasted food, a Number of Items text input field for entering the number of wasted items, and a large upload button for saving the post. <br />
Tapping on the Number of Items text input field should cause the device to display its numeric keypad. <br />
In the New Post screen, tapping the back button in the app bar should cause the List Screen to appear. <br />
In the New Post screen, tapping the large upload button should cause the List Screen to appear, with the latest post now appearing at the top of the list. <br />
In the New Post screen, if the Number of Items field is empty, tapping the upload button should cause a sensible error message to appear. <br />

In addition to the functional requirements above, your application should meet the following technical requirements: <br />
Use the location, image_picker, cloud_firestore, and firebase_storage packages to meet the functional and technical requirements. <br />
Incorporate the use of Firebase Cloud Storage and Firebase Cloud Firestore for storing images and post data. <br />
Data should not be stored locally on the device. <br />
On the List Screen, the application should display the posts stored in the Firestore database. <br />
On the Detail Screen, the application should display the image stored in the Cloud Storage bucket. <br />
On the New Post screen, tapping the large upload button should store a new post in the Firestore database. <br />
Each "post" in Firestore should have the following attributes: date, imageURL, quantity, latitude and longitude. <br />
The application should incorporate the Semantics widget in multiple places, such as interactive widgets like buttons, to aid accessibility. <br />
The codebase should incorporate a model class. <br />
The codebase should incorporate a few (two or three) simple unit tests that test the model class. <br />
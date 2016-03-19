var express = require('express');
var client = require('twilio');
var app = express();

app.get('/', function (req, res) {
  res.send('MyOrchestra');
});

app.post('/sms/', function (req, res) {
  res.set('Content-Type', 'text/xml');
  res.send(new Buffer('<Response><Message>:^)</Message></Response>'));
});

app.listen(8080, 'localhost', function () {
  console.log('MyOrchestra Server running on port 8080.');
});
var express = require('express');
var client = require('twilio');
var app = express();

app.get('/', function (req, res) {
  res.send('MyOrchestra');
});

app.post('/sms/', function (req, res) {
  console.log(req);
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});
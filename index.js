var express = require('express');
var bodyParser = require('body-parser');
var app = express();

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.get('/', function (req, res) {
  res.send('MyOrchestra');
});

app.get('/sms/', function (req, res) {
  res.set('Content-Type', 'text/xml');
  res.send(new Buffer(smsReply(req.query)));
});

app.post('/sms/', function (req, res) {
  res.set('Content-Type', 'text/xml');
  res.send(new Buffer(smsReply(req.body)));
});

app.listen(8080, 'localhost', function () {
  console.log('MyOrchestra Server running on port 8080.');
});

function smsReply(params) {
  console.log(params);
  var reply = "<Response><Message>";
  reply += "Hey there!";
  reply += "</Message></Response>";
  return reply;
}
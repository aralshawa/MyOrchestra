var express = require('express');
var twilio = require('twilio');
var bodyParser = require('body-parser');
var app = express();

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.get('/', function (req, res) {
  res.send('MyOrchestra');
});

app.get('/sms/', function (req, res) {
  res.set('Content-Type', 'text/xml');
  res.send(new Buffer(twilioReply(req.query)));
});

app.post('/sms/', function (req, res) {
  res.set('Content-Type', 'text/xml');
  res.send(new Buffer(twilioReply(req.body)));
});

app.listen(8080, 'localhost', function () {
  console.log('MyOrchestra Server running on port 8080.');
});

function twilioReply(params) {
  console.log(params);
  var reply = new twilio.TwimlResponse();
  reply.say("Your number is " + params.From);
  return reply.toString();
}
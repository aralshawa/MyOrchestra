var express = require('express');
var twilio = require('twilio');
var bodyParser = require('body-parser');
var fs = require('fs');
var app = express();

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.get('/', function (req, res) {
  res.send('MyOrchestra');
});

app.post('/sms/', function (req, res) {
  res.set('Content-Type', 'text/xml');
  res.send(smsReply(req.body));
});

app.listen(8080, 'localhost', function () {
  console.log('MyOrchestra Server running on port 8080.');
});

function smsReply(params) {
  console.log(params);
  var reply = new twilio.TwimlResponse();
  reply.message("MyOrchestra is a new orchestral experience using intuitive gesture controls to conduct your own symphony. Call this number to record your voice for a truly unique piece.");
  return new Buffer(reply.toString());
}

app.post('/voice/', function (req, res) {
  var rep = new twilio.TwimlResponse();
  rep.say('Welcome to MyOrchestra Voice. ')
    .gather({
      action: '/voice/choose',
      timeout: 2
    }, function() {
      this.say('To record your voice for the next performance, please press 1.');
    })
    .say('Thanks for calling MyOrchestra Voice. Have a great day!')
    .hangup();
  res.set('Content-Type', 'text/xml');
  res.send(rep.toString());
})

app.post('/voice/choose', function (req, res) {
  var rep = new twilio.TwimlResponse();
  if ( Number(req.body.Digits) === 1 ) {
    rep.say('This voice session will start recording after the beep.')
      .record({
        action: '/voice/record',
        maxLength: 600
      })
      .say('Nothing recorded. Thanks for calling MyOrchestra Voice. Have a great day!')
      .hangup();
  }
  else {
    rep.say('Thanks for calling MyOrchestra Voice. Have a great day!')
      .hangup();
  }
  res.set('Content-Type', 'text/xml');
  res.send(rep.toString());
})

app.post('/voice/record', function (req, res) {
  var rep = new twilio.TwimlResponse();
  rep.say('Nice work! Thanks for calling MyOrchestra Voice. Have a great day!')
    .hangup();
  res.set('Content-Type', 'text/xml');
  res.send(rep.toString());
  var file = fs.createWriteStream("recordings/voice.wav");
  http.get(req.body.RecordingUrl, function(res) {
    res.pipe(file);
  });
})
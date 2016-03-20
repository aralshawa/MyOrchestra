var express = require('express');
var twilio = require('twilio');
var bodyParser = require('body-parser');
var fs = require('fs');
var https = require('https');
var url = require('url');
var exphbs  = require('express-handlebars');

var app = express();
var voiceConfig = {
  voice: 'female', 
  language: 'en'
};

app.engine('handlebars', exphbs({defaultLayout: 'main'}));
app.set('view engine', 'handlebars');

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.use('/public/', express.static('recordings'));

app.get('/', function (req, res) {
  res.render('home');
});

app.post('/sms/', function (req, res) {
  var reply = new twilio.TwimlResponse();
  reply.message('MyOrchestra is a new orchestral experience using intuitive ' +
    'gesture controls to conduct your own symphony.' +
    'Call +16474928264 to record your voice for a truly unique piece.');
  res.set('Content-Type', 'text/xml');
  res.send(reply.toString());
  console.log('SMS   ' + req.body.From);
});

app.post('/voice/', function (req, res) {
  var rep = new twilio.TwimlResponse();
  rep.say('Welcome to MyOrchestra Voice.', voiceConfig)
    .gather({
      action: '/voice/choose',
      numDigits: 1,
      timeout: 5
    }, function() {
      this.say('To record your voice for the next performance, please press 1.', voiceConfig);
    })
    .say('Thanks for calling MyOrchestra Voice. Have a great day!', voiceConfig)
    .hangup();
  res.set('Content-Type', 'text/xml');
  res.send(rep.toString());
  console.log('HELO');
});

app.post('/voice/choose', function (req, res) {
  var rep = new twilio.TwimlResponse();
  if ( Number(req.body.Digits) === 1 ) {
    rep.pause()
      .say('This voice session will start recording for up to 10 seconds after the beep. Press any key to stop the recording.', voiceConfig)
      .record({
        action: '/voice/record',
        maxLength: 10
      })
      .say('Nothing recorded. Thanks for calling MyOrchestra Voice. Have a great day!', voiceConfig)
      .hangup();
  }
  else {
    rep.say('Thanks for calling MyOrchestra Voice. Have a great day!', voiceConfig)
      .hangup();
  }
  res.set('Content-Type', 'text/xml');
  res.send(rep.toString());
  console.log('RESP');
});

app.post('/voice/record', function (req, res) {
  var rep = new twilio.TwimlResponse();
  rep.say('Nice work! Thanks for calling MyOrchestra Voice. Have a great day!', voiceConfig)
    .hangup();
  res.set('Content-Type', 'text/xml');
  res.send(rep.toString());
  console.log('RECR');
  var remote = req.body.RecordingUrl;
  var filename = 'recordings/' + remote.match(/[^\/]*$/g)[0] + '.wav';
  var file = fs.createWriteStream(filename);
  https.get(remote, function(res) {
    res.pipe(file);
    console.log('FILE: ' + filename);
  });
});

app.get('/voice/recordings', function (req, res) {
  res.set('Content-Type', 'application/json');
  fs.readdir('recordings/', function( err, files ) {
    res.send( JSON.stringify(files.filter(function(filename) {
      return /(\.wav)$/.test(filename);
    })));
  });
});

app.listen(8080, 'localhost', function () {
  console.log('MyOrchestra Server running on port 8080.');
});
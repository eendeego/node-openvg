//
// A fake panic dashboard
// http://www.panic.com/blog/2010/03/the-panic-status-board/
//

var openVG = require('../openvg');

var util = require('./modules/util');
var text = require('./modules/text');

function terminate() {
  util.finish();
  console.log("Making a clean exit.");
}

var width, height;
var MAIL_QUEUES_HEIGHT = 0.2;

var dashboardData = {
  mails : [
    {
      project: 'Transmit',
      messages: 18,
      days: 1,
      color: [238, 192, 70]
    },
    {
      project: 'Coda',
      messages: 29,
      days: 3,
      color: [104, 192, 44]
    },
    {
      project: 'Unison',
      messages: 48,
      days: 3,
      color: [232, 84, 63]
    },
    {
      project: 'CandyBar',
      messages: 8,
      days: 2,
      color: [116, 88, 186]
    },
    {
      project: 'General',
      messages: 3,
      days: 0,
      color: [77, 124, 241]
    }
  ]
};

function paintMailQueues() {
  var mailQueues = dashboardData.mails.length;
  var margin = 20;
  var barMargin = 8;
  var countHeight = 30;
  var countMargin = 4;

  var baseHeight = height*(1.0-MAIL_QUEUES_HEIGHT);

  util.strokeWidth(0);

  var maxH =0;
  for(var i=0; i<mailQueues; i++) {
    msgs = dashboardData.mails[i].messages;
    if (msgs > maxH) { maxH = msgs; }
  }

  var barWidth = (width - 2 * margin) / mailQueues;
  for(var i=0; i<mailQueues; i++) {
    var offset = margin + (width - 2 * margin) * i / mailQueues;
    util.fill(dashboardData.mails[i].color[0],
              dashboardData.mails[i].color[1],
              dashboardData.mails[i].color[2],
              1);

    var barHeight = (height * MAIL_QUEUES_HEIGHT - countHeight) * dashboardData.mails[i].messages / maxH;
    util.rect(offset + barMargin,
              baseHeight,
              barWidth - barMargin * 2,
              barHeight);

    util.fill(255, 255, 255, 1.0);
    text.textMiddle(offset + barMargin + (barWidth - barMargin * 2) / 2,
                    baseHeight + barHeight + countMargin,
                    dashboardData.mails[i].messages + " / " + dashboardData.mails[i].days,
                    util.sansTypeface, countHeight - 2 * countMargin);
  }

  util.stroke(255, 255, 255, 1.0);
  util.strokeWidth(2);
  util.line(margin, baseHeight,
            width - margin, baseHeight);
  util.strokeWidth(0);

  // console.log("line: " +
  //             width * margin + ", " + baseHeight + ", " +
  //             width * (1.0-margin) + ", " + baseHeight);
}

function paintDashboard() {
  variate();

  util.background(0, 0, 0);

  paintMailQueues();

  util.end();
}

function requestPaint() {
  process.nextTick(paintDashboard);
}

function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function minZero(value) {
  return value < 0 ? 0 : value;
}

function variate() {
  dashboardData.mails.forEach(function(mq) {
    mq.messages = minZero(mq.messages + getRandomInt(-5, 5));
    mq.days     = minZero(mq.days     + getRandomInt(-2, 2));
  });
}

util.init();

width  = openVG.screen.width;
height = openVG.screen.height;

util.start();

var animTime = 1000 / 24; // 24 fps

var paintInterval = setInterval(requestPaint, animTime);

process.on('exit', terminate);

console.log("Press return to exit.");
process.stdin.resume();
process.stdin.setEncoding('utf8');

process.stdin.once('data', function (chunk) {
  clearInterval(paintInterval);
  process.stdin.pause();
});

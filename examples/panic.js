//
// A fake panic dashboard
// http://www.panic.com/blog/2010/03/the-panic-status-board/
//

'use strict';

const openVG = require('../index');

const util = require('./modules/util');
const text = require('./modules/text');

function terminate() {
  util.finish();
  console.log('Making a clean exit.');
}

util.init();

const {width, height} = openVG.screen;

const MAIL_QUEUES_HEIGHT = 0.2;

const dashboardData = {
  mails: [
    {
      project: 'Transmit',
      messages: 18,
      days: 1,
      color: [238, 192, 70],
    },
    {
      project: 'Coda',
      messages: 29,
      days: 3,
      color: [104, 192, 44],
    },
    {
      project: 'Unison',
      messages: 48,
      days: 3,
      color: [232, 84, 63],
    },
    {
      project: 'CandyBar',
      messages: 8,
      days: 2,
      color: [116, 88, 186],
    },
    {
      project: 'General',
      messages: 3,
      days: 0,
      color: [77, 124, 241],
    },
  ],
};

function paintMailQueues() {
  const mailQueues = dashboardData.mails.length;
  const margin = 20;
  const barMargin = 8;
  const countHeight = 30;
  const countMargin = 4;

  const baseHeight = height * (1.0 - MAIL_QUEUES_HEIGHT);

  util.strokeWidth(0);

  let maxH = 0;
  for (let i = 0; i < mailQueues; i++) {
    const msgs = dashboardData.mails[i].messages;
    if (msgs > maxH) {
      maxH = msgs;
    }
  }

  const barWidth = (width - 2 * margin) / mailQueues;
  for (let i = 0; i < mailQueues; i++) {
    const offset = margin + (width - 2 * margin) * i / mailQueues;
    util.fill(
      dashboardData.mails[i].color[0],
      dashboardData.mails[i].color[1],
      dashboardData.mails[i].color[2],
      1
    );

    const barHeight =
      (height * MAIL_QUEUES_HEIGHT - countHeight) *
      dashboardData.mails[i].messages /
      maxH;
    util.rect(
      offset + barMargin,
      baseHeight,
      barWidth - barMargin * 2,
      barHeight
    );

    util.fill(255, 255, 255, 1.0);
    text.textMiddle(
      offset + barMargin + (barWidth - barMargin * 2) / 2,
      baseHeight + barHeight + countMargin,
      dashboardData.mails[i].messages + ' / ' + dashboardData.mails[i].days,
      util.fonts.sansTypeface,
      countHeight - 2 * countMargin
    );
  }

  util.stroke(255, 255, 255, 1.0);
  util.strokeWidth(2);
  util.line(margin, baseHeight, width - margin, baseHeight);
  util.strokeWidth(0);

  // console.log('line: ' +
  //             width * margin + ', ' + baseHeight + ', ' +
  //             width * (1.0-margin) + ', ' + baseHeight);
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
  dashboardData.mails.forEach(mq => {
    mq.messages = minZero(mq.messages + getRandomInt(-5, 5));
    mq.days = minZero(mq.days + getRandomInt(-2, 2));
  });
}

util.start();

const animTime = 1000 / 24; // 24 fps

const paintInterval = setInterval(requestPaint, animTime);

process.on('exit', terminate);

console.log('Press return to exit.');
process.stdin.resume();
process.stdin.setEncoding('utf8');

process.stdin.once('data', () => {
  clearInterval(paintInterval);
  process.stdin.pause();
});

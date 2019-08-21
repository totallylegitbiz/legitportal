const path = require("path");
const util = require("util");
const exec = util.promisify(require("child_process").exec);

const cwd = path.resolve(__dirname, "..");

const listCmd = "platformio device list --json-output";
const runCmd = device =>
  `DEBUG=0 DEVICE_ROLE=0 LED_CNT=100 PORT=${device} make upload`;

const matchStr = "usbserial";

let devices = [];

const delay = ms => new Promise(resolve => setTimeout(resolve, ms));

const run = async () => {
  const { stdout: payload, stderr } = await exec(listCmd, { cwd });

  if (stderr) {
    console.error({ stderr });
    process.exit();
  }
  const nextDevices = JSON.parse(payload)
    .map(({ port }) => port)
    .filter(device => device.indexOf(matchStr) !== -1);

  const ps = nextDevices.map(async device => {
    if (devices.indexOf(device) !== -1) {
      return;
    }
    const { stdout, stderr } = await exec(runCmd(device), { cwd });
    console.log({ stdout });
  });

  devices = nextDevices;

  await Promise.all(ps);
};

(async () => {
  do {
    await run();
    await delay(500);
  } while (true);
})().catch(console.error);

const path = require("path");
const fs = require("fs");

const candleFileNum = "2";

const srcFile = `candle${candleFileNum}.csv`;
const outFile = `../Candle${candleFileNum}.h`;

const targetFile = path.resolve(__dirname, outFile);
const sourceFile = path.resolve(__dirname, srcFile);

const fileContent = fs.readFileSync(sourceFile).toString();

const srcRows = fileContent.trim().split("\n");

let row = [];

const sourceHertz = 175;
const sourceSize = srcRows.length;
const sourceSeconds = Math.floor(sourceSize / sourceHertz);

const sampleSeconds = sourceSeconds; // Can set this to lower.
const targetHertz = 20;
const targetSampleCount = targetHertz * sampleSeconds;

const sourceSampleSize = Math.floor(
  (sampleSeconds / sourceSeconds) * sourceSize
);

const downSample = (srcSamples, targetSampleCount) => {
  const srcLen = srcSamples.length;
  // 10 / 5 =  2
  const groupSize = Math.floor(srcLen / targetSampleCount);

  const out = [];

  for (let i = 0; i < srcLen - groupSize; i = i + groupSize) {
    let avg = 0;

    for (let j = i; j < i + groupSize - 1; j++) {
      const [_, intensity] = srcSamples[j].split(",");

      avg += intensity * 1;
    }

    out.push(avg / groupSize);
  }

  return out;
};

let minVal = null;
let maxVal = null;

const numbers = downSample(
  srcRows.splice(0, sourceSampleSize),
  targetSampleCount
)
  .map(intensity => {
    if (minVal === null || intensity < minVal) {
      minVal = intensity;
    }

    if (maxVal === null || intensity > maxVal) {
      maxVal = intensity;
    }

    return intensity;
  })
  .map(i => {
    const adjustedIntensity = (i - minVal) / (maxVal - minVal);

    return Math.floor(adjustedIntensity * 255);
  });

const output = `extern const uint8_t PROGMEM candle${candleFileNum}[] = {${numbers.join(
  ","
)}};
const int candle${candleFileNum}Size = ${numbers.length};`;

fs.writeFileSync(targetFile, output);

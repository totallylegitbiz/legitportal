const path = require("path");
const fs = require("fs");

const srcFile = "candle1.csv";
const outFile = "../RealCandle.h";

const targetFile = path.resolve(__dirname, outFile);
const sourceFile = path.resolve(__dirname, srcFile);

const fileContent = fs.readFileSync(sourceFile).toString();

const srcRows = fileContent.trim().split("\n");

let row = [];

// const numWidth = Math.ceil(srcRows.length / 2);
// const numbers = srcRows.reduce((p, c) => {
//   if (row.length === numWidth) {
//     p.push(row);
//     row = [];
//   }

//   const [_, intensity] = c.split(",");

//   const v = Math.floor(intensity * 255);

//   row.push(v);

//   return p;
// }, []);

// const rows = numbers.map(row => {
//   return "{" + numbers.join(",\n") + "}";
// });

// const output = `unsigned short int candle1[][${numWidth}] = {${rows.join(
//   ",\n"
// )}};const int candle1Width = ${numWidth};`;

const sourceHertz = 175;
const sourceSize = srcRows.length;
const sourceSeconds = Math.floor(sourceSize / sourceHertz);

const sampleSeconds = 60;
const targetHertz = 20;
const targetSampleCount = targetHertz * sampleSeconds;

const sourceSampleSize = (sampleSeconds / sourceSeconds) * sourceSize;

const downSample = (srcSamples, targetSampleCount) => {
  const srcLen = srcSamples.length;
  // 10 / 5 =  2
  const groupSize = Math.floor(srcLen / targetSampleCount);

  const out = [];

  for (let i = 0; i < srcLen - groupSize; i = i + groupSize) {
    let avg = 0;
    for (let j = i; j < i + groupSize - 1; j++) {
      const [_, intensity] = srcSamples[j].split(",");

      // console.log(intensity);
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
    // const [_, intensity] = r.split(",");

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

// const rows = [];

// const fullLen = numbers.length;

// for (let i = 0; i < samples; i++) {
//   const baseNum = 1000 / 500;
// }

const output = `extern const uint8_t PROGMEM candle1[] = {${numbers.join(
  ",\n"
)}};const int candle1Size = ${numbers.length};`;

// #####

fs.writeFileSync(targetFile, output);

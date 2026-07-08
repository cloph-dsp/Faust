const fs = require('fs');
const buf = fs.readFileSync('D:\\Github\\VST3 Developments\\BronzeNoise\\resources\\KenyanCoffeeRg.otf');
const numTables = buf.readUInt16BE(4);
for (let i = 0; i < numTables; i++) {
  const off = 12 + i * 16;
  const tag = buf.toString('ascii', off, off + 4);
  if (tag === 'name') {
    const tbl_off = buf.readUInt32BE(off + 8);
    const fmt = buf.readUInt16BE(tbl_off);
    const count = buf.readUInt16BE(tbl_off + 2);
    const str_off = buf.readUInt16BE(tbl_off + 4);
    console.log('format=' + fmt + ' count=' + count + ' str_off=' + str_off);
    for (let j = 0; j < count; j++) {
      const rec = tbl_off + 6 + j * 12;
      const platformID = buf.readUInt16BE(rec);
      const encodingID = buf.readUInt16BE(rec + 2);
      const languageID = buf.readUInt16BE(rec + 4);
      const nameID = buf.readUInt16BE(rec + 6);
      const length = buf.readUInt16BE(rec + 8);
      const str_offset = buf.readUInt16BE(rec + 10);
      if (nameID === 1 || nameID === 4 || nameID === 6) {
        const data = buf.slice(tbl_off + str_off + str_offset, tbl_off + str_off + str_offset + length);
        let s;
        if (platformID === 3) {
          // UTF-16BE: swap bytes manually
          const swapped = Buffer.alloc(data.length);
          for (let k = 0; k < data.length; k += 2) {
            swapped[k] = data[k + 1];
            swapped[k + 1] = data[k];
          }
          s = swapped.toString('utf16le');
        } else {
          s = data.toString('ascii');
        }
        console.log('nameID=' + nameID + ' plat=' + platformID + ' enc=' + encodingID + ' lang=' + languageID + ': ' + JSON.stringify(s));
      }
    }
  }
}
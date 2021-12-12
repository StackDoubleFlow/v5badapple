const fs = require('fs');

let buf = Buffer.alloc(0);

let ptrItems = "";
let dataLen = 0;
const files = fs.readdirSync("./images_jpg");
for (const file of files) {
    const fileData = fs.readFileSync("./images_jpg/" + file);
    ptrItems += `    ${dataLen},\n`;
    dataLen += fileData.length;
    
    buf = Buffer.concat([buf, fileData]);
}
ptrItems = ptrItems.substr(0, ptrItems.length - 2);

let out = `
#include <stdint.h>

const uint32_t frame_data_idx[] = {
${ptrItems}
};
`;

fs.writeFileSync('frame_data.c', out);
fs.writeFileSync('frame_data.bin', buf);

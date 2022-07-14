const {spawn} = require('child_process');
const sharp = require('sharp');
const fs = require('fs');
const path = require('path');

const fileNameInput = 'bike.png';
const fileNameOutput = 'output.ppm';

const filePathRead = path.join(__dirname, '../../uploads/') + fileNameInput;

const filePathWrite = path.join(__dirname, '../../uploads/') + fileNameOutput;

const imageInfoToHex = (data) => {

    const digits = String(data);
    var hexString = ''
    for (let i = 0; i < digits.length; i++)
    {
        hexString = hexString + (Number(digits[i]) + 48).toString(16)
    }
    return hexString;
}

const data = sharp(filePathRead)
    .raw()
    .toBuffer((err, data, info) =>{

        if(err) throw err

        const headerString = '50360A' + imageInfoToHex(info.width) + '20' + imageInfoToHex(info.height) + '0A3235350A'
        var fileHeader = new Buffer.from(headerString, 'hex');
        // fileHeader = [80, 54, 10, info.width, 20, info.height, 10, 32, 35, 35, 10];

        fs.writeFile(filePathWrite, fileHeader, { flag: 'w' }, err => {
            if(!err){
                fs.writeFile(filePathWrite, data, { flag: 'a+' }, err => {});
            }
        });
    })




const filter = (args, callback) => {
    const filtro_cmd = spawn('./utils/4898695/obligatorio', ['caricatura', '5', './utils/4898695/auto2.ppm', './utils/4898695//060622.ppm'])
    filtro_cmd.stdout.on('data', (data) => {
        console.log(`stdout: ${data}`);
     });
    filtro_cmd.on('close', (code) => {
        console.log(`child process exited with code ${code}`);
    });
}

exports.filter = filter

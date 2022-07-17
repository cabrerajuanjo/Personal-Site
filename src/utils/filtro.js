const {spawn} = require('child_process');
const sharp = require('sharp');
const fs = require('fs');
const path = require('path');

const imageInfoToHex = (data) => {
    const digits = String(data);
    var hexString = ''
    for (let i = 0; i < digits.length; i++)
        {hexString = hexString + (Number(digits[i]) + 48).toString(16)}
    return hexString;
}

const convertFileToPPM = (fileNameInput, fileNameOutput, callback) => {
    const filePathRead = path.join(__dirname, '../../uploads/') + fileNameInput;
    const filePathWrite = path.join(__dirname, '../../uploads/') + fileNameOutput;
    sharp(filePathRead)
        .raw()
        .removeAlpha()
        .toBuffer((error, data, info) =>{
            if(error) return error
            console.log(info.width)
            console.log(info.height)
            const headerString = '50360A' + imageInfoToHex(info.width) + '20' + imageInfoToHex(info.height) + '0A3235350A'
            const fileHeader = new Buffer.from(headerString, 'hex');

            fs.writeFile(filePathWrite, fileHeader, { flag: 'w' }, err => {
                if(!err){
                    fs.writeFile(filePathWrite, data, { flag: 'a+' }, err => {
                        if(!err){
                            fs.unlink(filePathRead, (err) => {
                                if (err) throw err;
                                callback();
                            });
                        }
                    });
                }
            });
        })
}

const getHeaderDataFromPPM = (data, start, end) => {
    const dataString = data.subarray(start , end).toString('hex');
    // return parseInt(dataString, '16');
    dataNumber = 0;
    for (var i = dataString.length - 1, j = 0; i > 0; i-=2, j++) {
        dataNumber += parseInt(dataString[i])*(10**j)
    }
    return dataNumber
}

const convertToOriginalFileType = (fileNameInput, fileNameOutput, callback) => {
    const filePathRead = path.join(__dirname, '../../uploads/') + fileNameInput;
    const filePathWrite = path.join(__dirname, '../../public/results/') + fileNameOutput;
    fs.readFile(filePathRead, (err, data) => {
        if(!err)
        {
            // Get 3rd CR caracter from file (start of raw pixel data)
            const firstCR = data.indexOf('0A', 'hex')
            const secondCR = data.indexOf('0A', firstCR + 1 , 'hex')
            const startOfRawPixelData = data.indexOf('0A', secondCR + 1 , 'hex') + 1;

            const firstSpace = data.indexOf('20', 'hex')
            const width = getHeaderDataFromPPM(data, firstCR + 1, firstSpace);
            console.log(width)
            const height = getHeaderDataFromPPM(data, firstSpace + 1, secondCR);
            console.log(height)

            //slice bufer from startOfRawPixelData
            const rawPixelDataBuffer = data.subarray(startOfRawPixelData, data.length);
            //load it into Sharp
            // fs.writeFile(filePathWrite + 'raw', rawPixelDataBuffer, { flag: 'w' }, (err) => {});
            sharp(rawPixelDataBuffer, {
                    raw: {
                        width,
                        height,
                        channels: 3
                    }
                })
            //save it as a png or jpg
                .toFile(filePathWrite)
            //delete previous file
                .then((info) => fs.unlink(filePathRead, (err) => {
                    if(err) throw err;
                    callback()
                }))
                .catch((err) => {throw err})

        }
    })
}


const filter = (fileNameInput, filterType, parameter, fileNameOutput, callback) => {
    const filePathRead = path.join(__dirname, '../../uploads/') + fileNameInput;
    const filePathWrite = path.join(__dirname, '../../uploads/') + fileNameOutput;
    const exec = path.join(__dirname, './4898695/') + 'obligatorio';

    const filtro_cmd = spawn(exec, [filterType, parameter, filePathRead, filePathWrite]);
    filtro_cmd.stdout.on('data', (data) => {
        // console.log(`stdout: ${data}`);
     });
    filtro_cmd.on('close', (code) => {
        // console.log(`child process exited with code ${code}`);
        fs.unlink(filePathRead, (err) => {if(err) throw err;})
        callback();
    });
}

module.exports = {filter, convertFileToPPM, convertToOriginalFileType};

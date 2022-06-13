const {spawn} = require('child_process');

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

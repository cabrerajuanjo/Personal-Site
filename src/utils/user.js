const uuid = require('uuid');
const uuidTime = require('uuid-time').v1;
const cronJob = require('cron').CronJob;
const fs = require('fs');
const path = require('path');



const userFilesLookup = [];

//Generates and pushes an UUID into active users array. UUID is V1 so gets time coded
const generateNewUser = () => {
    sessionID = (uuid.v1());
    userFilesLookup.push({
        [sessionID]: ''
    });
    return sessionID;
}

//Renders Main Page either when first acces page or when needs to get UUID (when is deleted due to time)
const mainRender = (res) => {
    const sessionID = generateNewUser();
    res.render('index', {
        title: 'Main',
        name: 'Juanjo',
        sessionID
    })
}

//Deletes files either from results folder or uploads folder
const deleteFile = (fileName) => {
    const filePathToCleanResults = path.join(__dirname, '../../public/results/') + fileName;
    const filePathToCleanUploads = path.join(__dirname, '../../uploads/') + fileName;
    fs.unlink(filePathToCleanResults, (err) => {
        if (!err) console.log('File deleted!');
    });
    fs.unlink(filePathToCleanUploads, (err) => {
        if (!err) console.log('File deleted!');
    });
}

//Deletes user that is older than 10 minutes (600000ms) and its asociated file if corresponds
const cleanUserArray = () => {
    console.log('clean user array')
    userFilesLookup.forEach((element, index) => {
        userUUID = Object.keys(element)[0];
        currentTimeStamp = new Date().getTime();
        if (!uuid.validate(userUUID) || (currentTimeStamp - parseInt(uuidTime(userUUID), 10)) > 600000) {
            const fileName = Object.values(element)[0]
            if (fileName) {
                deleteFile(fileName)
            }
            console.log("Something to splice");
            userFilesLookup.splice(index, 1);
        }
    })
}

//Schedule user and files cleaning each 15 minutes
var scheduleCleanup = new cronJob(
    '0 */15  * * * *',
    cleanUserArray,
    null,
    true,
    'America/Montevideo'
);

//Looks up if UUID gotten from form when uploading image is registrered (user could be not registrered or deleted due to time out)
//  If its already on userFilesLookup, checks if there's already a file asociated
//      Checks if there's already a file asociated If it is, it deletes it
//      Accepts the file and asociates irit with the user ID and
//  If it doesn't find the ID in userFilesLookup it redirects to Main in order to get one.
//accepts the file and it asociates it
const validatorMiddleware = (req, res, next) => {
    var foundID = false;
    const user = userFilesLookup.forEach((user, index) => {
        if (uuid.validate(Object.keys(user)[0])) {
            if (req.body.userID in userFilesLookup[index]) {
                if (userFilesLookup[index][req.body.userID] !== '') {
                    deleteFile(userFilesLookup[index][req.body.userID]);
                }
                foundID = true;
                (userFilesLookup[index])[req.body.userID] = req.file.filename;
                return next();
            }
        }
    })

    if ((userFilesLookup.length === 0) || (foundID === false)) {
        deleteFile(req.file.filename);
        return mainRender(res);
    }
}

// const doesUserHaveValidID = (req, res, next) => {
//     const user = userFilesLookup.forEach((user) => {
//         if (user.hasOwnProperty(req.query.userID))
//         {
//             next();
//         }else
//         {
//             mainRender(res);
//         }
//     })
// }
module.exports = {
    userFilesLookup,
    generateNewUser,
    cleanUserArray,
    validatorMiddleware,
    mainRender,
}
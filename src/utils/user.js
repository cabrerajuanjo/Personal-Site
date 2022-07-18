const uuid = require('uuid');
const uuidTime = require('uuid-time').v1;
const cronJob = require('cron').CronJob;
const fs = require('fs');
const path = require('path');



const userFilesLookup = [];

const generateNewUser = () => {
    sessionID = (uuid.v1());
    userFilesLookup.push({
        [sessionID]: ''
    });
    return sessionID;
}

const mainRender = (res) => {
    const sessionID = generateNewUser();
    res.render('index', {
        title: 'Main',
        name: 'Juanjo',
        sessionID
    })
}

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

const cleanUserArray = () => {
    console.log('clean user array')
    userFilesLookup.forEach((element, index) => {
        userUUID = Object.keys(element)[0];
        currentTimeStamp = new Date().getTime();
        if( !uuid.validate(userUUID) || (currentTimeStamp - parseInt(uuidTime(userUUID), 10)) >  600000)
        {
            const fileName = Object.values(element)[0]
            if(fileName)
            {
                deleteFile(fileName)
            }
            console.log("Something to splice");
            userFilesLookup.splice(index, 1);
        }
    })
}

var scheduleCleanup = new cronJob(
	'0 */15  * * * *',
	cleanUserArray,
	null,
	true,
	'America/Montevideo'
);

const validatorMiddleware = (req, res, next) => {
    var foundID = false;
    const user = userFilesLookup.forEach((user, index) => {
        if (uuid.validate(Object.keys(user)[0]))
        {
            if(req.body.userID in userFilesLookup[index])
            {
                if(userFilesLookup[index][req.body.userID] !== '')
                {
                    deleteFile(userFilesLookup[index][req.body.userID]);
                }
                foundID = true;
                (userFilesLookup[index])[req.body.userID] = req.file.filename;
                return next();
            }
        }
    })

    if ((userFilesLookup.length === 0) || (foundID === false))
    {
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

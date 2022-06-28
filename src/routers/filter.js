const express = require('express');
const filter = require('../utils/filtro.js');
const path = require('path');
const multer = require('multer');
const uuid = require('uuid').v4;

const router = new express.Router();

const userFilesLookup = []

// const formParse = multer();

router.use(express.urlencoded({
  extended: true
}));

const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null,path.join(__dirname, '../../uploads/'));
    },
    filename: (req, file, cb) => {
        cb(null, uuid() + '-' + file.originalname);
    }
})

const upload = multer({
    storage,
    limits: {
        fileSize: 10000000
    },
    // fileFilter(req, file, cb){
    //     cb(new Error("File must be either JPG, PNG or PNM"))
    // }
})

const validator = (req, res, next) => {
    const user = userFilesLookup.forEach((user) => {

        if (user[req.body.userID] || user[req.body.userID] == '')
        {
            (userFilesLookup[userFilesLookup.findIndex(element => user)])[req.body.userID] = req.file.filename;
            console.log(userFilesLookup);
            next();
        }
    })

    console.log("ERROR");
    res.status(403).send;
}

router.get('/filter/upload', (req, res) => {
    console.log(userFilesLookup)
    res.render('upload', {
        title: 'Upload Image',
        name: 'Juanjo'
    })
    //filter.filter("", "")
})

router.post('/filter/result', upload.single('input_img'), validator, (req, res) => {
    console.log("File name on server ", req.file.filename);
    console.log("File path on server ", req.file.path);
    console.log("Original name ", req.file.originalname);
    console.log("Filter options", req.body);

    res.render('result', {
        title: 'Result'
    })
})

const filterRouter = router;

module.exports = {
    filterRouter,
    userFilesLookup
};
